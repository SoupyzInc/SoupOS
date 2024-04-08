#include "page_frame_allocator.h"

uint64_t free_memory; // Free memory of the entire system.
uint64_t reserved_memory; // Memory that is reserved.
uint64_t used_memory; // Memory that has been requested.
bool initialized = false; // Whether or not a PageFrameAllocator has been initialized or not. Prevents multiple allocators from being initialized.
PageFrameAllocator kAllocator;

void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* m_map, size_t m_map_size, size_t m_map_desc_size) {
    if (initialized) {
        return;
    }

    initialized = true;

    uint64_t m_map_entries = m_map_size / m_map_desc_size;

    // Find largest segment of memory with type 7 (EfiConventionalMemory).
    void* largest_free_mem_seg = NULL;
    size_t largest_free_mem_seg_size = 0;

    for (int i = 0; i < m_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)m_map + (i * m_map_desc_size));
        if (desc->type == 7) {
            if (desc->num_pages * 4096 > largest_free_mem_seg_size) {
                largest_free_mem_seg = desc->phys_addr;
                largest_free_mem_seg_size = desc->num_pages;
            }
        }
    }

    uint64_t memory_size = GetMemorySize(m_map, m_map_entries, m_map_desc_size);
    free_memory = memory_size;

    uint64_t bitmap_size = memory_size / 4096 / 8 + 1;

    InitBitmap(bitmap_size, largest_free_mem_seg); // Initialize bitmap.
    LockPages(&page_bitmap_, page_bitmap_.size_ / 4096 + 1); // Lock pages of bitmap.

    // Reserve pages of unusable/reserved memory.
    for (int i = 0; i < m_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)m_map + (i * m_map_desc_size));
        if (desc->type != 7) {
            ReservePages(desc->phys_addr, desc->num_pages);
        }
    }
}

// Initializes a bitmap of size, `bitmap_size`, at `buffer_address` 
// with values 0.
void PageFrameAllocator::InitBitmap(size_t bitmap_size, void* buffer_address) {
    page_bitmap_.size_ = bitmap_size;
    page_bitmap_.buffer_ = (uint8_t*)buffer_address;
    // Set mem to 0.
    for (int i = 0; i < bitmap_size; i++) {
        *(uint8_t*)(page_bitmap_.buffer_ + i) = 0;
    }
}

void PageFrameAllocator::FreePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    // Check if page is already free.
    if (page_bitmap_[index] == false) {
        return;
    }

    page_bitmap_.Set(index, false);
    free_memory += 4096;
    used_memory -= 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::LockPage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    // Check if page is already locked.
    if (page_bitmap_[index] == true) {
        return;
    }

    page_bitmap_.Set(index, true);
    free_memory -= 4096;
    used_memory += 4096;
}

void PageFrameAllocator::LockPages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

// Finds the earliest free page. If non are available, returns NULL.
void* PageFrameAllocator::RequestPage() {
    // TODO: Implement better search algo.
    for (uint64_t i = 0; i < page_bitmap_.size_ * 8; i++) {
        if (page_bitmap_[i] == true) {
            continue;
        } 

        LockPage((void*)(i * 4096));
        return (void*)(i * 4096);
    }

    return NULL; // TODO: Page frame swap to file.
}

uint64_t PageFrameAllocator::GetFreeRam() {
    return free_memory;
}

uint64_t PageFrameAllocator::GetUsedRam() {
    return used_memory;
}

uint64_t PageFrameAllocator::GetReservedRam() {
    return reserved_memory;
}

void PageFrameAllocator::ReservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    // Check if page is already locked.
    if (page_bitmap_[index] == true) {
        return;
    }

    page_bitmap_.Set(index, true);
    free_memory -= 4096;
    reserved_memory += 4096;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    // Check if page is already free.
    if (page_bitmap_[index] == false) {
        return;
    }

    page_bitmap_.Set(index, false);
    free_memory += 4096;
    reserved_memory -= 4096;
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        UnreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}
