#ifndef BUI_CORE_HPP
#define BUI_CORE_HPP

#include <stdint.h>


struct Color { uint8_t r, g, b; };

static inline uint32_t RGB(uint8_t r,uint8_t g,uint8_t b){
    return (r<<16)|(g<<8)|b;
}

void Pixel(int x, int y, Color c);
void Clear(Color c);

#endif
