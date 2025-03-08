#pragma once

#include <vector>
#include <cstdint>

#define CHAR_NUM 256
#define CHAR_WIDTH 16
#define CELL_WIDTH 14

typedef enum {
    MODEL_II,
    MODEL_IIJPLUS,
    MODEL_IIE
} Model;

std::vector<uint8_t> hiresToGray(const std::vector<uint8_t>& hiresData);
//size_t generateHiresScanline(const std::vector<uint8_t>& hiresData, int startOffset, std::vector<uint8_t>& output, size_t outputOffset);
//std::vector<uint8_t> generateHiresData(const std::vector<uint8_t>& hiresBytes);
