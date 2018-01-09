#include "pipeline.h"

#include <common/error.h>
#include <renderer/vertex.h>

namespace Reweng
{
    VulkanPipeline::VulkanPipeline()
        : isInitialized(false)
    {
    }

    VulkanPipeline::VulkanPipeline(const VulkanPipelineSettings& settings)
        : isInitialized(true)
        , LogicalDevice(settings.LogicalDevice)
        , RenderPass(settings.RenderPass)
        , pSwapchain(settings.pSwapchain)
        , VertexInputAttributeDescriptions(settings.VertexInputAttributeDescriptions)
        , VertexInputBindingDescription(settings.VertexInputBindingDescription)
        , DescriptorSetLayout(settings.DescriptorSetLayout)
        , VertexShader(settings.VertexShader)
        , FragmentShader(settings.FragmentShader)
    {
    }

    std::vector<VkPipelineShaderStageCreateInfo> VulkanPipeline::GetShaderStageInfo()
    {
        std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
        
        VkPipelineShaderStageCreateInfo vertex = {};
        vertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex.module = VertexShader;
        vertex.pName = "main"; //TODO: Add this info in shader module
        shaderStageInfos.push_back(vertex);

        VkPipelineShaderStageCreateInfo fragment = {};
        fragment.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment.module = FragmentShader;
        fragment.pName = "main";
        shaderStageInfos.push_back(fragment);

        return shaderStageInfos;
    }

    VkPipelineVertexInputStateCreateInfo VulkanPipeline::GetVertexInputStateInfo()
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)VertexInputAttributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();

        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo VulkanPipeline::GetInputAssemblyStateInfo()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        return inputAssemblyInfo;
    }

    std::optional<VkPipelineTessellationStateCreateInfo> VulkanPipeline::GetTesselaltionStateInfo()
    {
        return std::nullopt;
    }

    VkPipelineViewportStateCreateInfo VulkanPipeline::GetViewportStateInfo()
    {
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.height = static_cast<float>(pSwapchain->GetExtent().height);
        viewport.width = static_cast<float>(pSwapchain->GetExtent().width);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        
        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = { pSwapchain->GetExtent().width, pSwapchain->GetExtent().height };
        
        VkPipelineViewportStateCreateInfo viewportInfo = {};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &scissor;

        return viewportInfo;
    }

    VkPipelineRasterizationStateCreateInfo VulkanPipeline::GetRasterizationStateInfo()
    {
        VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
        rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationInfo.depthClampEnable = VK_FALSE;
        rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationInfo.lineWidth = 1.0f;
        rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationInfo.depthBiasEnable = VK_FALSE;
        rasterizationInfo.depthBiasConstantFactor = 0.0f;
        rasterizationInfo.depthBiasClamp = 0.0f;
        rasterizationInfo.depthBiasSlopeFactor = 0.0f;

        return rasterizationInfo;
    }

    VkPipelineMultisampleStateCreateInfo VulkanPipeline::GetMultisampleStateInfo()
    {
        VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.minSampleShading = 1.0f;
        multisampleInfo.pSampleMask = nullptr;
        multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleInfo.alphaToOneEnable = VK_FALSE;

        return multisampleInfo;
    }

    std::optional<VkPipelineDepthStencilStateCreateInfo> VulkanPipeline::GetDepthStencilStateInfo()
    {
        return std::nullopt;
    }

    VkPipelineColorBlendStateCreateInfo VulkanPipeline::GetColorBlendStateInfo()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachmentInfo = {}; //TODO: READ MORE
        colorBlendAttachmentInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentInfo.blendEnable = VK_FALSE;
        colorBlendAttachmentInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentInfo.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentInfo.alphaBlendOp = VK_BLEND_OP_ADD;
        
        VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &colorBlendAttachmentInfo;
        colorBlendInfo.blendConstants[0] = 0.0f;
        colorBlendInfo.blendConstants[1] = 0.0f;
        colorBlendInfo.blendConstants[2] = 0.0f;
        colorBlendInfo.blendConstants[3] = 0.0f;

        return colorBlendInfo;
    }

    VkPipelineLayoutCreateInfo VulkanPipeline::GetPipelineLayoutInfo()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = 0;
        VK_VALIDATE(vkCreatePipelineLayout(LogicalDevice, &pipelineLayoutInfo, nullptr, &PipelineLayout));

        return pipelineLayoutInfo;
    }
   
    std::optional<VkPipelineDynamicStateCreateInfo> VulkanPipeline::GetDynamicStateInfo()
    {
        return std::nullopt;
    }

    void VulkanPipeline::CreatePipeline()
    {
        VK_CHECK(isInitialized, "Cannot create a pipeline without settings.");

        std::vector<VkPipelineShaderStageCreateInfo>         shaderStageInfos = GetShaderStageInfo();
        VkPipelineVertexInputStateCreateInfo                 vertexInputInfo = GetVertexInputStateInfo();
        std::optional<VkPipelineTessellationStateCreateInfo> tessellationStateInfo = GetTesselaltionStateInfo();
        VkPipelineInputAssemblyStateCreateInfo               inputAssemblyInfo = GetInputAssemblyStateInfo();
        VkPipelineViewportStateCreateInfo                    viewportInfo = GetViewportStateInfo();
        VkPipelineRasterizationStateCreateInfo               rasterizationInfo = GetRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo                 multisampleInfo = GetMultisampleStateInfo();
        VkPipelineColorBlendStateCreateInfo                  colorBlendInfo = GetColorBlendStateInfo();
        std::optional<VkPipelineDepthStencilStateCreateInfo> depthStencilInfo = GetDepthStencilStateInfo();
        VkPipelineLayoutCreateInfo                           pipelineLayoutInfo = GetPipelineLayoutInfo();
        std::optional<VkPipelineDynamicStateCreateInfo>      dynamicStateInfo = GetDynamicStateInfo();

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStageInfos.size();
        pipelineInfo.pStages = shaderStageInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.pTessellationState = tessellationStateInfo.has_value() ? &tessellationStateInfo.value() : nullptr;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &rasterizationInfo;
        pipelineInfo.pMultisampleState = &multisampleInfo;
        pipelineInfo.pDepthStencilState = depthStencilInfo.has_value() ? &depthStencilInfo.value() : nullptr;
        pipelineInfo.pColorBlendState = &colorBlendInfo;
        pipelineInfo.pDynamicState = dynamicStateInfo.has_value() ? &dynamicStateInfo.value() : nullptr;
        pipelineInfo.layout = PipelineLayout;
        pipelineInfo.renderPass = RenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        VK_VALIDATE(vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &Handle));
    }
}