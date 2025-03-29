/*
 *   Copyright (c) 2025 Jawaid Bazyar

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
