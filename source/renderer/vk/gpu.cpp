#include "gpu.h"

#include <common/error.h>

namespace Reweng
{
    GPUCollector::GPUCollector(VkInstance instance, VkSurfaceKHR surface)
        : Instance(instance)
        , Surface(surface)
    {
    }

    std::vector<GPU> GPUCollector::CollectGPUs()
    {
        uint32_t physicalDeviceCount = 0;
        std::vector<VkPhysicalDevice> physicalDevices;
        
        VK_VALIDATE(vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, nullptr));
        if (physicalDeviceCount == 0)
            return std::vector<GPU>{};

        physicalDevices.resize(physicalDeviceCount);
        VK_VALIDATE(vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, physicalDevices.data()));

        std::vector<GPU> gpus(physicalDeviceCount);
        for (const auto physicalDevice : physicalDevices)
        {
            GPU gpu = {};
            gpu.PhysicalDevice = physicalDevice;
            gpu.Properties = GetProperties(gpu.PhysicalDevice);
            gpu.Features = GetFeatures(gpu.PhysicalDevice);
            gpu.MemoryProperties = GetMemoryProperties(gpu.PhysicalDevice);
            gpu.QueueFamilyProperties = GetQueueFamilyProperties(gpu.PhysicalDevice);
            gpu.GraphicsFamilyIndex = -1;
            gpu.PresentFamilyIndex = -1;
            gpu.SurfaceCapabilities = GetSurfaceCapabilitiess(physicalDevice);
            gpu.SurfaceFormats = GetSurfaceFormats(physicalDevice);
            gpu.PresentModes = GetPresentModes(physicalDevice);

            gpus.push_back(gpu);
        }

        return gpus;
    }

    VkPhysicalDeviceProperties GPUCollector::GetProperties(VkPhysicalDevice physicalDevice)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        return properties;
    }
    VkPhysicalDeviceFeatures GPUCollector::GetFeatures(VkPhysicalDevice physicalDevice)
    {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);
        
        return features;
    }

    VkPhysicalDeviceMemoryProperties GPUCollector::GetMemoryProperties(VkPhysicalDevice physicalDevice)
    {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        return memoryProperties;
    }

    std::vector<VkQueueFamilyProperties> GPUCollector::GetQueueFamilyProperties(VkPhysicalDevice physicalDevice)
    {
        uint32_t propertiesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, nullptr);
        std::vector<VkQueueFamilyProperties> properties(propertiesCount);
        
        if (propertiesCount == 0)
            return properties;

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, properties.data());
        return properties;
    }

    VkSurfaceCapabilitiesKHR GPUCollector::GetSurfaceCapabilitiess(VkPhysicalDevice physicalDevice)
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VK_VALIDATE(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, Surface, &surfaceCapabilities));
        
        return surfaceCapabilities;
    }

    std::vector<VkSurfaceFormatKHR> GPUCollector::GetSurfaceFormats(VkPhysicalDevice physicalDevice)
    {
        uint32_t surfaceFormatCount = 0;
        VK_VALIDATE(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Surface, &surfaceFormatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
        VK_VALIDATE(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Surface, &surfaceFormatCount, surfaceFormats.data()));

        return surfaceFormats;
    }

    std::vector<VkPresentModeKHR> GPUCollector::GetPresentModes(VkPhysicalDevice physicalDevice)
    {
        uint32_t presentModeCount = 0;
        VK_VALIDATE(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, Surface, &presentModeCount, nullptr));
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VK_VALIDATE(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, Surface, &presentModeCount, presentModes.data()));

        return presentModes;
    }
}