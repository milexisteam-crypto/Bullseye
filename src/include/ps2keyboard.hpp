#pragma once
#include <stdint.h>

// Zwraca ASCII znak lub 0 jeśli brak znaku
char key();

// Konwersja scancode → ASCII (używane wewnętrznie)
char scancode_to_ascii(uint8_t scancode, bool shift);
