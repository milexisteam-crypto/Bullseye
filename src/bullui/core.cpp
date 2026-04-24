#include "bui_core.hpp"
#include "main.hpp"


void Pixel(int x,int y,Color c){
    if(!fb) return;
    if((uint32_t)x >= fb->width) return;
    if((uint32_t)y >= fb->height) return;

    fb_ptr[y*(fb->pitch/4)+x] = RGB(c.r,c.g,c.b);
}

void Clear(Color c){
    if(!fb) return;

    uint32_t col = RGB(c.r,c.g,c.b);

    for(uint64_t y=0;y<fb->height;y++){
        for(uint64_t x=0;x<fb->width;x++){
            fb_ptr[y*(fb->pitch/4)+x] = col;
        }
    }
}