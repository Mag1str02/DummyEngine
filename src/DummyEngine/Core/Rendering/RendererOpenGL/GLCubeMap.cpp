#include "Core/Rendering/RendererOpenGL/GLCubeMap.h"
#include "Core/Rendering/RendererOpenGL/GLUtils.h"
#include "ToolBox/Editors/TextureEditor.h"

namespace DE
{
    GLCubeMap::GLCubeMap(const Ref<TextureData> data)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_MapId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_MapId);

        // TODO: move somewhere else...s
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        if (data->Width() % 4 != 0 || data->Height() % 3 != 0 || data->Width() / 4 != data->Height() / 3)
        {
            std::string error;
            if (data->Width() % 4 != 0)
            {
                error += "(Width % 4 != 0)";
            }
            if (data->Height() % 3 != 0)
            {
                error += "(Height % 3 != 0)";
            }
            if (data->Width() / 4 != data->Height() / 3)
            {
                error += "(Width / 4 != Height / 3)";
            }
            Logger::Warning("loading", "GLCubeMap","Attempt to create cubemap with wrong texture size:" + error);
        }

        for (size_t i = 0; i < 6; ++i)
        {
            auto side = TextureEditor::GetSkyBoxSide(data, CubeSide(i));
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         TextureFormatToGLTextureInternalFormat(side->Format()),
                         side->Width(),
                         side->Height(),
                         0,
                         TextureFormatToGLTextureFormat(side->Format()),
                         GL_UNSIGNED_BYTE,
                         side->Data());
        }
    }
    GLCubeMap::~GLCubeMap() { glDeleteTextures(1, &m_MapId); }

    void GLCubeMap::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_MapId); }
}  // namespace DE