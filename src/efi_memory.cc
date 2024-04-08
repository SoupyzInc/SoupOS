#include "efi_memory.h"

const char* EFI_MEMORY_TYPE_STRINGS[] {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory", // Memory UEFI has not used that the kernel can use.
    "EfiUnusableMemory", // Memory the kernel cannot use.
    "EfiACPIReclaimMemory", // Memory the kernel can use after after getting info from the ACPI tables.
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode"
};