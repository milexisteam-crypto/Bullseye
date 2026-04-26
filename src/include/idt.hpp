#pragma once
#include <stdint.h>
#include <stddef.h>
void set_idt_gate(int index, uint64_t handler, uint8_t flags);
void set_idt();