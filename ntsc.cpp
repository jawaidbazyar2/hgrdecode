#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include "Matrix3x3.hpp"
#include "ppm.hpp"
#include "types.hpp"
#include "ntsc.hpp"

ntsc_config config ;

// Function to generate phase information for a scanline and stuff in config.
void generatePhaseInfo(int scanlineY, float colorBurst) {
    //ph_info phaseInfo;
    
    // Normalize the color burst phase (similar to the original code)
    float c = colorBurst / (2 * M_PI);
    config.phaseInfo[0] = c - std::floor(c);
    
    // Set phase alternation (in original code this is a boolean array, 
    // but we simplify to alternate based on scanline)
    config.phaseInfo[1] = 0.0f; //(scanlineY % 2) ? 1.0f : -0.0f;

}

// The core pixel function - convert an input luminance to YIQ-like representation
inline void processPixel(uint8_t input, float phase, float phaseAlternation, float* output) {
    // Normalize input byte (0-255 luma) to 0-1 luma.
    float luminance = input / 255.0f;
#if 0    
    // Apply YIQ encoding
    output[0] = luminance;                                    // Y component
    output[1] = luminance * std::sin(phase);                  // I component
    output[2] = luminance * (1.0f - 2.0f * phaseAlternation) * std::cos(phase); // Q component
#else
    output[0] = luminance;
    output[1] = luminance * std::sin(phase);
    output[2] = luminance * std::cos(phase);
#endif
}

// Apply FIR filter to YIQ components - completely revised implementation
inline void applyFilter(//const std::vector<std::vector<float>>& coefficients, 
                const std::vector<float>& yiqBuffer,
                int x, int width,
                float* output) {
    // Initialize output to zero
    output[0] = output[1] = output[2] = 0.0f;
    
    // Apply center tap (c0)
    const float* center = &yiqBuffer[x * 3];
    output[0] += center[0] * config.filterCoefficients[0][0];  // Y
    output[1] += center[1] * config.filterCoefficients[0][1];  // I
    output[2] += center[2] * config.filterCoefficients[0][2];  // Q
    
    // Apply symmetric taps (c1 through c8)
    for (int i = 1; i <= NUM_TAPS; i++) {
        // Safely access left and right neighbors with bounds checking
        int leftIdx = x - i;
        int rightIdx = x + i;
        
        // Apply left neighbor if in bounds
        if (leftIdx >= 0) {
            const float* left = &yiqBuffer[leftIdx * 3];
            output[0] += left[0] * config.filterCoefficients[i][0];  // Y
            output[1] += left[1] * config.filterCoefficients[i][1];  // I
            output[2] += left[2] * config.filterCoefficients[i][2];  // Q
        }
        
        // Apply right neighbor if in bounds
        if (rightIdx < width) {
            const float* right = &yiqBuffer[rightIdx * 3];
            output[0] += right[0] * config.filterCoefficients[i][0];  // Y
            output[1] += right[1] * config.filterCoefficients[i][1];  // I
            output[2] += right[2] * config.filterCoefficients[i][2];  // Q
        }
    }
}

// TODO: see if this is correct and is the source of the transposed row/col wise matrices issue.
// it's a different implementation than above. 
// Function to convert YIQ to RGB using a decoding matrix
inline void yiqToRgb(const float yiq[3], /* const Matrix3x3& matrix,  */ /* const float offset[3], */ RGBA &rgba) {
    float rgb[3];
    
    // Apply matrix and offset (matrix is 3x3, stored in row-major format)
    rgb[0] = config.decoderMatrix[0] * yiq[0] + config.decoderMatrix[1] * yiq[1] + config.decoderMatrix[2] * yiq[2] /* + offset[0] */;
    rgb[1] = config.decoderMatrix[3] * yiq[0] + config.decoderMatrix[4] * yiq[1] + config.decoderMatrix[5] * yiq[2] /* + offset[1] */;
    rgb[2] = config.decoderMatrix[6] * yiq[0] + config.decoderMatrix[7] * yiq[1] + config.decoderMatrix[8] * yiq[2] /* + offset[2] */;
    
    // Clamp values and convert to 8-bit
    rgba.r = std::clamp(static_cast<int>(rgb[0] * 255), 0, 255);
    rgba.g = std::clamp(static_cast<int>(rgb[1] * 255), 0, 255);
    rgba.b = std::clamp(static_cast<int>(rgb[2] * 255), 0, 255);
    rgba.a = 255;
}

// Function to display YIQ buffer values
void displayYIQBuffer(const std::vector<float>& yiqBuffer, int width, int startX, int count) {
    printf("YIQ Buffer Values (x=%d to %d):\n", startX, startX + count - 1);
    printf("   X   |     Y     |     I     |     Q     \n");
    printf("-------+-----------+-----------+-----------\n");
    
    for (int i = 0; i < count; i++) {
        int x = startX + i;
        if (x >= width) break;
        
        printf(" %5d | %9.6f | %9.6f | %9.6f\n", 
               x, 
               yiqBuffer[x * 3],     // Y
               yiqBuffer[x * 3 + 1], // I
               yiqBuffer[x * 3 + 2]  // Q
        );
    }
    printf("\n");
}

