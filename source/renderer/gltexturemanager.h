#pragma once

#include <common/error.h>
#include <renderer/gltexture.h>
#include <renderer/gltexturemanager.h>

#include <thirdparty/glad/glad.h>

#include <vector>
#include <string_view>
#include <unordered_map>

namespace Reweng
{
    typedef std::string_view TextureName;
    typedef GLuint TextureID;

    class GLTextureManager
    {
    public:
        void Upload2DTexture(std::string_view pathToTexture, std::string_view TextureName);

        GLTexture GetTexture(std::string_view TextureName) const
        {
            const auto it = TextureMap.find(TextureName);
            GL_CHECK(it != TextureMap.end(), "Unable to find a requested texture.");

            return Textures[it->second];
        }

        GLTexture GetTexture(TextureID id) const
        {
            return Textures[id];
        }

    private:
        std::vector<GLTexture> Textures;
        std::unordered_map<TextureName, TextureID> TextureMap;
    };
}