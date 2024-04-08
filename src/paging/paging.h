#pragma once
#include <stdint.h>

struct PageDirectoryEntry {
    bool present : 1; // If the page directory that this is pointing to is accesible.
    bool read_write : 1; // If read AND write function is enabled.
    bool user_super : 1; // If the user AND supervisor can access the page.
    bool write_through : 1;
    bool cache_disabled : 1;
    bool accessed : 1; // Set to 1 if CPU accesses page in memory.
    bool ignore0 : 1; // Reserved bit.
    bool larger_pages : 1;
    bool ignore1 : 1; // Reserved bit.
    uint8_t available : 3; // 3 available bits for use.
    uint64_t address : 52; 
};

struct PageTable {
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000)));
