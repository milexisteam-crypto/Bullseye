#include "limine.h"
#include "stdint.h"
#include <stddef.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

extern "C" void kmain(void) {
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    for (size_t i = 0; i < 100; i++) {
        // z jakiegokolwiek powodu to nie dzialaaa
        volatile uint32_t *fb_ptr = (uint32_t*)framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0x2233;
    }

    __asm__("hlt");
}