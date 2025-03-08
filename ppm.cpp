#include <vector>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include "types.hpp"
#include "hgr.hpp"


void writePPMFile(const char* filename, RGBA *image, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    // Write PPM header (P6 format - binary RGB)
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data (RGB only, skip alpha)
    for (int i = 0; i < width * height; i++) {
        const RGBA& pixel = image[i];
        unsigned char rgb[3] = {pixel.r, pixel.g, pixel.b};
        fwrite(rgb, 1, 3, file);
    }
    
    fclose(file);
    printf("Successfully wrote image to %s\n", filename);
}

uint8_t *readImageFromPGMFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return nullptr;
    }

    // Read PGM header
    char magic[3];
    int width, height, maxval;
    if (fscanf(file, "%2s\n", magic) != 1) {
        fclose(file);
        return nullptr;
    }

    // Skip comments
    int c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n');
        c = getc(file);
    }
    ungetc(c, file);

    if (fscanf(file, "%d %d\n%d\n", &width, &height, &maxval) != 3) {
        fclose(file);
        return nullptr;
    }

    // Read pixel data
    uint8_t *data = new uint8_t[width * height];
    size_t bytesRead = fread(data, 1, width * height, file);
    fclose(file);

    if (bytesRead != width * height) {
        printf("Error: Incomplete read of PGM file\n");
        return nullptr;
    }

    return data;
}

/**
 * @brief Write the processed image data to a PPM file
 * 
 * @param bitSignalOut The processed image data
 * @param filename The output filename (should end with .ppm)
 * @return bool True if the file was written successfully, false otherwise
 */
bool writeImageToPPM(uint8_t *bitSignalOut, const char* filename) {
    const int width = 560;  // 40 bytes * 14 pixels per byte
    const int height = 192; // 192 scanlines
    const int size = width * height;

    // Open the file for writing
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return false;
    }
    
    // Write PPM header
    // P6 format is binary RGB data
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data
    // Each pixel is represented by 3 bytes (R, G, B)
    const int pixelsPerScanline = 40 * CELL_WIDTH;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate index in the bitSignalOut vector
            int index = y * pixelsPerScanline + x;
            
            // Get the pixel value (0x00 or 0xFF)
            uint8_t pixelValue = (index < size) ? bitSignalOut[index] : 0x00;
            
            // Write RGB values (white or black)
            uint8_t rgb[3] = {pixelValue, pixelValue, pixelValue};
            fwrite(rgb, 1, 3, file);
        }
    }
    
    fclose(file);
    printf("Image written to %s\n", filename);
    return true;
}

/**
 * @brief Write the processed image data to a PBM file (Portable Bitmap)
 * 
 * @param bitSignalOut The processed image data
 * @param filename The output filename (should end with .pbm)
 * @return bool True if the file was written successfully, false otherwise
 */
bool writeImageToPBM(uint8_t *bitSignalOut, const char* filename) {
    const int width = 560;  // 40 bytes * 14 pixels per byte
    const int height = 192; // 192 scanlines
    const int size = width * height;

    // Open the file for writing
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return false;
    }
    
    // Write PBM header
    // P4 format is binary (0=white, 1=black)
    fprintf(file, "P4\n%d %d\n", width, height);
    
    // In PBM, each row must be padded to a full byte
    const int bytesPerRow = (width + 7) / 8;
    const int pixelsPerScanline = 40 * CELL_WIDTH;
    
    // Buffer for a single row of bits
    uint8_t* rowBuffer = new uint8_t[bytesPerRow]();
    
    for (int y = 0; y < height; y++) {
        // Clear the buffer for this row
        memset(rowBuffer, 0, bytesPerRow);
        
        for (int x = 0; x < width; x++) {
            // Calculate index in the bitSignalOut vector
            int index = y * pixelsPerScanline + x;
            
            // Get the pixel value (0x00 or 0xFF)
            // In PBM, 0 is white and 1 is black, so we need to invert our values
            bool isBlack = (index < size) ? (bitSignalOut[index] == 0x00) : true;
            
            if (isBlack) {
                // Set the corresponding bit in the row buffer
                int byteIndex = x / 8;
                int bitIndex = 7 - (x % 8);  // MSB first
                rowBuffer[byteIndex] |= (1 << bitIndex);
            }
        }
        
        // Write the row to the file
        fwrite(rowBuffer, 1, bytesPerRow, file);
    }
    
    delete[] rowBuffer;
    fclose(file);
    printf("Image written to %s\n", filename);
    return true;
}

/**
 * @brief Write the processed image data to a PGM file (Portable Graymap)
 * 
 * @param bitSignalOut The processed image data
 * @param filename The output filename (should end with .pgm)
 * @return bool True if the file was written successfully, false otherwise
 */
bool writeImageToPGM(uint8_t *bitSignalOut, const char* filename) {
    const int width = 560;  // 40 bytes * 14 pixels per byte
    const int height = 192; // 192 scanlines
    const int size = width * height;

    // Open the file for writing
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return false;
    }
    
    // Write PGM header
    // P5 format is binary grayscale data
    fprintf(file, "P5\n%d %d\n255\n", width, height);
    
    // Write pixel data
    // Each pixel is represented by 1 byte (0-255 grayscale)
    const int pixelsPerScanline = 40 * CELL_WIDTH;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate index in the bitSignalOut vector
            int index = y * pixelsPerScanline + x;
            
            // Get the pixel value (0x00 or 0xFF)
            uint8_t pixelValue = (index < size) ? bitSignalOut[index] : 0x00;
            
            // Write grayscale value (single byte per pixel)
            fwrite(&pixelValue, 1, 1, file);
        }
    }
    
    fclose(file);
    printf("Image written to %s\n", filename);
    return true;
}
