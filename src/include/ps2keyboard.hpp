#pragma once
#include <stdint.h>

// Zwraca ASCII znak lub 0 jeśli brak
char readKey();

// Zwraca kod specjalnego klawisza lub 0 jeśli brak
uint16_t readSpecialKey();

// Konwersja scancode → ASCII
char scancode_to_ascii(uint8_t scancode, bool shift);
