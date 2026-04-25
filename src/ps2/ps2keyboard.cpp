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

char key() {
    uint8_t sc = read_scancode();

    // Obsługa extended scancode (0xE0)
    if (sc == 0xE0) {
        extended = true;
        return 0;
    }

    // Break code (klawisz puszczony)
    if (sc & 0x80) {
        uint8_t make = sc & 0x7F;

        if (make == 0x2A || make == 0x36) shift = false;

        extended = false;
        return 0;
    }

    // Make code (klawisz wciśnięty)
    if (sc == 0x2A || sc == 0x36) {
        shift = true;
        return 0;
    }

    // Extended keys (strzałki itd.)
    if (extended) {
        extended = false;
        return 0; // na razie ignorujemy
    }

    return scancode_to_ascii(sc, shift);
}
