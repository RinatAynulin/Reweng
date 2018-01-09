#pragma once

#include <renderer/gpu.h>
#include <renderer/swapchain.h>
#include <renderer/pipelinemanager.h>

#include <thirdparty/glfw3/glfw3.h>
#include <thirdparty/vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <string>
#include <vector>
#include <array>

namespace Reweng
{   
    struct ApplicationInformation
    {
        const char* Name;
        uint32_t Version;

        struct
        {
            uint32_t Width;
            uint32_t Height;
        } Window;

        uint32_t SwapchainMinImageCount = 2;
    };

    struct EngineInformation
    {
        const char* Name = "Reweng";
        uint32_t Version;
        bool isDebugEnabled;
    };

    struct RendererSettings
    {
        ApplicationInformation ApplicationInfo;
        EngineInformation EngineInfo;
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    class Renderer
    {
    public:
        void Init(const RendererSettings& settings);
        void Loop();
    private:
        void EnableInstanceExtensions();
        void EnableDeviceExtensions();
        void EnableValidationLayers();
        void InitRenderer(const RendererSettings& settings);
        void InitGLFW();
        void CheckInstanceExtensionsSupport();
        void CheckInstanceDebugLayerSupport();
        void CreateInstance();
        void CreateDebugCallback();
        void ConstructWindow();
        std::tuple<int/*graphicsIndex*/, int/*presentsIndex*/, bool> GetQueueFamilyIndexes(const std::vector<VkQueueFamilyProperties>& familyProperties
                                                                                          , VkPhysicalDevice physicalDevice);
        void SelectGPU();
        void CheckDeviceExtensionsSupport();
        void CheckDeviceDebugLayerSupport();
        void CreateLogicalDevice();
        
        void CreateSurface();
        void CreateSwapchain();
        void CreatePipelineManager();
  
        void CreateUniformBuffer();
        
        //void CreateDescriptorPool();
        //void CreateDescriptorSet();
        
        //void CreateFramebuffers();
        //void CreateCommandPool();
        //int  GetSuitableMemoryTypeIndex(const uint32_t memoryTypeBits, const VkMemoryPropertyFlags requiredMemoryFlag);
        //void CreateVertexBuffers();
        //void CreateCommandBuffers();
        //void CreateSemaphores();
        //void UpdateUniformBuffer();
        //void DrawFrame();
    private:
        EngineInformation EngineInfo;
        ApplicationInformation ApplicationInfo;

        std::vector<const char*> EnabledInstanceExtensions;
        std::vector<const char*> EnabledDeviceExtensions;
        std::vector<const char*> EnabledDebugLayers;

        VkInstance                 Instance;
        VkDebugReportCallbackEXT   DebugReportCallback;
        GLFWwindow*                WindowHandler;
        VkSurfaceKHR               Surface;
        GPU                        gpu;

        VkDevice                   LogicalDevice;
        VkQueue                    GraphicsQueue;
        VkQueue                    PresentQueue;

        VulkanSwapchain::Ptr       pSwapchain;
        VulkanPipelineManager      PipelineManager;

        std::vector<VkFramebuffer> SwapchainFramebuffers;

        
        VkDescriptorPool DescriptorPool;
        VkDescriptorSet DescriptorSet;
        VkCommandPool CommandPool;
        std::vector<VkCommandBuffer> CommandBuffers;
        VkSemaphore ImageAvailableSemaphore;
        VkSemaphore RenderFinishedSemaphore;

        const std::vector<Vertex> vertices = {
            { { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
            { { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
            { { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
            { { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
        };
        const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        VkBuffer VertexBuffer;
        VkDeviceMemory VertexBufferMemory;

        VkBuffer VertexIndexBuffer;
        VkDeviceMemory VertexIndexBufferMemory;
        
        VkBuffer UniformBuffer;
        VkDeviceMemory UniformBufferMemory;
    };
}