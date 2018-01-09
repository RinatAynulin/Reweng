#pragma once

#include <renderer/gpu.h>
#include <thirdparty/vulkan/vulkan.h>
#include <thirdparty/glfw3/glfw3.h>

#include <vector>
#include <memory>

namespace Reweng
{
    struct SwapchainSettings
    {
        VkDevice     LogicalDevice;
        GPU          Gpu;
        VkSurfaceKHR Surface;
        uint32_t     MinImageCount;
        VkExtent2D   Extent;
    };

    class VulkanSwapchain
    {
    public:
        typedef std::shared_ptr<VulkanSwapchain> Ptr;

        VulkanSwapchain() = default;
        explicit VulkanSwapchain(const SwapchainSettings& settings);

        inline VkSwapchainKHR GetSwapchainHandle()
        {
            return Handle;
        }

        inline VkFormat GetImageFormat()
        {
            return ImageFormat;
        }

        inline std::vector<VkImage> GetImages()
        {
            return SwapchainImages;
        }

        inline std::vector<VkImageView>  GetImageViews()
        {
            return SwapchainImageViews;
        }

        inline VkExtent2D GetExtent()
        {
            return Extent;
        }

    private:
        VkSurfaceFormatKHR         ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR           ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D                 ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void                       CreateSwapchain();
        void                       CreateSwapchainImages();

    private:
        VkDevice                   LogicalDevice;
        GPU                        Gpu;
        uint32_t                   MinImageCount;
        VkExtent2D                 Extent;

        VkSurfaceKHR               Surface;
        VkSwapchainKHR             Handle;
        VkFormat                   ImageFormat;
        std::vector<VkImage>       SwapchainImages;
        std::vector<VkImageView>   SwapchainImageViews;
    };
}