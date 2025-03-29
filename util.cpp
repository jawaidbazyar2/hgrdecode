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

#include "types.hpp"
#include <vector>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <cstring>

#include "hgr.hpp"

/**
 * @brief Loads an Apple II hi-res image file into a vector
 * 
 * @param filename Path to the Apple II hi-res image file
 * @return std::vector<uint8_t> Vector containing the 8,192 bytes of image data
 * @throws std::runtime_error if file cannot be opened or read
 */
uint8_t *readHiresFile(const char* filename) {
    // Apple II hi-res images are exactly 8,192 bytes
    const size_t HIRES_SIZE = 8192;
    
    // Pre-allocate the vector to the exact size
    uint8_t *imageData = new uint8_t[HIRES_SIZE];
    
    // Open the file in binary mode
    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    
    // Read the entire file into the vector
    size_t bytesRead = fread(imageData, 1, HIRES_SIZE, file);
    fclose(file);
    
    // Verify we read the correct amount
    if (bytesRead != 8192 && bytesRead != 8184) {
        throw std::runtime_error("File size incorrect - expected 8,192 or 8,184 bytes");
    }
    
    return imageData;
}

/**
 * @brief Loads an Apple II lo-res image file into a vector
 * 
 * @param filename Path to the Apple II hi-res image file
 * @return std::vector<uint8_t> Vector containing the 8,192 bytes of image data
 * @throws std::runtime_error if file cannot be opened or read
 */
uint8_t *readLoresFile(const char* filename) {
    // Apple II lo-res images are exactly 1024 bytes
    const size_t LORES_SIZE = 1024;
    
    // Pre-allocate the vector to the exact size
    uint8_t *imageData = new uint8_t[LORES_SIZE];
    
    // Open the file in binary mode
    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    
    // Read the entire file into the vector
    size_t bytesRead = fread(imageData, 1, LORES_SIZE, file);
    fclose(file);
    
    // Verify we read the correct amount
    if (bytesRead != LORES_SIZE) {
        throw std::runtime_error("File size incorrect - expected 1024 bytes");
    }
    
    return imageData;
}

char *rewriteExtension(const char *filename, const char *newExtension) {
    const char* inputExt = strrchr(filename, '.');
    size_t baseLen = inputExt - filename;
    char *newfilename = new char[256];
    if (inputExt) {
        strncpy(newfilename, filename, std::min(baseLen, (size_t)254));
        newfilename[std::min(baseLen, (size_t)254)] = '\0';
        strncat(newfilename, newExtension, 255 - std::min(baseLen, (size_t)254));
    } else {
        strncpy(newfilename, filename, 254);
        newfilename[254] = '\0';
        strncat(newfilename, newExtension, 255 - strlen(newfilename));
    }
    return newfilename;
}