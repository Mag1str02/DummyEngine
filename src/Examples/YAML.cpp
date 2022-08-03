#include "../DummyEngine/Addition/DummyEngine.h"

int main()
{
    std::ofstream stream("test.yml");
    YAML::Node file;
    YAML::Node scene, entities, entity1, entity2;
    file["Scene"] = scene;

    scene["name"] = "Test scene";
    scene["entities"] = entities;

    entities.push_back(entity1);
    entities.push_back(entity2);

    entity1["Name"] = "First Entity";
    entity1["UUID"] = (uint64_t)DE::UUID();
    entity1["Transform"]["Translation"] = YAML::Load("[]");
    entity1["Transform"]["Rotation"] = YAML::Load("[1,2,2]");
    entity1["Transform"]["Scale"] = YAML::Load("[1,1,1]");

    stream << file;
}