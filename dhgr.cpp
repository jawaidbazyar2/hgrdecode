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

#include <vector>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include "types.hpp"
#include "hgr.hpp"
#include "ppm.hpp"
#include "font.hpp"
#include "util.hpp"

/** 
 * each one of these video mode modules takes Apple II source data, and creates a 560x192 array.
 */

#define DEBUG 0

int dhiresMap[24] = {
            0x0000,
            0x0080,
            0x0100,
            0x0180,
            0x0200,
            0x0280,
            0x0300,
            0x0380,

            0x0028,
            0x00A8,
            0x0128,
            0x01A8,
            0x0228,
            0x02A8,
            0x0328,
            0x03A8,

            0x0050,
            0x00D0,
            0x0150,
            0x01D0,
            0x0250,
            0x02D0,
            0x0350,
            0x03D0,
        };

int dhiresMapIndex(int y) {
    int row1 = y / 8;
    int row2 = y % 8;
    return dhiresMap[row1] + row2 * 0x400;
}

#if 0
std::vector<uint8_t> generateHiresData(const std::vector<uint8_t>& hiresBytes) {
    // Calculate exact size needed
    size_t outputSize = hiresBytes.size() * CELL_WIDTH;
    
    // Pre-allocate with exact size (resize, not reserve)
    std::vector<uint8_t> output(outputSize);
    
    int lastByte = 0x00;
    size_t index = 0;

    for (uint8_t byte : hiresBytes) {
        //printf("byte: %d lastByte: %d\n", byte, lastByte);
        size_t fontIndex = (byte | ((lastByte & 0x40) << 2)) * CHAR_WIDTH;
        //printf("fontIndex: %zu\n", fontIndex);
        
        for (int x = 0; x < CELL_WIDTH; x++) {
            // if last byte was 0x80 set, index into 2nd half of font 
            output[index++] = hires40Font[fontIndex + x];
        }

        lastByte = byte;
    }
    
    return output;
}
#endif

/**
 * @brief Processes a single scanline (40 bytes) of Apple II hi-res data
 * 
 * @param hiresData The complete hi-res image data
 * @param startOffset The starting offset in the image data for this scanline
 * @param output Vector to store the processed scanline data
 * @param outputOffset Offset in the output vector to store the data
 * @return The number of bytes written to the output
 */
size_t generatedHiresScanline(uint8_t *hiresData, int startOffset, 
                           uint8_t *output, size_t outputOffset) {
    size_t index = outputOffset;
    index+=1; // TODO: this is a dirty hack to alter the phase 90 degrees. Something's not right here though.

    // Process 40 bytes (one scanline)
    for (int x = 0; x < 40; x++) {
        uint8_t byteM = hiresData[startOffset + x];
        uint8_t byteA = hiresData[0x2000 + startOffset + x];
        for (int i = 0; i < 7; i++ ) {
            output[index] = (byteM & 0x01) ? 0xff : 0x00;
            index++;
            byteM >>= 1;
        }
        for (int i = 0; i < 7; i++ ) {
            output[index] = (byteA & 0x01) ? 0xff : 0x00;
            index++;
            byteA >>= 1;
        }
    }
    
    return index - outputOffset; // Return number of bytes written
}

/**
 * @brief Process an Apple II double hi-res image and convert it to bit signal data
 * 
 * @param hiresData The source double hi-res image data
 * @return std::vector<uint8_t> The processed image as bit signal data
 */
/** TODO: there is some hack we need to do where a pixel value carries over from the end of a previous scanline somehow. */
uint8_t *dhiresToGray(uint8_t *hiresData) {
    // Calculate output size and pre-allocate
    const int pixelsPerScanline = 40 * CELL_WIDTH;
    const size_t totalPixels = 192 * pixelsPerScanline;
    uint8_t *bitSignalOut = new uint8_t[totalPixels];
    
    // Process each scanline (0-191)
    for (int y = 0; y < 192; y++) {
        // Get the starting memory offset for this scanline
        int offset = dhiresMapIndex(y);
        
        // Process this scanline directly
        size_t outputOffset = y * pixelsPerScanline;
        size_t bytesWritten = generatedHiresScanline(hiresData, offset, bitSignalOut, outputOffset);
        
        // Print progress every 20 scanlines
        if (DEBUG && (y % 20 == 0)) {
            printf("Processed scanline %d\n", y);
        }
    }
    
    printf("Processed entire image: %zu pixels\n", totalPixels);
    return bitSignalOut;
}
