#include "page_map_indexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtual_address) {
    virtual_address >>= 12; // Correct for 12 bit alignment.
    P_i_ = virtual_address & 0x1ff; 
    virtual_address >>= 9;
    PT_i_ = virtual_address & 0x1ff;
    virtual_address >>= 9;
    PD_i_ = virtual_address & 0x1ff;
    virtual_address >>= 9;
    PDP_i_ = virtual_address & 0x1ff;
}