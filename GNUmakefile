# =========================
# BASIC CONFIG
# =========================

.SUFFIXES:

override OUTPUT := myos

TOOLCHAIN :=
TOOLCHAIN_PREFIX :=

ifneq ($(TOOLCHAIN),)
    ifeq ($(TOOLCHAIN_PREFIX),)
        TOOLCHAIN_PREFIX := $(TOOLCHAIN)-
    endif
endif

# Compilers
CC  := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++
LD  := $(TOOLCHAIN_PREFIX)ld.lld
AS  := nasm

# =========================
# FLAGS
# =========================

CXXFLAGS := -g -O2 -pipe
CPPFLAGS := -I src -MMD -MP
NASMFLAGS := -f elf64 -g -F dwarf

# C++ freestanding kernel flags
override CXXFLAGS += \
    -Wall \
    -Wextra \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-red-zone \
    -mcmodel=kernel \
    -fno-exceptions \
    -fno-rtti \
    -fno-use-cxa-atexit

LDFLAGS := \
    -nostdlib \
    -static \
    -T linker.lds \
    -z max-page-size=0x1000 \
    --gc-sections

# =========================
# FILES
# =========================

override SRCFILES := $(shell find -L src -type f 2>/dev/null | LC_ALL=C sort)

CPPFILES := $(filter %.cpp,$(SRCFILES))
CFILES   := $(filter %.c,$(SRCFILES))
SFILES   := $(filter %.S,$(SRCFILES))
ASMFILES := $(filter %.asm,$(SRCFILES))

OBJ := \
    $(addprefix obj/,$(CPPFILES:.cpp=.cpp.o) \
                      $(CFILES:.c=.c.o) \
                      $(SFILES:.S=.S.o) \
                      $(ASMFILES:.asm=.asm.o))

DEPS := \
    $(addprefix obj/,$(CPPFILES:.cpp=.cpp.d) \
                      $(CFILES:.c=.c.d) \
                      $(SFILES:.S=.S.d))

# =========================
# TARGETS
# =========================

.PHONY: all clean run

all: bin/$(OUTPUT)

-include $(DEPS)

# =========================
# LINK
# =========================

bin/$(OUTPUT): GNUmakefile linker.lds $(OBJ)
	mkdir -p bin
	$(LD) $(LDFLAGS) $(OBJ) -o $@

# =========================
# C++
# =========================

obj/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# =========================
# C (optional)
# =========================

obj/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# =========================
# ASM (GCC-style)
# =========================

obj/%.S.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# =========================
# NASM
# =========================

obj/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(NASMFLAGS) $< -o $@

# =========================
# CLEAN
# =========================

clean:
	rm -rf bin obj