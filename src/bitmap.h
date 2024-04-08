#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap
{
    public:
    size_t size_; // Size of buffer the bitmap is using.
    uint8_t* buffer_; // Start of buffer for the bitmap.
    bool operator[](uint64_t index); // Allows bitmap to be used like an array. Get bitmap.
    void Set(uint64_t index, bool value); // Set bitmap.
};
