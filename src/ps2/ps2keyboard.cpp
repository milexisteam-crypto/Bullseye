#include <ps2keyboard.hpp>

uint8_t read_scancode() {
  while (!(inb(0x64) & 1));
  return inb(0x60);
}

char scancode_to_ascii(uint8_t scancode, bool shift) {
  static char map[128] = {0,    27,  '1', '2',  '3',  '4',  '5', '6', '7',  '8',
                          '9',  '0', '-', '=',  '\b', '\t', 'q', 'w', 'e',  'r',
                          't',  'y', 'u', 'i',  'o',  'p',  '[', ']', '\n', 0,
                          'a',  's', 'd', 'f',  'g',  'h',  'j', 'k', 'l',  ';',
                          '\'', '`', 0,   '\\', 'z',  'x',  'c', 'v', 'b',  'n',
                          'm',  ',', '.', '/',  0,    '*',  0,   ' '};
  static char mapShift[128] = {
      0,   27,  '!',  '@',  '#',  '$', '%', '^', '&', '*', '(', ')',
      '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
      'O', 'P', '{',  '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
      'J', 'K', 'L',  ':',  '"',  '~', 0,   '|', 'Z', 'X', 'C', 'V',
      'B', 'N', 'M',  '<',  '>',  '?', 0,   '*', 0,   ' '};

  if (scancode > 127)
    return 0;
  if (shift)
    return mapShift[scancode];
  return map[scancode];
}

char key(){
  uint8_t scancode = read_scancode();
  bool shift;
  if (scancode == 0x2A || scancode == 0x36) {
    shift = true;
  }

  if (scancode == 0xAA || scancode == 0xB6) {
    shift = false;
  }
  return scancode_to_ascii(scancode, shift);
}