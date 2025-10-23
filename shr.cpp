#include "shr.hpp"

/* shrData is location of shr data in memory */

// Convert 12-bit color value to 24-bit color value
inline RGBA convert12bitTo24bit(SHRColor c12) {
    RGBA c24;

    // Expand each 4-bit component to 8 bits
    // By duplicating the 4 bits to fill the 8-bit space
    c24.r = (c12.r << 4) | c12.r;
    c24.g = (c12.g << 4) | c12.g;
    c24.b = (c12.b << 4) | c12.b;
    c24.a = 255;
    // Combine into 24-bit color value
    return c24;
}

template<int pixelno>
inline uint8_t pixel640(uint8_t pval) {
    return (pval & (0b11 << (pixelno*2)))>>(pixelno*2);
}

template<int pixelno>
inline uint8_t pixel320(uint8_t pval) {
    return (pval & (0b1111 << (pixelno*4)))>>(pixelno*4);
}

void shrToPpm(SHR *shrPic, RGBA *ppmData) {
    // Process each scanline (0-191)
    RGBA *ppmLoc = ppmData;

    for (int y = 0; y < 200; y++) {
        // Get the starting memory offset for this scanline
        // 160 bytes per scanline
        SHRMode mode = shrPic->modes[y];
        uint8_t p_num = mode.p;

        if (mode.mode640) { // each byte is 4 pixels
            for (int x = 0; x < 160; x++) {
                uint8_t pval = shrPic->pixels[y * 160 + x];

                SHRColor pind = shrPic->palettes[p_num].colors[pixel640<3>(pval) + 0x8];
                *ppmLoc++ = convert12bitTo24bit(pind);
                
                pind = shrPic->palettes[p_num].colors[pixel640<2>(pval) + 0x0C];
                *ppmLoc++ = convert12bitTo24bit(pind);

                pind = shrPic->palettes[p_num].colors[pixel640<1>(pval) + 0x00];
                *ppmLoc++ = convert12bitTo24bit(pind);

                pind = shrPic->palettes[p_num].colors[pixel640<0>(pval) + 0x04];
                *ppmLoc++ = convert12bitTo24bit(pind);
            }
        } else {
            for (int x = 0; x < 160; x++) { // each byte is 2 pixels
                uint8_t pval = shrPic->pixels[y * 160 + x];

                SHRColor pind = shrPic->palettes[p_num].colors[pixel320<1>(pval)];
                RGBA xx = convert12bitTo24bit(pind);
                *ppmLoc++ = xx;
                *ppmLoc++ = xx;

                pind = shrPic->palettes[p_num].colors[pixel320<0>(pval)];
                xx = convert12bitTo24bit(pind);
                *ppmLoc++ = xx;
                *ppmLoc++ = xx;
            }
        }

        // Process this scanline directly
    }
}