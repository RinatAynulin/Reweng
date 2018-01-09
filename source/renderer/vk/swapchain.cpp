#include "swapchain.h"

#include <common/error.h>
#include <algorithm>

namespace Reweng
{
    VulkanSwapchain::VulkanSwapchain(const SwapchainSettings& settings)
        : LogicalDevice(settings.LogicalDevice)
        , Gpu(settings.Gpu)
        , MinImageCount(settings.MinImageCount)
        , Extent(ChooseSwapchainExtent(settings.Gpu.SurfaceCapabilities))
    {
        CreateSwapchain();
        CreateSwapchainImages();
    }


    VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
            return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapchain::ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        VkBool32 isVkPresentModeImmediateSupported = false;

        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return VK_PRESENT_MODE_MAILBOX_KHR;
            else
                isVkPresentModeImmediateSupported = availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        return isVkPresentModeImmediateSupported ? VK_PRESENT_MODE_IMMEDIATE_KHR : VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
        {
            return capabilities.currentExtent;
        }
        else 
        {
            VkExtent2D actualExtent = { Extent.width, Extent.height };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    void VulkanSwapchain::CreateSwapchain()
    {
        const VkSurfaceFormatKHR suitableSurfaceFormat = ChooseSurfaceFormat(Gpu.SurfaceFormats);
        ImageFormat = suitableSurfaceFormat.format;

        VkSwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainInfo.surface = Surface;
        swapchainInfo.minImageCount = MinImageCount;
        swapchainInfo.imageFormat = ImageFormat;
        swapchainInfo.imageColorSpace = suitableSurfaceFormat.colorSpace;
        swapchainInfo.imageExtent.height = Extent.height;
        swapchainInfo.imageExtent.width = Extent.width;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const uint32_t indices[] = { (uint32_t)Gpu.GraphicsFamilyIndex, (uint32_t)Gpu.PresentFamilyIndex };
        if (indices[0] == indices[1])
        {
            swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainInfo.queueFamilyIndexCount = 0;
            swapchainInfo.pQueueFamilyIndices = nullptr;
        }
        else
        {
            swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;;
            swapchainInfo.queueFamilyIndexCount = 2;
            swapchainInfo.pQueueFamilyIndices = indices;
        }

        swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = ChooseSurfacePresentMode(Gpu.PresentModes);
        swapchainInfo.clipped = VK_TRUE;
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_VALIDATE(vkCreateSwapchainKHR(LogicalDevice, &swapchainInfo, nullptr, &Handle));
    }

    void VulkanSwapchain::CreateSwapchainImages()
    {
        uint32_t swapchainImageCount = 0;
        VK_VALIDATE(vkGetSwapchainImagesKHR(LogicalDevice, Handle, &swapchainImageCount, nullptr));
        VK_CHECK(swapchainImageCount > 0, "Swap chain has no images.");

        SwapchainImages.resize(swapchainImageCount);
        VK_VALIDATE(vkGetSwapchainImagesKHR(LogicalDevice, Handle, &swapchainImageCount, SwapchainImages.data()));

        SwapchainImageViews.resize(swapchainImageCount);
        for (uint32_t i = 0; i < swapchainImageCount; ++i)
        {
            VkImageViewCreateInfo imageViewInfo = {};
            imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewInfo.image = SwapchainImages[i];
            imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewInfo.format = ImageFormat;
            imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewInfo.subresourceRange.baseMipLevel = 0;
            imageViewInfo.subresourceRange.levelCount = 1;
            imageViewInfo.subresourceRange.baseArrayLayer = 0;
            imageViewInfo.subresourceRange.layerCount = 1;

            VK_VALIDATE(vkCreateImageView(LogicalDevice, &imageViewInfo, nullptr, &SwapchainImageViews[i]));
        }
    }
}