#pragma once

#include <thirdparty/vulkan/vulkan.h>

namespace Reweng
{
    struct BufferInformation
    {
        VkBuffer BufferHandle;
        VkDeviceMemory BufferMemory;
    };

    struct BufferAllocatorSettings
    {
        VkDevice LogicalDevice;
        VkPhysicalDeviceMemoryProperties MemoryProperties;
    };

    class BufferAllocator
    {
    public:
        BufferAllocator();
        void Init(const BufferAllocatorSettings& settings);
        BufferInformation Allocate(VkBufferUsageFlags bufferType, size_t allocationSize);
    private:
        int GetSuitableMemoryTypeIndex(const uint32_t memoryTypeBits, const VkMemoryPropertyFlags requiredMemoryFlags);
    private:
        bool isInitialized;
        VkDevice LogicalDevice;
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
    };
}