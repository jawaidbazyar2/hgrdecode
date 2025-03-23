#pragma once

#include "types.hpp"
#include <vector>
#include <cstdio>
#include <stdexcept>
#include "hgr.hpp"

uint8_t *readHiresFile(const char* filename);
uint8_t *readLoresFile(const char* filename);
char *rewriteExtension(const char *filename, const char *newExtension);
