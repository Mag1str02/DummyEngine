#include "DummyEngine/DummyEngine.h"

using namespace DE;

int main() {
    freopen("output.txt", "w", stdout);
    for (size_t i = 0; i < 100; ++i) {
        std::cout << (std::string)UUID::Generate() << std::endl;
    }
}