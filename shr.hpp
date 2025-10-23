#pragma once

#include <cstdint>
#include "types.hpp"

struct SHRColor {
    union {
        struct {
            uint8_t b:4; 
            uint8_t g:4;
            uint8_t r:4;
            uint8_t _x:4;
        };
        uint16_t v;
    };
};

struct SHRMode {
    union {
        struct {
            uint8_t p:4;
            uint8_t _unused:1;
            uint8_t fill:1;
            uint8_t interrupt:1;
            uint8_t mode640:1;
        };
        uint8_t v;
    };
};

struct Palette {
    SHRColor colors[16];
};

struct SHR {
    uint8_t pixels[32000];
    SHRMode modes[200];
    uint8_t unused1[56];
    Palette palettes[16];
};

void shrToPpm(SHR *shrPic, RGBA *grayData);
