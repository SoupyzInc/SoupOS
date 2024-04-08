#include "basic_renderer.h"

// Instantiantes a new BasicRenderer class with default color value 
// white and cursor position 0, 0.
BasicRenderer::BasicRenderer(Framebuffer* target_framebuffer, PSF1_FONT* psf1_font)  {
    target_framebuffer_ = target_framebuffer;
    psf1_font_ = psf1_font;
    color_ = 0xffffffff;
    cursor_position_ = { 0, 0 };
}

// Prints a single character, `chr` to the screen at point `x_off`, 
// `y_off`.
void BasicRenderer::PutChar(char chr, unsigned int x_off, unsigned int y_off) {
    unsigned int* pix_ptr = (unsigned int*)target_framebuffer_->base_address;
    char* font_ptr = (char*)psf1_font_->glyph_buffer + (chr * psf1_font_->psf1_header->char_size);
    
    // Select bit in bitmap to draw.
    for (unsigned long y = y_off; y < y_off + 16; y++) {
        for (unsigned long x = x_off; x < x_off + 8; x++) {
            if ((*font_ptr & (0b10000000 >> (x - x_off))) > 0) {
                // Bit is on.
                *(unsigned int*)(pix_ptr + x+ (y * target_framebuffer_->pixels_per_scan_line)) = color_;
            }
        }

        font_ptr++;
    }
}

// Prints an entire string, `str`, to the screen at this renderer's 
// cursor position.
void BasicRenderer::Print(const char* str) {
    char* chr = (char*)str;
    while (*chr != 0) {
        PutChar(*chr, cursor_position_.x, cursor_position_.y);
        cursor_position_.x+=8;

        // Handle text overflow and move to new line.
        if (cursor_position_.x + 8 > target_framebuffer_->width) {
            cursor_position_.x = 0;
            cursor_position_.y += 16;
        }
        chr++;
    }
}