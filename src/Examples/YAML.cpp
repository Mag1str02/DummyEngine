#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

int main()
{
    for(size_t i = 0; i < 100; ++i){ 
        std::cout << DE::UUID() << std::endl;
    }
}