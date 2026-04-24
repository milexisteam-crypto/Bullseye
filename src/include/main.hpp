#include <cstdint>
#pragma once
#include "limine.h"
static volatile limine_framebuffer_request framebuffer_request;

// ===============================
// FRAMEBUFFER
// ===============================

extern volatile limine_framebuffer *fb;
extern volatile uint32_t *fb_ptr;