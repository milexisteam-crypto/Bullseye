#include "gdt.hpp"

static GdtEntry gdt[3];
static GdtDescriptor gdt_desc;

static void gdt_set_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[idx].limit_low = limit & 0xFFFF;
    gdt[idx].base_low  = base & 0xFFFF;
    gdt[idx].base_mid  = (base >> 16) & 0xFF;
    gdt[idx].access    = access;
    gdt[idx].gran      = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[idx].base_high = (base >> 24) & 0xFF;
}

void gdt_init() {
    // null
    gdt_set_entry(0, 0, 0, 0, 0);

    // kernel code (64-bit)
    gdt_set_entry(1, 0, 0, 0x9A, 0x20);

    // kernel data
    gdt_set_entry(2, 0, 0, 0x92, 0x00);

    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base  = (uint64_t)&gdt;

    // załaduj GDT
    asm volatile("lgdt %0" : : "m"(gdt_desc));

    // przeładuj segmenty danych
    asm volatile(
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        :
        :
        : "ax"
    );

    // UWAGA:
    // Nie przeładowujemy CS (brak retfq),
    // bo Limine już ustawił poprawny 64-bit code segment.
}
