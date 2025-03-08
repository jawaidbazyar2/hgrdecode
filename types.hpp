#pragma once

#include <vector>
#include <cstdint>

//typedef int int;
typedef signed int OESInt;
typedef uint8_t uint8_t;
typedef uint8_t Buffer[];

/**
 * We have three different data types for image data. Maybe four.
 * 1. RGBA - final output for emulator and input into PPM writer. four unsigned 8-bit elements.
 * 2. YIQ - intermedia form - three float elements.
 * 3. Mono - single unsigned 8-bit element. Just a luminance value, integer, 0-255.
 * 4. 
 *  */ 

struct RGBA {
    uint8_t r, g, b, a;
};

struct YIQ {
    float y, i, q;
};

struct Mono {
    uint8_t value;
};
