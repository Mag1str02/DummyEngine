#include "../DummyEngine/Addition/DummyEngine.h"

int main()
{
    std::ofstream stream("test.yml");
    YAML::Node primes = YAML::Load("[2, 3, 5, 7, 11]");
    stream << primes; 
}