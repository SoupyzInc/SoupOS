#include "bitmap.h"

// Returns if a location in the bitmap is used or not.
bool Bitmap::operator[](uint64_t index) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_indexer = 0b10000000 >> bit_index;

    if ((buffer_[byte_index] & bit_indexer) > 0) {
        return true;
    }

    return false;
}

void Bitmap::Set(uint64_t index, bool value) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_indexer = 0b10000000 >> bit_index;

    // Set bit needed to 0, if true, then change to 1.
    buffer_[byte_index] &= ~bit_indexer;
    if (value) {
        buffer_[byte_index] |= bit_indexer;
    }
}
