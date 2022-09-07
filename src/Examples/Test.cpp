#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

int main()
{
    Config::Init();
    Path sky_box = Config::GetPath(DE_CFG_TEXTURE_PATH) / "Skyboxes" / "Sky";
    auto cross = TextureLoader::Load({sky_box / "Sky.png", false});
    for (size_t i = 0; i < 6; ++i)
    {
        std::cout << CubeSide(i) << std::endl;
        // TextureLoader::Save(sky_box / ("Sky" + std::to_string(i) + ".png"), TextureEditor::GetSkyBoxSide(cross, CubeSide(i)));
    }
}