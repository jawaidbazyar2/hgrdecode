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

#include <vector>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <getopt.h>
#include <cstring>
#include <chrono>
#include "types.hpp"
#include "dhgr.hpp"
#include "hgr.hpp"
#include "lgr.hpp"
#include "ppm.hpp"
#include "font.hpp"
#include "util.hpp"
#include "ntsc.hpp"

int main(int argc, char **argv)
{
    buildHires40Font(MODEL_IIE, 0);
    //if (DEBUG) displayHires40Font();
    int opt;
    enum Input_Mode {
        IN_NONE,
        IN_HGR,
        IN_LGR,
        IN_DHGR
    } input_mode = IN_NONE;
    enum Output_Mode {
        OUT_NONE,
        OUT_PPM,
        OUT_PGM
    } output_mode = OUT_NONE;

    static struct option long_options[] = {
        {"hgr", no_argument, 0, 'h'},
        {"dhgr", no_argument, 0, 'd'},
        {"lgr", no_argument, 0, 'l'},
        {"ppm", no_argument, 0, 'p'},
        {"pgm", no_argument, 0, 'g'},
        {0, 0, 0, 0}
    };

    // default renderer settings
    float saturation = 1.0f;

    while ((opt = getopt_long(argc, argv, "hlpgs:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                output_mode = OUT_PPM;
                break;
            case 'g': 
                output_mode = OUT_PGM;
                break;
            case 'l':
                input_mode = IN_LGR;
                break;
            case 'h':
                input_mode = IN_HGR;
                break;
            case 'd':
                input_mode = IN_DHGR;
                break;
            case 's': {
                float value = atof(optarg);
                if (value < 0.0f || value > 1.0f) {
                    fprintf(stderr, "Saturation must be between 0.0 and 1.0\n");
                    exit(1);
                }
                saturation = value;
                break;
            }
            default:
                fprintf(stderr, "Usage: %s [-s <saturation>] [--hgr | --lgr | --ppm | --pgm] <input_file>\n", argv[0]);
                exit(1);
        }
    }

    if (input_mode == IN_NONE) {
        fprintf(stderr, "Must specify either --hgr or --lgr\n");
        exit(1);
    }
    if (output_mode == OUT_NONE) {
        fprintf(stderr, "Must specify either --ppm or --pgm\n");
        exit(1);
    }

    if (optind >= argc) {
        fprintf(stderr, "No input file provided\n");
        exit(1);
    }

    const char *filename = argv[optind];

    try {
        uint8_t *graymap = nullptr;

        if (input_mode == IN_LGR) { 
            uint8_t *hiresData = readLoresFile(filename);
            printf("Successfully loaded lo-res image: %s\n", filename);
            
            // Process the image
            graymap = loresToGray(hiresData);
        } else if (input_mode == IN_HGR) {

            uint8_t *hiresData = readHiresFile(filename);
            printf("Successfully loaded hi-res image: %s\n", filename);
            
            // Process the image
            graymap = hiresToGray(hiresData);
        } else if (input_mode == IN_DHGR) {
            uint8_t *hiresData = readdHiresFile(filename);
            printf("Successfully loaded dh-res image: %s\n", filename);
            graymap = dhiresToGray(hiresData);
        }

        if (output_mode == OUT_PGM) {   
            // Generate output filenames by replacing or adding extensions
            char *pgmFilename = rewriteExtension(filename, ".pgm");
            
            // Write to image files
            writeImageToPGM(graymap, pgmFilename);            
        } else if (output_mode == OUT_PPM) {

            setupConfig(saturation);
            if (input_mode == IN_DHGR) {
                config.phaseInfo[0] = 0.25f; // adjust phase because DHGR lines start at an odd offset compared to standard HGR.
            }
            RGBA *outputImage = new RGBA[config.width * config.height];

            // read nanosecond time
            auto start = std::chrono::high_resolution_clock::now();

            processAppleIIFrame(graymap, outputImage);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            printf("Execution time: %lld nanoseconds\n", duration);

            // Generate output filenames by replacing or adding extensions
            char *ppmFilename = rewriteExtension(filename, ".ppm");
            
            // Write to image files
            writePPMFile(ppmFilename, outputImage, 560, 192 );
        }
        
    } catch (const std::exception& e) {
        printf("Error processing image: %s\n", e.what());
    }
    exit(0);
}
