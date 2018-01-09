#include "glshadermanager.h"

#include <common/error.h>

namespace Reweng
{
    void GLShaderManager::UploadShader(const GLShaderCreateInfo& infos, std::string_view shaderName)
    {
        GLShader newShader(infos);
        ShaderID newShaderID = Shaders.size();
        Shaders.push_back(std::move(newShader));
        
        GL_CHECK( static_cast<GLuint>(newShader) == 0, "blyat" );
        ShaderMap[shaderName] = newShaderID;
    }
}