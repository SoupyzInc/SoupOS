#pragma once
#include <stdint.h>

const char* ToString(uint64_t value);
const char* ToString(int64_t value);

const char* ToString(double value, uint8_t decimal_places);
const char* ToString(double value);

const char* ToHString(uint64_t value);
const char* ToHString(uint32_t value);
const char* ToHString(uint16_t value);
const char* ToHString(uint8_t value);