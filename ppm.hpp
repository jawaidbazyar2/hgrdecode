#pragma once
#include <vector>
#include "types.hpp"
#include "hgr.hpp"

bool writeImageToPPM(const std::vector<uint8_t>& bitSignalOut, const char* filename);
bool writeImageToPBM(const std::vector<uint8_t>& bitSignalOut, const char* filename);
bool writeImageToPGM(const std::vector<uint8_t>& bitSignalOut, const char* filename);
uint8_t *readImageFromPGMFile(const char* filename);
void writePPMFile(const char* filename, RGBA *image, int width, int height);
