#include <cstdio>   
#include "types.hpp"
#include "hgr.hpp"

uint8_t hires40Font[2 * CHAR_NUM * CHAR_WIDTH];

void displayHires40Font() {
    for (size_t i = 0; i < 2 * CHAR_NUM * CHAR_WIDTH; i++) {
        if (i % 16 == 0) {
            printf("[%03zx]:%3zu    |   ", i/16, i%16);
        }
        printf("%c ", hires40Font[i] == 0xff ? '*' : '_');
        if (i % 16 == 15) {
            printf("\n");
        }
    }
}


/** make the 'font' - i.e a predefined mapping of hires input bytes to pixel data
 * The first half of the 'font' covers 256 byte values 
 * The 2nd half covers 256 byte values if the previous byte was delayed - we draw an extra 1 bit.
 * These tables convert the 7 bits of hires input into 14 bits of "display" data. Bits are doubled
 * in order to map to the actual screen clock (2x the number of pixels) and to allow us to shift
 * the bits to the right to account for the delay (if the hi-bit of a byte is set, all the bits in that
 * are delayed by one 14M cycle.)
 * Essentially we create a signal clocked at 14M. This is required to get the right bits for simulating
 * various composite quirks in the Apple II design.
 * */

void buildHires40Font(Model model, int revision)
{
    bool delayEnabled = (((model == MODEL_II) && (revision != 0)) ||
                         (model == MODEL_IIJPLUS) ||
                         (model == MODEL_IIE));
    
    //hires40Font.resize(2 * CHAR_NUM * CHAR_WIDTH);
    
    for (int i = 0; i < 2 * CHAR_NUM; i++)
    {
        uint8_t value = (i & 0x7f) << 1 | (i >> 8);
        bool delay = delayEnabled && (i & 0x80);
        
        for (int x = 0; x < CHAR_WIDTH; x++)
        {
            bool bit = (value >> ((x + 2 - delay) >> 1)) & 0x1;
            
            hires40Font[i * CHAR_WIDTH + x] = bit ? 0xff : 0x00;
        }
    }
}
