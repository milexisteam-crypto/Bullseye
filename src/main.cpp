#include "limine.h"
#include "stdint.h"
#include <stddef.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

typedef struct Color{
    int r;
    int g;
    int b;
}Color;

struct limine_framebuffer *framebuffer;
volatile uint32_t *fb_ptr;

unsigned long RGB(int r, int g, int b)
{   
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void DrawPixel(int x, int y, Color color){
    fb_ptr[x * (framebuffer->pitch / 4) + y] = RGB(color.r, color.g, color.b);
}

extern "C" void kmain(void) {
    framebuffer = framebuffer_request.response->framebuffers[0];
    fb_ptr = (uint32_t*)framebuffer->address;
    Color col = {255,255,255};
    DrawPixel(20,20, col);

    __asm__("hlt");
}