#include <iostream>
#include <cstdio>

int main() {
    float a = 3.0;
    uint8_t b = static_cast<uint8_t>(a);
    printf("%d\n", b);
    std::cout << static_cast<int>(b) << std::endl;
    return 0;
}
