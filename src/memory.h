#pragma once
#include <stdint.h>
#include "efi_memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* m_map, uint64_t m_map_entries, uint64_t m_map_desc_size);
void MemSet(void* start, uint8_t value, uint64_t num);