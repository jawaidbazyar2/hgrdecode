# HGRD - Apple II Hi-Res to NTSC Decoder

## Overview

HGRD is a tool that decodes Apple II hi-res graphics (HGR) to NTSC video. It can output either a PPM (color) image or a PGM (gray map) image.

There are a large number of configuration settings that are currently hard-coded. Future releases will allow control of these parameters from the CLI interface. I wanted to get the basic decoder out into the community as soon as possible.

## Building

This uses standard C++17 features and should build with any modern compiler that supports C++17. I use clang++, and that is what the Makefile uses.

There is nothing platform-specific at all in the code, so it should build and operate on: Mac, Windows, Linux.

## Usage

```
% ./hgrd [--hgr | --lgr] [--ppm | --pgm] <input_file> <output_file>
```

There are four command-line options:
   * --hgr - input file is hi-res
   * --lgr - input file is lo-res
   * --ppm - output a ppm (color) image
   * --pgm - output a pgm (gray map) image.

## Examples

```bash
./hgrd --hgr --ppm <input_file> <output_file>
``` 
Create a PPM (color) image from a hi-res image. This uses a simulation of the Apple II bit-stream output and an NTSC decoder to display an Apple II hi-res image the way it would have looked on an old composite video monitor.

```bash
./hgrd --lgr --pgm <input_file> <output_file>
```
Create a PGM (gray map) image from a lo-res image. This is essentially a bare (no color decoding) image of a lo-res file. 560 dots wide by 192 dots high. This is one stage of the input to the color decoding process, put here in case you want to create "monochrome" output.

## Performance

With optimization disable (remove -O3 from the Makefile), the PPM decode (not counting file loading or writing) takes about 55-60 milliseconds. With -O3 enabled, it takes between 6ms to 8ms typically to produce one frame.

## Future Work

- Add CLI configuration options various NTSC decoding parameters.
- Drill down and implement optimizations. The ultimate goal is for this code base to be able to operate as part of an emulator pipeline - 60fps is 17ms per frame, so we need another 2-3x increase in performance for this code to work in that scenario. It may also be possible to use SIMD instructions to speed up the color decoding process. That will be a work in progress.

## Acknowledgements

This is an implementation in pure C++of the NTSC decoding pipeline using OpenGL shaders from OpenEmulator. 

[OpenEmulator](https://github.com/openemulator/openemulator)



