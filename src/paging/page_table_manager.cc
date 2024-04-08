#include "page_table_manager.h"
#include "page_map_indexer.h"
#include "page_frame_allocator.h"
#include "../memory.h"
#include <stdint.h>

PageTableManager::PageTableManager(PageTable* PML4) {
    this->PML4_ = PML4;
}

void PageTableManager::MapMemory(void* virtual_memory, void* physical_memory) {
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtual_memory);
    PageDirectoryEntry PDE;

    PDE = PML4_->entries[indexer.PDP_i_];
    PageTable* PDP;
    if (!PDE.present) {
        PDP = (PageTable*)kAllocator.RequestPage();
        MemSet(PDP, 0, 0x1000);
        PDE.address = (uint64_t)PDP >> 12;
        PDE.present = true;
        PDE.read_write = true;
        
        PML4_->entries[indexer.PDP_i_] = PDE;
    } else {
        PDP = (PageTable*)((uint64_t)PDE.address << 12);
    }

    PDE = PDP->entries[indexer.PD_i_];
    PageTable* PD;
    if (!PDE.present) {
        PD = (PageTable*)kAllocator.RequestPage();
        MemSet(PD, 0, 0x1000);
        PDE.address = (uint64_t)PD >> 12;
        PDE.present = true;
        PDE.read_write = true;
        
        PDP->entries[indexer.PD_i_] = PDE;
    } else {
        PD = (PageTable*)((uint64_t)PDE.address << 12);
    }

    PDE = PD->entries[indexer.PT_i_];
    PageTable* PT;
    if (!PDE.present) {
        PT = (PageTable*)kAllocator.RequestPage();
        MemSet(PT, 0, 0x1000);
        PDE.address = (uint64_t)PT >> 12;
        PDE.present = true;
        PDE.read_write = true;
        
        PD->entries[indexer.PT_i_] = PDE;
    } else {
        PT = (PageTable*)((uint64_t)PDE.address << 12);
    }

    PDE = PT->entries[indexer.P_i_];
    PDE.address = (uint64_t)physical_memory >> 12;
    PDE.present = true;
    PDE.read_write = true;
    PT->entries[indexer.P_i_] = PDE;
}