#include "DummyEngine/ToolBox/Editors/TextureEditor.h"

namespace DE
{
    Ref<TextureData> TextureEditor::GetTexturePart(const Ref<TextureData> data, U32 width, U32 height, U32 x_pos, U32 y_pos)
    {
        Ref<TextureData> res = CreateRef<TextureData>();
        res->SetData(data->Data(), width, height, data->Format());
        U32 pixel_size = data->Channels();
        U8* new_image = res->Data();
        const U8* old_image = data->Data();
        for (size_t i = 0; i < height; ++i)
        {
            std::memcpy(new_image + (i * width * pixel_size), old_image + (data->Width() * (i + y_pos) + x_pos) * pixel_size, width * pixel_size);
        }
        return res;
    }

    Ref<TextureData> TextureEditor::GetSkyBoxSide(const Ref<TextureData> data, CubeSide side)
    {
        U32 x_step = data->Width() / 4;
        U32 y_step = data->Height() / 3;

        switch (side)
        {
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