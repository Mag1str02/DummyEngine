#include <iostream>

int main() {
    for (size_t i = 0; i < 10; ++i) {
        int* a = new int();
        *a     = 1;
        delete a;
        std::cout << *a << std::endl;
    }
}