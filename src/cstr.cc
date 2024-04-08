#include "cstr.h"

char uint_to_string_output[128];
// Converts an unsigned int 64, `value`, into a char array that can 
// be printed.
const char* ToString(uint64_t value) {
    uint8_t size;
    uint64_t size_test = value;

    // Find amount of characters needed.
    while (size_test / 10 > 0) {
        size_test /= 10;
        size++;
    }

    uint8_t index = 0; // How far we are into buffer.
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10; // Remove one's digit.
        uint_to_string_output[size - index] = remainder + '0'; // Note: working backwords.
        index++;
    }
    
    // Get final value.
    uint8_t remainder = value % 10;
    uint_to_string_output[size - index] = remainder + '0';

    // Null terminate string.
    uint_to_string_output[size + 1] = 0; // Note, not char for '0'.

    return uint_to_string_output;
}

char int_to_string_output[128];
// Converts a signed int 64, `value`, into a char array that can be 
// printed.
const char* ToString(int64_t value) {
    uint8_t is_negative = 0;

    if (value < 0) {
        is_negative = 1; // Offset to allow for negative symbol.
        value *= -1; // Convert value to positive.
        int_to_string_output[0] = '-';
    }

    uint8_t size;
    uint64_t size_test = value;

    // Find amount of characters needed.
    while (size_test / 10 > 0) {
        size_test /= 10;
        size++;
    }

    uint8_t index = 0; // How far we are into buffer.
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10; // Remove one's digit.
        int_to_string_output[is_negative + size - index] = remainder + '0'; // Note: working backwords.
        index++;
    }
    
    // Get final value.
    uint8_t remainder = value % 10;
    int_to_string_output[is_negative + size - index] = remainder + '0';

    // Null terminate string.
    int_to_string_output[is_negative + size + 1] = 0;

    return int_to_string_output;
}

char double_to_string_output[128];
// Converts a double, `value`, into a char array that can be printed
// with decimal places, `decimal_places`.
const char* ToString(double value, uint8_t decimal_places) {
    if (decimal_places > 20) {
        decimal_places = 20;
    }

    char* int_ptr = (char*)ToString((int64_t)value);
    char* double_ptr = double_to_string_output;

    if (value < 0) {
        value *= -1;
    }

    while (*int_ptr != 0) {
        *double_ptr = *int_ptr; // Copy char by char
        int_ptr++;
        double_ptr++;
    }

    *double_ptr = '.';
    double_ptr++;

    double new_value = value - (int)value;

    for (uint8_t i = 0; i < decimal_places; i++) {
        new_value *= 10;
        *double_ptr = (int)new_value + '0';
        new_value -= (int)new_value;
        double_ptr++;
    }

    // Null terminate string.
    *double_ptr = 0;

    return double_to_string_output;
}

// Converts a double, `value`, into a char array that can be printed
// with two decimal places.
const char* ToString(double value) {
    return ToString(value, 2);
}

char hex_to_string_output[128];
// Converts an unsigned int 64, `value`, into a char array of hex
// values that can be printed.
const char* ToHString(uint64_t value) {
    uint64_t* val_ptr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)val_ptr + i);
        temp = ((*ptr & 0xF0) >> 4); // Get left side of hex value.
        hex_to_string_output[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
        temp = ((*ptr & 0x0F));
        hex_to_string_output[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
    }

    // Null terminate string.
    hex_to_string_output[size + 1] = 0;

    return hex_to_string_output;
}

char hex_to_string_output32[128];
// Converts an unsigned int 32, `value`, into a char array of hex
// values that can be printed.
//
// This method will print half the leading values of 
// `const char* ToHString(uint64_t value)`
const char* ToHString(uint32_t value) {
    uint32_t* val_ptr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = 4 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)val_ptr + i);
        temp = ((*ptr & 0xF0) >> 4); // Get left side of hex value.
        hex_to_string_output32[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
        temp = ((*ptr & 0x0F));
        hex_to_string_output32[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
    }

    // Null terminate string.
    hex_to_string_output32[size + 1] = 0;
    
    return hex_to_string_output32;
}

char hex_to_string_output16[128];
// Converts an unsigned int 16, `value`, into a char array of hex
// values that can be printed.
//
// This method will print half the leading values of 
// `const char* ToHString(uint32_t value)`
const char* ToHString(uint16_t value) {
    uint16_t* val_ptr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)val_ptr + i);
        temp = ((*ptr & 0xF0) >> 4); // Get left side of hex value.
        hex_to_string_output16[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
        temp = ((*ptr & 0x0F));
        hex_to_string_output16[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
    }

    // Null terminate string.
    hex_to_string_output16[size + 1] = 0;
    
    return hex_to_string_output16;
}

char hex_to_string_output8[128];
// Converts an unsigned int 8, `value`, into a char array of hex
// values that can be printed.
//
// This method will print half the leading values of 
// `const char* ToHString(uint16_t value)`
const char* ToHString(uint8_t value) {
    uint8_t* val_ptr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = 1 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)val_ptr + i);
        temp = ((*ptr & 0xF0) >> 4); // Get left side of hex value.
        hex_to_string_output8[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
        temp = ((*ptr & 0x0F));
        hex_to_string_output8[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
    }

    // Null terminate string.
    hex_to_string_output8[size + 1] = 0;
    
    return hex_to_string_output8;
}