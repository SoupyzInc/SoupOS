#pragma once
#include "paging.h"

class PageTableManager {
    public:
    PageTableManager(PageTable* PML4);
    PageTable* PML4_;
    void MapMemory(void* virtual_memory, void* physical_memory);
};