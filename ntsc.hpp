#pragma once

#include <vector>
#include "types.hpp"
#include "Matrix3x3.hpp"

// Define types

struct ntsc_config {
    int width;
    int height;
    float colorBurst;
    float subcarrier;
    std::vector<std::vector<float>> filterCoefficients;
    float decoderOffset[3]; // TODO: first value should brightness. -1 to +1.
    Matrix3x3 decoderMatrix;
    float phaseInfo[2];
};

#define FRAME_WIDTH 560
#define FRAME_HEIGHT 192
#define NUM_TAPS 8

// Constants
const float NTSC_FSC = 3.579545e6; // NTSC colorburst frequency
const float NTSC_4FSC = 4 * NTSC_FSC;

extern ntsc_config config ;

void setupConfig();
void processAppleIIFrame(
    uint8_t *frameData,  // 560x192 bytes
    RGBA *outputImage        // Will be filled with 560x192 RGBA pixels
);
