#include "error.h"

//#include "thirdparty/vulkan/vulkan.h"

namespace Common
{
    /*    namespace
    {
        std::string ConvertVkResultToString(VkResult error)
        {
            switch (error)
            {
            case VK_SUCCESS:
                return "VK_SUCCESS: Command successfully completed";

            case VK_NOT_READY:
                return "VK_NOT_READY: A fence or query has not yet completed";

            case VK_TIMEOUT:
                return "VK_TIMEOUT: A wait operation has not completed in the specified time";

            case VK_EVENT_SET:
                return "VK_EVENT_SET: An event is signaled";

            case VK_EVENT_RESET:
                return "VK_EVENT_RESET: An event is unsignaled";

            case VK_INCOMPLETE:
                return "VK_INCOMPLETE: A return array was too small for the result";

            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "VK_ERROR_OUT_OF_HOST_MEMORY: A host memory allocation has failed.";

            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "VK_ERROR_OUT_OF_DEVICE_MEMORY: A device memory allocation has failed.";

            case VK_ERROR_INITIALIZATION_FAILED:
                return "VK_ERROR_INITIALIZATION_FAILED: Initialization of an object could not be completed for implementation - specific reasons.";

            case VK_ERROR_DEVICE_LOST:
                return "VK_ERROR_DEVICE_LOST: The logical or physical device has been lost.See Lost Device";

            case VK_ERROR_MEMORY_MAP_FAILED:
                return "VK_ERROR_MEMORY_MAP_FAILED: Mapping of a memory object has failed.";

            case VK_ERROR_LAYER_NOT_PRESENT:
                return "VK_ERROR_LAYER_NOT_PRESENT: A requested layer is not present or could not be loaded.";

            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "VK_ERROR_EXTENSION_NOT_PRESENT: A requested extension is not supported.";

            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "VK_ERROR_FEATURE_NOT_PRESENT: A requested feature is not supported.";

            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "VK_ERROR_INCOMPATIBLE_DRIVER: The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation - specific reasons.";

            case VK_ERROR_TOO_MANY_OBJECTS:
                return "VK_ERROR_TOO_MANY_OBJECTS: Too many objects of the type have already been created.";

            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "VK_ERROR_FORMAT_NOT_SUPPORTED:A requested format is not supported on this device.";

            case VK_ERROR_FRAGMENTED_POOL:
                return "VK_ERROR_FRAGMENTED_POOL: A pool allocation has failed due to fragmentation of the pool’s memory.\n\
                   This must only be returned if no attempt to allocate host or device memory was made to accomodate the new allocation.";

            default:
                return "Unknown Vulkan error code";
            }
        }
    }
    */
    Error::Error(const dbg_line& line, const std::string& msg, ErrorType type)
        : Msg(line + std::string(msg))
        , Type(type)
    {
    }

    Error::Error(const dbg_line& line, const char* const msg, ErrorType type)
        : Msg(line + std::string(msg))
        , Type(type)
    {
    }

    Error::Error(const dbg_line& line, ErrorType type)
        : Msg(line)
        , Type(type)
    {
    }

    char const* Error::what() const
    {
        return Msg.c_str();
    }

    ErrorType Error::type() const
    {
        return Type;
    }

    

    Error& Error::AddVulkanError(VkResult error)
    {
        //Msg += std::string("Vulkan error:") + std::to_string(error) + " " + ConvertVkResultToString(error) + "\n";

        return *this;
    }
}