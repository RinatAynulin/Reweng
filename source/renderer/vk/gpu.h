#pragma once

#include <thirdparty/vulkan/vulkan.h>

#include <vector>

namespace Reweng
{
    struct GPU
    {
        VkPhysicalDevice                 PhysicalDevice;
        VkPhysicalDeviceProperties       Properties;
        VkPhysicalDeviceFeatures         Features;
        VkPhysicalDeviceMemoryProperties MemoryProperties;
        
        
        std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
        int GraphicsFamilyIndex;
        int PresentFamilyIndex;

        VkSurfaceCapabilitiesKHR SurfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> SurfaceFormats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

   class GPUCollector
   {
   public:
       GPUCollector(VkInstance instance, VkSurfaceKHR surface);
       std::vector<GPU> CollectGPUs();
   
   private:
       VkPhysicalDeviceProperties           GetProperties(VkPhysicalDevice physicalDevice);
       VkPhysicalDeviceFeatures             GetFeatures(VkPhysicalDevice physicalDevice);
       VkPhysicalDeviceLimits               GetLimits(VkPhysicalDevice physicalDevice);
       VkPhysicalDeviceMemoryProperties     GetMemoryProperties(VkPhysicalDevice physicalDevice);
       std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice physicalDevice);
       VkSurfaceCapabilitiesKHR             GetSurfaceCapabilitiess(VkPhysicalDevice physicalDevice);
       std::vector<VkSurfaceFormatKHR>      GetSurfaceFormats(VkPhysicalDevice physicalDevice);
       std::vector<VkPresentModeKHR>        GetPresentModes(VkPhysicalDevice physicalDevice);
   private:
       VkInstance Instance;
       VkSurfaceKHR Surface;
   };
}