#include "bui_core.hpp"
#include <cstdint>
struct Font {
    int w = 8;
    int h = 16;
    uint32_t g[256][32];
};

extern Font font;


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
// TEXT RENDERING
// ===============================

void draw_char(int x,int y,char c,Color col);

extern void draw_string(int x,int y,const char *s,Color col);