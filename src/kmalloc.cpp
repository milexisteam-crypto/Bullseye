#include <stdint.h>
#include <stddef.h>

extern "C" void* kmalloc(size_t size) {
    // Tymczasowy bump allocator
    static uint8_t* heap = (uint8_t*)0x1000000; // 16MB
    void* ptr = heap;
    heap += size;
    return ptr;
}
