#pragma once
#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR {
    uint32_t type; // Type of memory section.
    void* phys_addr; 
    void* virt_addr;
    uint64_t num_pages; // num_pages * 4096 is number of bytes that this mem section spans.
    uint64_t attribs; // Extra information.
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];
