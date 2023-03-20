#include "DummyEngine/ToolBox/Editors/TextureEditor.h"

namespace DE {
    Ref<TextureData> TextureEditor::GetTexturePart(const Ref<TextureData> data, uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos) {
        Ref<TextureData> res = CreateRef<TextureData>();
        res->SetData(data->Data(), width, height, data->Format());
        uint32_t       pixel_size = data->Channels();
        uint8_t*       new_image  = res->Data();
        const uint8_t* old_image  = data->Data();
        for (size_t i = 0; i < height; ++i) {
            std::memcpy(new_image + (i * width * pixel_size), old_image + (data->Width() * (i + y_pos) + x_pos) * pixel_size, width * pixel_size);
        }
        return res;
    }

    Ref<TextureData> TextureEditor::GetSkyBoxSide(const Ref<TextureData> data, CubeSide side) {
        uint32_t x_step = data->Width() / 4;
        uint32_t y_step = data->Height() / 3;

        switch (side) {
            case CubeSide::Top: return GetTexturePart(data, x_step, y_step, x_step, 0);
            case CubeSide::Bottom: return GetTexturePart(data, x_step, y_step, x_step, y_step * 2);
            case CubeSide::Left: return GetTexturePart(data, x_step, y_step, 0, y_step);
            case CubeSide::Right: return GetTexturePart(data, x_step, y_step, x_step * 2, y_step);
            case CubeSide::Back: return GetTexturePart(data, x_step, y_step, x_step * 3, y_step);
            case CubeSide::Front: return GetTexturePart(data, x_step, y_step, x_step, y_step);
            default: return nullptr;
        }
    }
}  // namespace DE