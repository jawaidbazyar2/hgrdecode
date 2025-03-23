
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

int loresMapIndex[24] =
  {   // text page 1 line addresses
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

#define DEBUG 1

size_t generateLoresScanline(uint8_t *loresData, int startOffset, 
                           uint8_t *output, size_t outputOffset) {
    int lastByte = 0x00;
    size_t index = outputOffset;
    
    // Process 40 bytes (one scanline)
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 40; x++) {
            uint8_t byte = loresData[startOffset + x];

            if (y & 4) { // if we're in the second half of the scanline, shift the byte right 4 bits to get the other nibble
                byte = byte >> 4;
            }
            int pixeloff = (x * 14) % 4;

            for (int bits = 0; bits < 14; bits++) {
                uint8_t bit = ((byte >> pixeloff) & 0x01);
                uint8_t gray = (bit ? 0xFF : 0x00);
                output[index++] = gray;
                pixeloff = (pixeloff + 1) % 4;
            }
        }
    }
    
    return index - outputOffset; // Return number of bytes written
}

uint8_t *loresToGray(uint8_t *loresData) {
    // Calculate output size and pre-allocate
    const int pixelsPerScanline = 40 * CELL_WIDTH;
    const size_t totalPixels = 192 * pixelsPerScanline;
    uint8_t *bitSignalOut = new uint8_t[totalPixels];
    
    // Process each scanline (0-191)
    for (int y = 0; y < 24; y++) {
        // Get the starting memory offset for this scanline
        int offset = loresMapIndex[y];
        
        // Process this scanline directly
        size_t outputOffset = y * pixelsPerScanline * 8;
        size_t bytesWritten = generateLoresScanline(loresData, offset, bitSignalOut, outputOffset);
        
        // Print progress every 20 scanlines
        if (DEBUG ) {
            printf("Processed scanline %d at offset %04X\n", y, offset);
        }
    }
    
    printf("Processed entire image: %zu pixels\n", totalPixels);
    return bitSignalOut;
}