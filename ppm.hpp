#pragma once

#include <vector>
#include "types.hpp"
#include "hgr.hpp"

bool writeImageToPPM(uint8_t *bitSignalOut, const char* filename);
bool writeImageToPBM(uint8_t *bitSignalOut, const char* filename);
bool writeImageToPGM(uint8_t *bitSignalOut, const char* filename);
uint8_t *readImageFromPGMFile(const char* filename);
void writePPMFile(const char* filename, RGBA *image, int width, int height);
