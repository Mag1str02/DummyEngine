#include "ToolBox/Manipulators/TextureEditor.h"

namespace DE
{
    TextureData TextureEditor::GetTexturePart(const TextureData& data, uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos)
    {
        TextureData res;
        res.SetData(data.Data(), width, height, data.Format());
        uint32_t pixel_size = data.Channels();
        uint8_t* new_image = res.Data();
        const uint8_t* old_image = data.Data();
        for (size_t i = 0; i < height; ++i)
        {
            std::memcpy(new_image + (i * width * pixel_size), old_image + (data.Width() * (i + y_pos) + x_pos) * pixel_size, width * pixel_size);
        }
        return res;
    }
}  // namespace DE