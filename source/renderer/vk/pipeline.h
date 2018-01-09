#pragma once

#include <renderer/vertex.h>
#include <renderer/swapchain.h>
#include <thirdparty/vulkan/vulkan.h>

#include <optional>
#include <vector>

namespace Reweng
{

    struct VulkanPipelineSettings
    {
        VkDevice LogicalDevice;
        VkRenderPass RenderPass;

        VulkanSwapchain::Ptr pSwapchain;

        Vertex::VertexInputAttributeDescriptions VertexInputAttributeDescriptions;
        VkVertexInputBindingDescription          VertexInputBindingDescription;

        VkDescriptorSetLayout DescriptorSetLayout;

        VkShaderModule VertexShader;
        VkShaderModule FragmentShader;
    };

    class VulkanPipeline
    {
    public:
        VulkanPipeline();
        VulkanPipeline(const VulkanPipelineSettings& settings);

        inline VkPipeline GetPipelineHandle()
        {
            return Handle;
        }

        inline VkRenderPass GetRenderPass()
        {
            return RenderPass;
        }

    private:
        std::vector<VkPipelineShaderStageCreateInfo>         GetShaderStageInfo();
        VkPipelineVertexInputStateCreateInfo                 GetVertexInputStateInfo();
        VkPipelineInputAssemblyStateCreateInfo               GetInputAssemblyStateInfo();
        std::optional<VkPipelineTessellationStateCreateInfo> GetTesselaltionStateInfo();
        VkPipelineViewportStateCreateInfo                    GetViewportStateInfo();
        VkPipelineRasterizationStateCreateInfo               GetRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo                 GetMultisampleStateInfo();
        VkPipelineColorBlendStateCreateInfo                  GetColorBlendStateInfo();
        VkPipelineLayoutCreateInfo                           GetPipelineLayoutInfo();
        std::optional<VkPipelineDepthStencilStateCreateInfo> GetDepthStencilStateInfo();
        std::optional<VkPipelineDynamicStateCreateInfo>      GetDynamicStateInfo();
        void CreatePipeline();

    private:
        bool isInitialized;

        VkPipeline       Handle;
        VkPipelineLayout PipelineLayout;

        VkDevice         LogicalDevice;
        VkRenderPass     RenderPass;

        VulkanSwapchain::Ptr pSwapchain;

        Vertex::VertexInputAttributeDescriptions VertexInputAttributeDescriptions;
        VkVertexInputBindingDescription          VertexInputBindingDescription;

        VkDescriptorSetLayout DescriptorSetLayout;

        VkShaderModule VertexShader;
        VkShaderModule FragmentShader;
    };
}