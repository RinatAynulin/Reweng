#include "bufferallocation.h"

#include <common/error.h>

namespace Reweng
{
    BufferAllocator::BufferAllocator()
        : isInitialized(false)
    {
    }

    void BufferAllocator::Init(const BufferAllocatorSettings& settings)
    {
        LogicalDevice = settings.LogicalDevice;
        PhysicalDeviceMemoryProperties = settings.MemoryProperties;
    }

    BufferInformation BufferAllocator::Allocate(VkBufferUsageFlags bufferType, size_t allocationSize)
    {
        VK_CHECK(isInitialized, "attempt to allocate memory for buffer by uninitialized BufferAllocator");
        BufferInformation bufferInformation = {};

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = allocationSize;
        bufferInfo.usage = bufferType;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_VALIDATE(vkCreateBuffer(LogicalDevice, &bufferInfo, nullptr, &bufferInformation.BufferHandle));

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(LogicalDevice, bufferInformation.BufferHandle, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        VkMemoryPropertyFlags requiredMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        memoryAllocateInfo.memoryTypeIndex = GetSuitableMemoryTypeIndex(memoryRequirements.memoryTypeBits, requiredMemoryFlags);

        VK_VALIDATE(vkAllocateMemory(LogicalDevice, &memoryAllocateInfo, nullptr, &bufferInformation.BufferMemory));
        VK_VALIDATE(vkBindBufferMemory(LogicalDevice, bufferInformation.BufferHandle, bufferInformation.BufferMemory, 0));

        return bufferInformation;
    }

    int BufferAllocator::GetSuitableMemoryTypeIndex(const uint32_t memoryTypeBits, const VkMemoryPropertyFlags requiredMemoryFlags)
    {
        for (uint32_t i = 0; i < PhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
        {
            const bool isMemoryTypeSupported = memoryTypeBits & (1 << i);
            if (isMemoryTypeSupported)
            {
                const bool isMemorySuitable = (PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & requiredMemoryFlags) == requiredMemoryFlags;
                if (isMemorySuitable)
                    return i;
            }
        }

        throw Common::Error(DBG_LINE, "GPU doesn't support a required memory.", Common::Critical);
    }
}