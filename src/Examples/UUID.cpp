#include "../DummyEngine/Addition/DummyEngine.h"
#include <unordered_set>

int main()
{
    std::unordered_set<uint64_t> uuids;
    uint64_t cnt = 0;
    while (true)
    {
        DE::UUID uuid;
        if (uuids.find((uint64_t)uuid) != uuids.end())
        {
            std::cout << "Collision detected at: " << cnt << std::endl;
        }
        uuids.insert((uint64_t)uuid);
        if (cnt % 1'000'000 == 0)
        {
            std::cout << cnt << std::endl;
        }
        ++cnt;
    }
}