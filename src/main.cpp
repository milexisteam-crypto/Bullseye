#include <stdint.h>
#include <stddef.h>
#include "limine.h"
#include "font.hpp"

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

static limine_framebuffer *fb = nullptr;
static uint32_t *fb_ptr = nullptr;

struct Color { uint8_t r,g,b; };

static inline uint32_t RGB(uint8_t r,uint8_t g,uint8_t b){
    return (r<<16)|(g<<8)|b;
}

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

// ===============================
// FONT STRUCTURE
// ===============================

struct Font {
    int w = 8;
    int h = 16;
    uint32_t g[256][32];
} font;

// ===============================
// SAFE HELPERS
// ===============================

static inline int is_hex(char c){
    return (c>='0'&&c<='9')||
           (c>='a'&&c<='f')||
           (c>='A'&&c<='F');
}

static inline int hex_val(char c){
    if(c>='0'&&c<='9') return c-'0';
    if(c>='a'&&c<='f') return 10+(c-'a');
    if(c>='A'&&c<='F') return 10+(c-'A');
    return 0;
}

// ===============================
// BUILTIN FONT LOADER (SAFE VERSION)
// ===============================

bool load_bdf_builtin(const uint8_t *data, uint64_t size){
    if(!data || size == 0) return false;

    const char *p   = (const char*)data;
    const char *end  = p + size;

    // clear font
    for(int c=0;c<256;c++)
        for(int r=0;r<32;r++)
            font.g[c][r]=0;

    int enc = -1;
    bool bitmap = false;
    int row = 0;

    int safety = 0;

    while(p < end){
        if(++safety > 2000000) return false; // anti-freeze guard

        const char *line = p;

        while(p < end && *p!='\n' && *p!='\r') p++;
        size_t len = p - line;

        while(p < end && (*p=='\n'||*p=='\r')) p++;

        // ---------------- ENCODING ----------------
        if(len >= 8 && line[0]=='E' && line[1]=='N' && line[2]=='C'){
            int val = 0;
            int i = 9;

            while(i < (int)len && line[i]>='0' && line[i]<='9'){
                val = val*10 + (line[i]-'0');
                i++;
            }

            if(val >= 0 && val <= 255)
                enc = val;
            else
                enc = -1;

            row = 0;
        }

        // ---------------- BITMAP START ----------------
        else if(len >= 6 && line[0]=='B' && line[1]=='I'){
            bitmap = true;
            row = 0;
        }

        // ---------------- END ----------------
        else if(len >= 3 && line[0]=='E' && line[1]=='N'){
            bitmap = false;
            enc = -1;
        }

        // ---------------- BITMAP DATA ----------------
        else if(bitmap && enc >= 0 && enc < 256){

            if(row >= 32) continue;

            uint32_t bits = 0;

            for(size_t i=0;i<len;i++){
                if(!is_hex(line[i])) continue;
                bits <<= 4;
                bits |= hex_val(line[i]);
            }

            font.g[enc][row] = bits;
            row++;
        }
    }

    return true;
}

// ===============================
// TEXT RENDERING
// ===============================

void draw_char(int x,int y,char c,Color col){
    unsigned char uc = (unsigned char)c;

    for(int r=0;r<font.h;r++){
        uint32_t bits = font.g[uc][r];

        for(int b=0;b<font.w;b++){
            if(bits & (1u << (31-b))){
                Pixel(x+b, y+r, col);
            }
        }
    }
}

void draw_string(int x,int y,const char *s,Color col){
    int cx = x;

    while(*s){
        draw_char(cx,y,*s,col);
        cx += font.w;
        s++;
    }
}

// ===============================
// BUILTIN FONT DATA (FROM font.hpp)
// ===============================
// musisz mieć w font.hpp:
// extern const uint8_t font_bdf[];
// extern const uint64_t font_bdf_size;

// ===============================
// KERNEL MAIN
// ===============================

extern "C" void kernel_main(){

    if(!framebuffer_request.response ||
       framebuffer_request.response->framebuffer_count < 1){
        for(;;)__asm__("hlt");
    }

    fb = framebuffer_request.response->framebuffers[0];
    fb_ptr = (uint32_t*)fb->address;

    Clear({0,0,64});

    // LOAD BUILTIN FONT
    if(!load_bdf_builtin(font_bdf, font_bdf_size)){
        Clear({255,0,0});
        for(;;)__asm__("hlt");
    }

    Clear({0,255,0});

    draw_string(20,20,"HELLO WORLD",{255,255,255});

    for(;;)__asm__("hlt");
}

// ===============================
// ENTRY
// ===============================

extern "C" void _start(){
    kernel_main();
    for(;;)__asm__("hlt");
}