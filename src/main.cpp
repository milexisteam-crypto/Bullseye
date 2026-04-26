#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>   // potrzebne do va_list
#include <stdlib.h>   // jeśli używasz malloc
#include "limine.h"
#include "font.hpp"
#include "bui_text.hpp"
#include "rtc.hpp"
#include "formats.hpp"
#include "ps2keyboard.hpp"
#include <bootloader.hpp>
#include "gdt.hpp"
#include <idt.hpp>
#include <idt.hpp>
#include "string.hpp"
// ===============================
// LIMINE FRAMEBUFFER REQUEST
// ===============================

__attribute__((used, section(".limine_requests")))
static volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// ===============================
// FRAMEBUFFER
// ===============================

volatile limine_framebuffer *fb = nullptr;
volatile uint32_t *fb_ptr = nullptr;

// ===============================
// join()
// ===============================

void join(char* out, const char* first, ...) {
    if (!first) {
        out[0] = 0;
        return;
    }

    va_list args;
    va_start(args, first);

    size_t pos = 0;
    const char* s = first;

    while (s != nullptr) {
        for (size_t i = 0; s[i] != 0; i++)
            out[pos++] = s[i];

        s = va_arg(args, const char*);
    }

    va_end(args);

    out[pos] = 0;
}


// ===============================
// TIME
// ===============================

Time time;

// ===============================
// KERNEL MAIN
// ===============================

extern "C" void kernel_main() {
    gdt_init();
    enableInterrupts();
    if (!framebuffer_request.response ||
        framebuffer_request.response->framebuffer_count < 1) {
        for(;;) __asm__("hlt");
    }

    fb = framebuffer_request.response->framebuffers[0];
    fb_ptr = (uint32_t*)fb->address;

    Clear({0,0,64});

    // bufor na datę
String dataFormatted = "Date: " + FormatDate(time);

draw_string(20, 20, dataFormatted.c_str(), {255,255,255});

while(true) {
    draw_char(20, 40, readKey(), {255,255,255});
}

}

// ===============================
// ENTRY
// ===============================

extern "C" void _start() {
    disableInterrupts();
    
    parse_bootloader();
    time = read_rtc();
    kernel_main();

    for(;;) __asm__("hlt");
}
