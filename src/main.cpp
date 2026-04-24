#include <stdint.h>
#include <stddef.h>
#include "limine.h"
#include "font.hpp"
#include "bui_text.hpp"
#include <rtc.hpp>
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
// FONT STRUCTURE
// ===============================




// ===============================
// KERNEL MAIN
// ===============================

Time time;

extern "C" void kernel_main(){

    if(!framebuffer_request.response ||
       framebuffer_request.response->framebuffer_count < 1){
        for(;;)__asm__("hlt");
    }

    fb = framebuffer_request.response->framebuffers[0];
    fb_ptr = (uint32_t*)fb->address;

    Clear({0,0,64});
    Clear({0,255,0});
    char buf[3];

    buf[0] = (time.second / 10) + '0';
    buf[1] = (time.second % 10) + '0';
    buf[2] = 0;

    draw_string(20,20,"sekunda: ",{255,255,255});
    draw_char(90,20,buf[0] ,{255,255,255});
    draw_char(98,20,buf[1],{255,255,255});
   

    for(;;)__asm__("hlt");
}

// ===============================
// ENTRY
// ===============================

extern "C" void _start(){
    time = read_rtc();
    kernel_main();
    
    for(;;)__asm__("hlt");
}