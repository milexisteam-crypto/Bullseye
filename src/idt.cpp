#include <idt.hpp>

struct idt_gate {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t reserved;            // reserved
};

typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) idt_register_t;

static idt_gate idt[256] = {0};
static idt_register_t idt_reg;

void set_idt_gate(int index, uint64_t handler, uint8_t flags){
   idt[index].offset_1 = handler;
   idt[index].selector = 0x08
   idt[index].ist = 0;
   idt[index].attributes = flags;
   idt[index].offset_2 = (uint16_t)(handler >> 16);
   idt[index].offset_3 = (uint32_t)(handler >> 32);
   idt[index].reserved = 0;
}

void set_idt(){
   idt_reg.base = (size_t)&idt;
   idt_reg.limit = 256 * sizeof(idt_gate) - 1;
   asm volatile("lidt %0" ::"m"(idt_reg) : "memory");
}