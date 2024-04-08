#pragma once
#include <stdint.h>

class PageMapIndexer {
    public:
    PageMapIndexer(uint64_t virtual_address);
    uint64_t PDP_i_; // Page directory pointer Index
    uint64_t PD_i_; // Page directory index
    uint64_t PT_i_; // Page table index
    uint64_t P_i_; // Page index
};