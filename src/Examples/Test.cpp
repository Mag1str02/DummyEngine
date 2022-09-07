#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

TextureData GetTextureSubregion(uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos, const TextureData& data)
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

int main()
{
    Config::Init();
    unsigned char* stb_data;
    int width, height, nrChannels;
    TextureFormat format;
    TextureData data;

    stbi_set_flip_vertically_on_load(false);

    stb_data = stbi_load((Config::GetPath(DE_CFG_TEXTURE_PATH) / "Skyboxes" / "Sky" / "Sky.png").string().c_str(), &width, &height, &nrChannels, 0);

    switch (nrChannels)
    {
        case 1: format = TextureFormat::RED; break;
        case 3: format = TextureFormat::RGB; break;
        case 4: format = TextureFormat::RGBA; break;
        default: format = TextureFormat::None; break;
    }

    data.SetData(stb_data, width, height, format);
    stbi_image_free(stb_data);

    TextureData res = GetTextureSubregion(512, 512, 512, 0, data);
    stbi_write_png((Config::GetPath(DE_CFG_TEXTURE_PATH) / "Skyboxes" / "Sky" / "Sky_Up.png").string().c_str(), res.Width(), res.Height(), res.Channels(), res.Data(), res.Width() * res.Channels());
    std::cout << "Done" << std::endl;
}