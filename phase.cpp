#include <iostream>
#include <cmath>

float pixels[560] = {
    255, 255, 255, 0, 255, 0, 255, 0, 255, 0
};

void computePhaseValues() {
    const double PI = 3.14159265358979323846;
    const int width = 560;       // Width in pixels
    const double subcarrier = 3.579545 / 14.31818;  // NTSC: ~3.58MHz / sampling rate
    const double p_x = 0.908333;      // Example phase offset from phase info texture
    
    std::cout << "Pixel\tPhase (radians)\tPhase (normalized)\n";
    
    for (int pixel = 0; pixel < width; pixel++) {
        // Calculate raw phase (same as in shader)
        double phase = 2.0 * PI * (subcarrier * pixel + p_x);
        
        // Output raw phase and normalized phase (0-1)
        double normalizedPhase = phase / (2.0 * PI) - floor(phase / (2.0 * PI));
        std::cout << pixel << "\t" << phase << "\t" << normalizedPhase << std::endl;

    float luminance = pixels[pixel] / 255.0f;
    
    // Apply YIQ encoding
    float output[3];
    output[0] = luminance;                                    // Y component
    output[1] = luminance * std::sin(phase);                  // I component
    output[2] = luminance * (1.0f) * std::cos(phase); // Q component
    printf("YIQ: %f, %f, %f\n", output[0], output[1], output[2]);


        // Display only every 40 pixels to keep output manageable
        if (pixel % 40 == 0 && pixel > 0) {
            std::cout << "...\n";
            pixel += 39;  // Skip ahead (will become next pixel after loop increment)
        }
    }
}


// The core pixel function - convert an input luminance to YIQ-like representation
void processPixel(unsigned char input, float phase, float phaseAlternation, float* output) {
    // Convert input byte to normalized RGB (assuming it's luminance)
}

int main() {
    computePhaseValues();
    return 0;
}