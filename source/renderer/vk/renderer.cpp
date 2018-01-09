#include "renderer.h"

#include <common/error.h>
#include <renderer/shaderloader.h>

#include <algorithm>
#include <utility>
#include <string.h>
#include <tuple>
#include <iostream>

namespace Reweng
{
    namespace
    {
        class IsExtensionSupported
        {
        public:
            IsExtensionSupported(const char* extensionName)
                : ExtensionName(extensionName)
            {
            }

            inline bool operator()(const VkExtensionProperties extension)
            {
                return strcmp(ExtensionName, extension.extensionName) == 0;
            }

            inline bool operator()(const char* extension)
            {
                return strcmp(ExtensionName, extension) == 0;
            }
        private:
            const char* ExtensionName;
        };

        class IsDebugLayerSupported
        {
        public:
            IsDebugLayerSupported(const char* layerName)
                :LayerName(layerName)
            {
            }

            bool operator()(const VkLayerProperties& layerProperties)
            {
                return strcmp(LayerName, layerProperties.layerName);
            }

        private:
            const char* LayerName;
        };
    }

    void Renderer::EnableInstanceExtensions()
    {
        uint32_t extensionsCount = 0;
        const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionsCount);

        if (extensionsCount == 0)
            return;

        for (uint32_t i = 0; i < extensionsCount; ++i)
            EnabledInstanceExtensions.push_back(extensionNames[i]);