// Process a single scanline of Apple II video data
void processAppleIIScanline(
    uint8_t *inputScanline,  // Input 560 bytes of luminance data
    RGBA *outputScanline,         // Output 560 RGBA pixels
    int scanlineY                             // Current scanline index
) {

    // Generate phase information for this scanline - should tweak for every other scanline
    //std::vector<float> phaseInfo = generatePhaseInfo(scanlineY, config.colorBurst);
    
    // Create temporary buffer for YIQ components
    std::vector<float> yiqBuffer(config.width * 3);  // 3 components per pixel (Y, I, Q)
    
    // First pass: Convert input to YIQ representation
    for (int x = 0; x < config.width; x++) {
        // Calculate phase for this pixel. Should use addition instead of all these multiplies.
        float phase = 2.0f * M_PI * (config.subcarrier * x + config.phaseInfo[0]);
        //printf("scanlineY: %d, x: %d, phase: %.6f\n", scanlineY, x, phase);
        // Process pixel to YIQ
        float* yiq = &yiqBuffer[x * 3];
        processPixel(inputScanline[x], phase, config.phaseInfo[1], yiq);
    }
    
    // Display YIQ buffer values for the first few pixels if this is the first scanline
#if 0
    if (scanlineY == 0) {
        displayYIQBuffer(yiqBuffer, config.width, 0, 10);  // Show first 10 pixels
    }
#endif
    
    // Second pass: Apply FIR filter to each pixel
    std::vector<float> filteredYiq(3);  // Temporary storage for filtered YIQ
    
    for (int x = 0; x < config.width; x++) {
        // Apply filter using the whole buffer
        applyFilter(/* config.filterCoefficients, */ yiqBuffer, x, config.width, filteredYiq.data());
        
        // Convert to RGB
        yiqToRgb(filteredYiq.data(), /* config.decoderMatrix, */ /* config.decoderOffset, */ outputScanline[x]);
    }
}

void processAppleIIFrame(
    uint8_t *frameData,  // 560x192 bytes
    RGBA *outputImage        // Will be filled with 560x192 RGBA pixels
    //ntsc_config &config
) {

#if 0
        // Set up YIQ to RGB conversion matrix and offset
        // This is a standard NTSC YIQ to RGB conversion matrix
        Matrix3x3 decoderMatrix(
                    1.0f,  0.956f,  0.619f,
                    1.0f, -0.272f, -0.647f,
                    1.0f, -1.106f,  1.703f
                    );
#else
        // TODO: multiple decoderMatrix by contrast which is specified, and divided by (imageWhiteLevel / imageBlackLevel).

        // identity matrix?
        Matrix3x3 decoderMatrix(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);
        
        Matrix3x3 yiqMatrix(
            1, 0,        1.13983,
            1, -0.39465, -0.58060,
            1, 2.03206,  0
        );

        // saturation should be 0 to 1.0 
        // 0 is basically monochrome. transposing row/col here does not matter.
        float videoSaturation = 1.0f;
        Matrix3x3 saturationMatrix(
            1, 0,               0,
            0, videoSaturation, 0,
            0, 0,                videoSaturation
        );
        // if hue is 0, then hueMatrix becomes identity matrix. (transposing r
        float videoHue = 2 * (float) M_PI * 0.05f;
        Matrix3x3 hueMatrix(
            1, 0,              0,
            0, cosf(videoHue), -sinf(videoHue),
            0, sinf(videoHue), cosf(videoHue)
        );
    
        decoderMatrix.multiply(saturationMatrix);
        decoderMatrix.multiply(hueMatrix);
        decoderMatrix.multiply(yiqMatrix);
        decoderMatrix.print();
        config.decoderMatrix = decoderMatrix;
#endif

    // Process each scanline
    for (int y = 0; y < config.height; y++) {
        // Process the scanline
         processAppleIIScanline(frameData + (y * config.width), outputImage + (y * config.width), y );
    }
}

void setupConfig() {
    // TODO: analyze and document - in apple ii land the colorburst is defined as negative 33 degrees,
    // But that shifts the colorburst the wrong way. We needed to bring it counterclockwise around the 
    // colorwheel.
    //float colorBurst = 2.0 * M_PI * (-33.0 / 360.0 + (imageLeft % 4) / 4.0);
    float colorBurst = (2.0 * M_PI * (33.0 / 360.0 )); // plus 0.25, 0.5 0.75, if imageLeft % 4 is 1 2 3
    float subcarrier = NTSC_FSC / NTSC_4FSC; /* 0.25, basically constant here */
    printf("ColorBurst: %.6f\n", colorBurst);
    printf("Subcarrier: %.6f\n", subcarrier);

    //config.phaseInfo = generatePhaseInfo(/* scanlineY */ 0, colorBurst);
    printf("Phase Info:\n");
    printf("Phase[0]: %.6f\n", config.phaseInfo[0]);
    printf("Phase[1]: %.6f\n", config.phaseInfo[1]);

   config = {
        .width = FRAME_WIDTH,
        .height = FRAME_HEIGHT,
        .colorBurst = colorBurst,
        .subcarrier = subcarrier,
            // Set up filter coefficients based on measured YIQ filter response
        .filterCoefficients = {
            {0.279408f, 0.374189f, 0.260298f},  // c0
            {0.235926f, 0.341248f, 0.247879f},  // c1
            {0.134616f, 0.256477f, 0.213725f},  // c2
            {0.036655f, 0.153548f, 0.166016f},  // c3
            {-0.015383f, 0.066575f, 0.115090f}, // c4
            {-0.022104f, 0.013702f, 0.070082f}, // c5
            {-0.009993f, -0.006718f, 0.036478f},// c6
            {-0.000718f, -0.007999f, 0.015433f},// c7
            {0.001297f, -0.003928f, 0.005148f}  // c8
        },
        .decoderOffset = {0.0f, 0.0f, 0.0f} // TODO: first value should brightness. -1 to +1.
    };
    generatePhaseInfo(/* scanlineY */ 0, colorBurst);
}

