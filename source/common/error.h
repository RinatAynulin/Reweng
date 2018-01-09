#pragma once
#include <exception>
#include <string>

enum VkResult;
#define DBG_LINE std::string("(") + __FILE__ + ": " + __FUNCTION__ + " at " + std::to_string(__LINE__) + ")"
typedef std::string dbg_line;
/*
#define VK_VALIDATE(result) \
    do\
    {\
      if(result != VK_SUCCESS)\
        throw Common::Error(DBG_LINE, Common::Critical).AddVulkanError(result); \
    } while(0)

#define VK_CHECK(condition, errMsg) \
    do\
    {\
        if(condition == VK_FALSE)\
            throw Common::Error(DBG_LINE, errMsg, Common::Critical);\
    } while(0)
*/
#define GL_CHECK(condition, errMsg) \
    do\
    {\
        if(condition == false)\
            throw Common::Error(DBG_LINE, errMsg, Common::Critical);\
    } while(0)

namespace Common
{
    enum ErrorType
    {
        Warning = 0,
        Critical
    };

    class Error : public std::exception
    {
    public:
        Error(const dbg_line& line , const std::string& msg, ErrorType type);
        Error(const dbg_line& line, const char* const msg, ErrorType type);
        explicit Error(const dbg_line& line, ErrorType type);

        char const* what() const override;
        ErrorType type() const;

        Error& AddVulkanError(VkResult error);
    private:
        std::string Msg;
        const ErrorType Type;
    };
}