        if (EngineInfo.isDebugEnabled)
            EnabledInstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    void Renderer::EnableDeviceExtensions()
    {
        EnabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    void Renderer::EnableValidationLayers()
    {
        if (EngineInfo.isDebugEnabled)
            EnabledDebugLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    }

    void Renderer::Init(const RendererSettings& settings)
    {
        InitRenderer(settings);
        InitGLFW();
        EnableInstanceExtensions();
        EnableDeviceExtensions();
        EnableValidationLayers();
        CreateInstance();
        ConstructWindow();
        CreateSurface();
        SelectGPU();
        CreateLogicalDevice();
        CreateSwapchain();
        /*CreateSwapchainImages();
        CreateRenderPass();
        CreateUniformBuffer();
        CreateDescriptorSetLayouts();
        CreateDescriptorPool();
        CreateDescriptorSet();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommandPool();
        CreateVertexBuffers();
        CreateCommandBuffers();
        CreateSemaphores();*/
    }

    void Renderer::Loop()
    {
        while (glfwWindowShouldClose(WindowHandler) == false)
        {
            glfwPollEvents();
            //UpdateUniformBuffer();
            //DrawFrame();
        }
    }

    void Renderer::InitRenderer(const RendererSettings& settings)
    {
        EngineInfo = settings.EngineInfo;
        ApplicationInfo = settings.ApplicationInfo;

        EnableInstanceExtensions();
    }

    void Renderer::InitGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    void Renderer::CheckInstanceExtensionsSupport()
    {
        uint32_t supportedExtensionsCount = 0;
        VK_VALIDATE(vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, nullptr));
        VK_CHECK(supportedExtensionsCount > 0, "System doesn't support the required extensions.");
        
        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionsCount);
        VK_VALIDATE(vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, supportedExtensions.data()));

        for (const auto& enabledExtension : EnabledInstanceExtensions)
        {
            const auto it = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), IsExtensionSupported(enabledExtension));
            if (it == supportedExtensions.end())
                throw Common::Error(DBG_LINE, "System doesn't support on of the required extensions", Common::Critical);
        }
    }

    void Renderer::CheckInstanceDebugLayerSupport()
    {
        uint32_t supportedLayerCount = 0;
        VK_VALIDATE(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr));
        VK_CHECK(supportedLayerCount > 0, "System doesn't support the required debug layers.");

        std::vector<VkLayerProperties> supportedLayers(supportedLayerCount);
        VK_VALIDATE(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()));

        for (const auto& enabledDebugLayer : EnabledDebugLayers)
        {
            const auto it = std::find_if(supportedLayers.begin(), supportedLayers.end(), IsDebugLayerSupported(enabledDebugLayer));
            if (it == supportedLayers.end())
                throw Common::Error(DBG_LINE, "System doesn't support on of the required debug layers.", Common::Critical);
        }
    }

    void Renderer::CreateInstance()
    {
        if (EnabledInstanceExtensions.empty() == false)
            CheckInstanceExtensionsSupport();
        
        if (EngineInfo.isDebugEnabled)
            CheckInstanceDebugLayerSupport();

        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pEngineName = EngineInfo.Name;
        applicationInfo.engineVersion = EngineInfo.Version;
        applicationInfo.pApplicationName = ApplicationInfo.Name;
        applicationInfo.applicationVersion = ApplicationInfo.Version;
        applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION);

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &applicationInfo;
        instanceInfo.enabledExtensionCount = (uint32_t)EnabledInstanceExtensions.size();
        instanceInfo.ppEnabledExtensionNames = EnabledInstanceExtensions.data();
        instanceInfo.enabledLayerCount = (uint32_t)EnabledDebugLayers.size();
        instanceInfo.ppEnabledLayerNames = EnabledDebugLayers.data();

        VK_VALIDATE(vkCreateInstance(&instanceInfo, nullptr, &Instance));
        
        if (EngineInfo.isDebugEnabled)
            CreateDebugCallback();
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugLayerCallback(VkDebugReportFlagsEXT      flags,
                                                      VkDebugReportObjectTypeEXT objType,
                                                      uint64_t                   obj,
                                                      size_t                     location,
                                                      int32_t                    code,
                                                      const char*                layerPrefix,
                                                      const char*                msg,
                                                      void*                      userData)
    {
        std::cerr << "validation layer: " << layerPrefix << msg << std::endl;

        return VK_FALSE;
    }

    void Renderer::CreateDebugCallback()
    {
        VkDebugReportCallbackCreateInfoEXT debugCallbackInfo = {};
        debugCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugCallbackInfo.pfnCallback = DebugLayerCallback;
        debugCallbackInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT
                                | VK_DEBUG_REPORT_WARNING_BIT_EXT
                                | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
                                | VK_DEBUG_REPORT_ERROR_BIT_EXT;

        PFN_vkCreateDebugReportCallbackEXT extFunction = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(Instance, "vkCreateDebugReportCallbackEXT"));
        if (extFunction == nullptr)
            throw Common::Error(DBG_LINE, Common::Critical).AddVulkanError(VK_ERROR_EXTENSION_NOT_PRESENT);

       VK_VALIDATE(extFunction(Instance, &debugCallbackInfo, nullptr, &DebugReportCallback));
    }

    void Renderer::ConstructWindow()
    {
        WindowHandler = glfwCreateWindow(ApplicationInfo.Window.Width, ApplicationInfo.Window.Height, ApplicationInfo.Name, nullptr, nullptr);
    }

    void Renderer::CreateSurface()
    {
        VK_VALIDATE(glfwCreateWindowSurface(Instance, WindowHandler, nullptr, &Surface));
    }

    std::tuple<int/*graphicsIndex*/, int/*presentsIndex*/, bool> Renderer::GetQueueFamilyIndexes(const std::vector<VkQueueFamilyProperties>& familyProperties
                                                                                                , VkPhysicalDevice physicalDevice)
    {
        int graphicsFamilyIndex = -1, presentsFamilyIndex = -1;
        for (int idx = 0; idx < familyProperties.size(); ++idx)
        {
            if (familyProperties[idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsFamilyIndex = idx;

            VkBool32 isSurfaceSupported = VK_FALSE;
            VK_VALIDATE(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, idx, Surface, &isSurfaceSupported));
            if (isSurfaceSupported)
                presentsFamilyIndex = idx;

            if (graphicsFamilyIndex != -1 && presentsFamilyIndex != -1)
                return { graphicsFamilyIndex, presentsFamilyIndex, true };
        }

        return { -1, -1, false };
    }

    void Renderer::SelectGPU()
    {
        GPUCollector gpuCollector(Instance, Surface);
        std::vector<GPU> gpus = gpuCollector.CollectGPUs();

        for (const auto& gpu : gpus)
        {
            const bool isDiscrete = gpu.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
            
            const bool HasSwapChainNicelySupport = (gpu.SurfaceFormats.size() != 0) && (gpu.PresentModes.size() != 0);

            auto[graphicsFamilyIndex, presentFamilyIndex, areFamiliesFound] = GetQueueFamilyIndexes(gpu.QueueFamilyProperties, gpu.PhysicalDevice);
            
            if (isDiscrete && areFamiliesFound && HasSwapChainNicelySupport)
            {
                this->gpu = gpu;
                this->gpu.GraphicsFamilyIndex = graphicsFamilyIndex;
                this->gpu.PresentFamilyIndex  = presentFamilyIndex;

                return;
            }
        }

        throw Common::Error(DBG_LINE, "System doesn't have a required video card.", Common::Critical);
    }

    void Renderer::CheckDeviceExtensionsSupport()
    {
        uint32_t supportedExtensionsCount = 0;
        VK_VALIDATE(vkEnumerateDeviceExtensionProperties(gpu.PhysicalDevice, nullptr, &supportedExtensionsCount, nullptr));
        VK_CHECK(supportedExtensionsCount > 0, "System doesn't support a required extensions for logical device.");

        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionsCount);
        VK_VALIDATE(vkEnumerateDeviceExtensionProperties(gpu.PhysicalDevice, nullptr, &supportedExtensionsCount, supportedExtensions.data()));

        for (const auto& enabledExtension : EnabledDeviceExtensions)
        {
            const auto it = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), IsExtensionSupported(enabledExtension));
            if (it == supportedExtensions.end())
                throw Common::Error(DBG_LINE, "System doesn't support one of the required extensions for logical device.", Common::Critical);
        }
    }

    void Renderer::CheckDeviceDebugLayerSupport()
    {
        uint32_t supportedLayerCount = 0;
        VK_VALIDATE(vkEnumerateDeviceLayerProperties(gpu.PhysicalDevice, &supportedLayerCount, nullptr));
        VK_CHECK(supportedLayerCount > 0, "System doesn't support the required debug layers.");

        std::vector<VkLayerProperties> supportedLayers(supportedLayerCount);
        VK_VALIDATE(vkEnumerateDeviceLayerProperties(gpu.PhysicalDevice, &supportedLayerCount, supportedLayers.data()));

        for (const auto& enabledDebugLayer : EnabledDebugLayers)
        {
            const auto it = std::find_if(supportedLayers.begin(), supportedLayers.end(), IsDebugLayerSupported(enabledDebugLayer));
            if (it == supportedLayers.end())
                throw Common::Error(DBG_LINE, "System doesn't support one of the required debug layers for logical device.", Common::Critical);
        }
    }

    void Renderer::CreateLogicalDevice()
    {
        if (EnabledDebugLayers.empty() == false)
            CheckDeviceDebugLayerSupport();
        
        if (EnabledDeviceExtensions.empty() == false)
            CheckDeviceExtensionsSupport();

        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        const float priority = 1.0;
        for (const auto& familyIndex : { gpu.GraphicsFamilyIndex, gpu.PresentFamilyIndex })
        {
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueCount = 1;
            queueInfo.queueFamilyIndex = familyIndex;
            queueInfo.pQueuePriorities = &priority;
            
            queueInfos.push_back(queueInfo);
        }

        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.enabledExtensionCount = (uint32_t)EnabledDeviceExtensions.size();
        deviceInfo.ppEnabledExtensionNames = EnabledDeviceExtensions.data();
        deviceInfo.enabledLayerCount = (uint32_t)EnabledDebugLayers.size();
        deviceInfo.ppEnabledLayerNames = EnabledDebugLayers.data();
        deviceInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();
        deviceInfo.pQueueCreateInfos = queueInfos.data();
        deviceInfo.pEnabledFeatures = &gpu.Features;

        VK_VALIDATE(vkCreateDevice(gpu.PhysicalDevice, &deviceInfo, nullptr, &LogicalDevice));
        vkGetDeviceQueue(LogicalDevice, gpu.GraphicsFamilyIndex, 0, &GraphicsQueue);
        vkGetDeviceQueue(LogicalDevice, gpu.PresentFamilyIndex, 0, &PresentQueue);
    }

    void Renderer::CreateSwapchain()
    {
        SwapchainSettings settings = {};
        settings.LogicalDevice = LogicalDevice;
        settings.Gpu = gpu;
        settings.Surface = Surface;
        settings.MinImageCount = ApplicationInfo.SwapchainMinImageCount;
        settings.Extent.height = ApplicationInfo.Window.Height;
        settings.Extent.width = ApplicationInfo.Window.Width;

        pSwapchain = VulkanSwapchain::Ptr(new VulkanSwapchain(settings));
    }
    
    void Renderer::CreatePipelineManager()
    {
        VulkanPipelineManagerSettings settings = {};
        settings.LogicalDevice = LogicalDevice;
        settings.pSwapchain = pSwapchain;
        PipelineManager = VulkanPipelineManager(settings);
    }

    /*void Renderer::CreateUniformBuffer()
    {
        VkMemoryPropertyFlags requiredMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkBufferCreateInfo uboInfo = {};
        uboInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        uboInfo.size = sizeof(UniformBufferObject);
        uboInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        uboInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_VALIDATE(vkCreateBuffer(LogicalDevice, &uboInfo, nullptr, &UniformBuffer));
        VkMemoryRequirements uboMemoryRequirements;
        vkGetBufferMemoryRequirements(LogicalDevice, UniformBuffer, &uboMemoryRequirements);

        VkMemoryAllocateInfo uboMemoryAllocateInfo = {};
        uboMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        uboMemoryAllocateInfo.allocationSize = uboMemoryRequirements.size;
        uboMemoryAllocateInfo.memoryTypeIndex = GetSuitableMemoryTypeIndex(uboMemoryRequirements.memoryTypeBits, requiredMemoryFlags);

        VK_VALIDATE(vkAllocateMemory(LogicalDevice, &uboMemoryAllocateInfo, nullptr, &UniformBufferMemory));
        VK_VALIDATE(vkBindBufferMemory(LogicalDevice, UniformBuffer, UniformBufferMemory, 0));
    }
    
    void Renderer::CreateFramebuffers()
    {
        SwapchainFramebuffers.resize(SwapchainImageViews.size());

        for (int i = 0; i <  SwapchainImageViews.size(); ++i)
        {
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = &SwapchainImageViews[i];
            framebufferInfo.width = ApplicationInfo.Window.Width;
            framebufferInfo.height = ApplicationInfo.Window.Height;
            framebufferInfo.layers = 1;

            VK_VALIDATE(vkCreateFramebuffer(LogicalDevice, &framebufferInfo, nullptr, &SwapchainFramebuffers[i]));
        }
    }

    void Renderer::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolInfo = {};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.queueFamilyIndex = gpu.GraphicsFamilyIndex;

        VK_VALIDATE(vkCreateCommandPool(LogicalDevice, &commandPoolInfo, nullptr, &CommandPool));
    }

    int Renderer::GetSuitableMemoryTypeIndex(const uint32_t memoryTypeBits, const VkMemoryPropertyFlags requiredMemoryFlags)
    {
        for (uint32_t i = 0; i < gpu.MemoryProperties.memoryTypeCount; ++i)
        {
            const bool isMemoryTypeSupported = memoryTypeBits & (1 << i);
            if (isMemoryTypeSupported)
            {
                const bool isMemorySuitable = (gpu.MemoryProperties.memoryTypes[i].propertyFlags & requiredMemoryFlags) == requiredMemoryFlags;
                if (isMemorySuitable)
                    return i;
            }
        }

        throw Common::Error(DBG_LINE, "GPU doesn't support a required memory.", Common::Critical);
    }

    void Renderer::CreateVertexBuffers()
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_VALIDATE(vkCreateBuffer(LogicalDevice, &bufferInfo, nullptr, &VertexBuffer));

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(LogicalDevice, VertexBuffer, &memoryRequirements);
        
        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        VkMemoryPropertyFlags requiredMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        memoryAllocateInfo.memoryTypeIndex = GetSuitableMemoryTypeIndex(memoryRequirements.memoryTypeBits, requiredMemoryFlags);

        VK_VALIDATE(vkAllocateMemory(LogicalDevice, &memoryAllocateInfo, nullptr, &VertexBufferMemory));
        VK_VALIDATE(vkBindBufferMemory(LogicalDevice, VertexBuffer, VertexBufferMemory, 0));

        void* data = nullptr;
        VK_VALIDATE(vkMapMemory(LogicalDevice, VertexBufferMemory, 0, bufferInfo.size, 0, &data));
        memcpy(data, vertices.data(), bufferInfo.size);
        vkUnmapMemory(LogicalDevice, VertexBufferMemory);
        data = nullptr;
        

        VkBufferCreateInfo indexBufferInfo = {};
        indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        indexBufferInfo.size = sizeof(indices[0]) * indices.size();
        indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_VALIDATE(vkCreateBuffer(LogicalDevice, &indexBufferInfo, nullptr, &VertexIndexBuffer));
        VkMemoryRequirements indexMemoryRequirements;
        vkGetBufferMemoryRequirements(LogicalDevice, VertexIndexBuffer, &indexMemoryRequirements);

        VkMemoryAllocateInfo indexMemoryAllocateInfo = {};
        indexMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        indexMemoryAllocateInfo.allocationSize = indexMemoryRequirements.size;
        indexMemoryAllocateInfo.memoryTypeIndex = GetSuitableMemoryTypeIndex(indexMemoryRequirements.memoryTypeBits, requiredMemoryFlags);

        VK_VALIDATE(vkAllocateMemory(LogicalDevice, &indexMemoryAllocateInfo, nullptr, &VertexIndexBufferMemory));
        VK_VALIDATE(vkBindBufferMemory(LogicalDevice, VertexIndexBuffer, VertexIndexBufferMemory, 0));

        VK_VALIDATE(vkMapMemory(LogicalDevice, VertexIndexBufferMemory, 0, indexBufferInfo.size, 0, &data));
        memcpy(data, indices.data(), indexBufferInfo.size);
        vkUnmapMemory(LogicalDevice, VertexIndexBufferMemory);
        /////////////////////////UNIFORM BUFFER///////////////////////////////////////////
    }

    void Renderer::CreateDescriptorPool()
    {
        VkDescriptorPoolSize descriptorPoolSize = {};
        descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorPoolSize.descriptorCount = 1;
        
        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount = 1;
        descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
        descriptorPoolInfo.maxSets = 1;

        VK_VALIDATE(vkCreateDescriptorPool(LogicalDevice, &descriptorPoolInfo, nullptr, &DescriptorPool));
    }

    void Renderer::CreateDescriptorSet()
    {
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.descriptorPool = DescriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = 1;
        descriptorSetAllocateInfo.pSetLayouts = &DescriptorSetLayout;

        VK_VALIDATE(vkAllocateDescriptorSets(LogicalDevice, &descriptorSetAllocateInfo, &DescriptorSet));

        VkDescriptorBufferInfo descriptorBufferInfo = {};
        descriptorBufferInfo.buffer = UniformBuffer;
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet = DescriptorSet;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(LogicalDevice, 1, &writeDescriptorSet, 0, nullptr);

    }

    void Renderer::CreateCommandBuffers()
    {
        CommandBuffers.resize(SwapchainFramebuffers.size());

        VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
        cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufferAllocateInfo.commandPool = CommandPool;
        cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdBufferAllocateInfo.commandBufferCount = (uint32_t)CommandBuffers.size();

        VK_VALIDATE(vkAllocateCommandBuffers(LogicalDevice, &cmdBufferAllocateInfo, CommandBuffers.data()));
        
        for (int i = 0; i < CommandBuffers.size(); ++i)
        {
            VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
            cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            cmdBufferBeginInfo.pInheritanceInfo = nullptr;

            VK_VALIDATE(vkBeginCommandBuffer(CommandBuffers[i], &cmdBufferBeginInfo));

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = RenderPass;
            renderPassBeginInfo.framebuffer = SwapchainFramebuffers[i];
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = { ApplicationInfo.Window.Width, ApplicationInfo.Window.Height };
            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
            VkDeviceSize offset = 0;
            vkCmdBindDescriptorSets(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
            vkCmdBindVertexBuffers(CommandBuffers[i], 0, 1, &VertexBuffer, &offset);
            vkCmdBindIndexBuffer(CommandBuffers[i], VertexIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(CommandBuffers[i], (uint32_t)indices.size(), 1, 0, 0, 0);
            vkCmdEndRenderPass(CommandBuffers[i]);

            VK_VALIDATE(vkEndCommandBuffer(CommandBuffers[i]));
        }
    }

    void Renderer::CreateSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VK_VALIDATE(vkCreateSemaphore(LogicalDevice, &semaphoreInfo, nullptr, &ImageAvailableSemaphore));
        VK_VALIDATE(vkCreateSemaphore(LogicalDevice, &semaphoreInfo, nullptr, &RenderFinishedSemaphore));
    }

    void Renderer::UpdateUniformBuffer()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), ApplicationInfo.Window.Width / (float)ApplicationInfo.Window.Height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void* data;
        VK_VALIDATE(vkMapMemory(LogicalDevice, UniformBufferMemory, 0, sizeof(ubo), 0, &data));
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(LogicalDevice, UniformBufferMemory);
    }

    void Renderer::DrawFrame()
    {
        uint32_t imageIndex = -1;
        VK_VALIDATE(vkAcquireNextImageKHR(LogicalDevice, Swapchain, std::numeric_limits<uint64_t>::max(), ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &ImageAvailableSemaphore;
        VkPipelineStageFlags pipelineStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.pWaitDstStageMask = pipelineStageFlags;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &RenderFinishedSemaphore;

        VK_VALIDATE(vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &RenderFinishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &pSwapchain->GetSwapchainHandle();
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        VK_VALIDATE(vkQueuePresentKHR(PresentQueue, &presentInfo));
        VK_VALIDATE(vkQueueWaitIdle(PresentQueue));
    }*/
}