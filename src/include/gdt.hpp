#pragma once
#include <stdint.h>

struct __attribute__((packed)) GdtDescriptor {
    uint16_t limit;
    uint64_t base;
};

struct __attribute__((packed)) GdtEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  gran;
    uint8_t  base_high;
};

enum : uint16_t {
    GDT_NULL = 0x00,
    GDT_CODE = 0x08,
    GDT_DATA = 0x10,
};

void gdt_init();
