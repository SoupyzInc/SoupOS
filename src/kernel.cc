#include <stddef.h>
#include "basic_renderer.h"
#include "cstr.h"
#include "efi_memory.h"
#include "memory.h"
#include "bitmap.h"
#include "paging/page_frame_allocator.h"
#include "paging/page_map_indexer.h"
#include "paging/page_table_manager.h"
#include "paging/paging.h"

struct BootInfo {
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* m_map; // Pointer to memory descriptor struct. # of pages of mem, address of mem, type of mem, etc.
	uint64_t m_map_size; // Complete size of map in bytes.
	uint64_t m_map_desc_size; // How big each descriptor entry is.
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* boot_info) {
    BasicRenderer renderer = BasicRenderer(boot_info->framebuffer, boot_info->psf1_font);

    uint64_t m_map_entries = boot_info->m_map_size / boot_info->m_map_desc_size;

    kAllocator = PageFrameAllocator();
    kAllocator.ReadEfiMemoryMap(boot_info->m_map, boot_info->m_map_size, boot_info->m_map_desc_size);

    uint64_t kernel_size = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;

    kAllocator.LockPages(&_KernelStart, kernel_pages);

    PageTable* PML4 = (PageTable*)kAllocator.RequestPage();
    MemSet(PML4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(boot_info->m_map, m_map_entries, boot_info->m_map_desc_size); t+= 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)boot_info->framebuffer->base_address;
    uint64_t fbSize = (uint64_t)boot_info->framebuffer->buffer_size + 0x1000;
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4)); // Mov control register 3, into 

    renderer.Print("Successfully moved into new page map.");
    renderer.cursor_position_ = { 0, renderer.cursor_position_.y += 16 };

    pageTableManager.MapMemory((void*)0x80010, (void*)0x80000);
    uint64_t* addrV = (uint64_t*)0x80010;
    *addrV = 21;

    uint64_t* addrP = (uint64_t*)0x80000;

    renderer.Print(ToString(*addrP));
    renderer.cursor_position_ = { 0, renderer.cursor_position_.y += 16 };

    // uint64_t* addrP = (uint64_t*)0x80000;
    // *addrP = 21;

    // renderer.Print(ToString(*addrP));
    // renderer.cursor_position_ = { 0, renderer.cursor_position_.y += 16 };

    return ;
}