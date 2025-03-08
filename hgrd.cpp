#include <vector>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <getopt.h>
#include "types.hpp"
#include "hgr.hpp"
#include "ppm.hpp"
#include "font.hpp"
#include "util.hpp"

int main(int argc, char **argv)
{
    buildHires40Font(MODEL_IIE, 0);
    //if (DEBUG) displayHires40Font();
    int opt;
    enum Mode {
        MODE_NONE,
        MODE_PPM,
        MODE_PGM
    } mode = MODE_NONE;

    static struct option long_options[] = {
        {"hgrtoppm", no_argument, 0, 'p'},
        {"hgrtopgm", no_argument, 0, 'g'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "pg", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                mode = MODE_PPM;
                break;
            case 'g': 
                mode = MODE_PGM;
                break;
            default:
                fprintf(stderr, "Usage: %s [--hgrtoppm | --hgrtopgm] <input_file>\n", argv[0]);
                exit(1);
        }
    }

    if (mode == MODE_NONE) {
        fprintf(stderr, "Must specify either --hgrtoppm or --hgrtopgm\n");
        exit(1);
    }

    if (optind >= argc) {
        fprintf(stderr, "No input file provided\n");
        exit(1);
    }

    const char *filename = argv[optind];

    try {
        std::vector<uint8_t> hiresData = readHiresFile(filename);
        printf("Successfully loaded hi-res image: %s (%zu bytes)\n", 
                filename, hiresData.size());
        
        // Process the image
        std::vector<uint8_t> graymap = hiresToGray(hiresData);

        if (mode == MODE_PGM) {
            // Generate output filenames by replacing or adding extensions
            char *pgmFilename = rewriteExtension(filename, ".pgm");
            
            // Write to image files
            writeImageToPGM(graymap, pgmFilename);            
        } else if (mode == MODE_PPM) {

            
            // Generate output filenames by replacing or adding extensions
            char *ppmFilename = rewriteExtension(filename, ".ppm");
            
            // Write to image files
            writeImageToPPM(graymap, ppmFilename);
        }

        
    } catch (const std::exception& e) {
        printf("Error processing image: %s\n", e.what());
    }
    exit(0);
}
