#include "gltexturemanager.h"

#include <common/error.h>

#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_image.h>

namespace Reweng
{
    void GLTextureManager::Upload2DTexture(std::string_view pathToTexture, std::string_view TextureName)
    {

        int width, height;
        int nrChannels;
        unsigned char *data = stbi_load(pathToTexture.data(), &width, &height, &nrChannels, 0);

        GL_CHECK(data != nullptr, "Unable to upload a texture.");

        GLTexture newTextureComponent = {};
        glGenTextures(1, &newTextureComponent.TextureID);
        glBindTexture(GL_TEXTURE_2D, newTextureComponent.TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        GLuint newTextureArrayID = Textures.size();
        Textures.push_back(newTextureComponent);
        TextureMap[TextureName] = newTextureArrayID;
    }
}