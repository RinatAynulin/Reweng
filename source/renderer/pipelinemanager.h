#pragma once

#include <renderer/glshaderprogram.h>

#include <thirdparty/glad/glad.h>

#include <array>

namespace Reweng
{
    enum PipelineType
    {
        GRAPHICS_GENERAL = 0,
        PIPELINES_COUNT = (GRAPHICS_GENERAL + 1)
    };

    class GLPipelineManager
    {
    public:
        GLPipelineManager() = default;

        void Init();

        inline GLShaderProgram GetShaderProgram(PipelineType type)
        {
            return Pipelines[type];
        }

        inline void UseShaderProgram(PipelineType type)
        {
            Pipelines[type].Use();
        }

    private:
        void ConstructGeneralPipeline();

    private:
        std::array<GLShaderProgram, PIPELINES_COUNT> Pipelines;
    };
}