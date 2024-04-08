#include "memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* m_map, uint64_t m_map_entries, uint64_t m_map_desc_size) {
    static uint64_t memory_size_bytes = 0; // Static to remain the same value thorugh concurrent calls to the function.

    // Already calculated mem size; do not recalculate
    if (memory_size_bytes > 0) {
        return memory_size_bytes;
    }

    for (int i = 0; i < m_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)m_map + (i * m_map_desc_size));
        memory_size_bytes += desc->num_pages * 4096;
    }

    return memory_size_bytes;
}

void MemSet(void* start, uint8_t value, uint64_t num) {
    for (uint64_t i = 0; i < num; i++) {
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}