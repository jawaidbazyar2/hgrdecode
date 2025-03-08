#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <chrono>

#include "Matrix3x3.hpp"
#include "ppm.hpp"
#include "types.hpp"
#include "ntsc.hpp"

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <input.pgm>\n", argv[0]);
        return 1;
    }

    uint8_t *frameData = readImageFromPGMFile(argv[1]);
    if (frameData == nullptr) {
        printf("Failed to read PGM file\n");
        return 1;
    }

    RGBA *outputImage = new RGBA[config.width * config.height];

    setupConfig();

    // read nanosecond time
    auto start = std::chrono::high_resolution_clock::now();

    processAppleIIFrame(frameData, outputImage);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("Execution time: %lld nanoseconds\n", duration);

    // Write the output image to a PPM file
    writePPMFile("output.ppm", outputImage, config.width, config.height);

    // TODO: Process frameData
}
