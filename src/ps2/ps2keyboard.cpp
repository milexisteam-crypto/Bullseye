#include <ps2keyboard.hpp>
#include "utils.hpp"

static bool shift = false;
static bool extended = false;

uint8_t read_scancode() {
    while (!(inb(0x64) & 1));
    return inb(0x60);
}

char scancode_to_ascii(uint8_t scancode, bool shift) {
    static char map[128] = {
        0, 27, '1','2','3','4','5','6','7','8',
        '9','0','-','=', '\b','\t','q','w','e','r',
        't','y','u','i','o','p','[',']','\n', 0,
        'a','s','d','f','g','h','j','k','l',';',
        '\'','`',0,'\\','z','x','c','v','b','n',
        'm',',','.','/',0,'*',0,' '
    };

    static char mapShift[128] = {
        0, 27, '!','@','#','$','%','^','&','*',
        '(',')','_','+','\b','\t','Q','W','E','R',
        'T','Y','U','I','O','P','{','}','\n',0,
        'A','S','D','F','G','H','J','K','L',':',
        '"','~',0,'|','Z','X','C','V','B','N',
        'M','<','>','?',0,'*',0,' '
    };

    if (scancode > 127)
        return 0;

    return shift ? mapShift[scancode] : map[scancode];
}

// ===============================
//  SPECIAL KEY CODES
// ===============================

#define KEY_ESC        0x0100
#define KEY_UP         0x0101
#define KEY_DOWN       0x0102
#define KEY_LEFT       0x0103
#define KEY_RIGHT      0x0104
#define KEY_DELETE     0x0105
#define KEY_HOME       0x0106
#define KEY_END        0x0107
#define KEY_PAGEUP     0x0108
#define KEY_PAGEDOWN   0x0109

uint16_t readSpecialKey() {
    uint8_t sc = read_scancode();

    if (sc == 0xE0) {
        extended = true;
        return 0;
    }

    if (sc & 0x80) {
        uint8_t make = sc & 0x7F;
        if (make == 0x2A || make == 0x36) shift = false;
        extended = false;
        return 0;
    }

    if (sc == 0x2A || sc == 0x36) {
        shift = true;
        return 0;
    }

    if (extended) {
        extended = false;

        switch (sc) {
            case 0x48: return KEY_UP;
            case 0x50: return KEY_DOWN;
            case 0x4B: return KEY_LEFT;
            case 0x4D: return KEY_RIGHT;
            case 0x53: return KEY_DELETE;
            case 0x47: return KEY_HOME;
            case 0x4F: return KEY_END;
            case 0x49: return KEY_PAGEUP;
            case 0x51: return KEY_PAGEDOWN;
        }

        return 0;
    }

    if (sc == 0x01) return KEY_ESC;

    return 0;
}

// ===============================
//  ASCII KEY READER
// ===============================

char readKey() {
    uint8_t sc = read_scancode();

    if (sc == 0xE0) {
        extended = true;
        return 0;
    }

    if (sc & 0x80) {
        uint8_t make = sc & 0x7F;
        if (make == 0x2A || make == 0x36) shift = false;
        extended = false;
        return 0;
    }

    if (sc == 0x2A || sc == 0x36) {
        shift = true;
        return 0;
    }

    if (extended) {
        extended = false;
        return 0;
    }

    return scancode_to_ascii(sc, shift);
}
