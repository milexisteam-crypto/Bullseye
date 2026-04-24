#include <cstdint>
#include "bui_core.hpp"
#include "font.hpp"
struct Font {
    int w = 8;
    int h = 16;
    uint32_t g[256][32];
} font;

// ===============================
// SAFE HELPERS
// ===============================




// ===============================
// TEXT RENDERING
// ===============================

void draw_char(int x,int y,char c,Color col){
    unsigned char uc = (unsigned char)c;
    int set;
    for (int w=0; w < 8; w++) {
        for (int z=0; z < 8; z++) {
            set = font8x8_basic[uc][z] & (1 << w);
            if(set){
                Pixel(x + w, y + z, col);
            }
        }
    }
}

void draw_string(int x, int y, const char *s, Color col) {
    int cx = x;

    while (*s) {
        draw_char(cx, y, *s, col);
        cx += 8; // szerokość znaku (lub font.w jeśli używasz klasy Font)
        s++;
    }
}
