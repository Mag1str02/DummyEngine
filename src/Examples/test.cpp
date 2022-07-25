#include <iostream>

class A
{
public:
    int c;
};


int main(){
    A b;
    std::cout << typeid(b).name() << std::endl;
}