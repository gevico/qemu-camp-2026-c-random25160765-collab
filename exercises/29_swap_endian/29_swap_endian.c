#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    return ((num & 0x000000FF) << 24) |
           ((num & 0x0000FF00) << 8) |
           ((num & 0x00FF0000) >> 8) |
           ((num & 0xFF000000) >> 24);
}

int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}