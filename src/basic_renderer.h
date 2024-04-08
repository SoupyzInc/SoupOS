#pragma once
#include "math.h"
#include "framebuffer.h"
#include "simple_fonts.h"

class BasicRenderer{
    public:
    BasicRenderer(Framebuffer* target_framebuffer, PSF1_FONT* psf1_font);
    Point cursor_position_;
    Framebuffer* target_framebuffer_;
    PSF1_FONT* psf1_font_;
    unsigned int color_;
    void PutChar(char chr, unsigned int x_off, unsigned int y_off);
    void Print(const char* str);
};