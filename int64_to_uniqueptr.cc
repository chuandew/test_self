#include <cstdint>
#include <iostream>


int main() {
uint64_t value = 140419819878144;

uintptr_t ptr_value = (uintptr_t)value;
std::cout << "ptr:" << ptr_value << "\n";
}


