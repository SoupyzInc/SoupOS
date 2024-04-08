#pragma once
#include <stdint.h>
#include "../efi_memory.h"
#include "../bitmap.h"
#include "../memory.h"

class PageFrameAllocator {
    public:
    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* m_map, size_t m_map_size, size_t m_map_desc_size);
    Bitmap page_bitmap_;
    void FreePage(void* address);
    void FreePages(void* address, uint64_t page_count);
    void LockPage(void* address);
    void LockPages(void* address, uint64_t page_count);
    void* RequestPage();
    uint64_t GetFreeRam();
    uint64_t GetUsedRam();
    uint64_t GetReservedRam();

    private:
    void InitBitmap(size_t bitmap_size, void* buffer_address);
    void ReservePage(void* address);
    void ReservePages(void* address, uint64_t page_count);
    void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t page_count);
};

extern PageFrameAllocator kAllocator;