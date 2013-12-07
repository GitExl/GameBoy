#ifndef H_RENDERLIB
#define H_RENDERLIB

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>


#define inline  __inline__ __attribute__((always_inline))

// Gameboy modes
#define MODE_GAMEBOY      0x01
#define MODE_GAMEBOY_STEP 0x02

// Toolset modes
#define MODE_TOOLSET      0x20
#define MODE_DEBUG        0x20
#define MODE_VRAM_TILES   0x21
#define MODE_VRAM_MAPS    0x22
#define MODE_VRAM_SPRITES 0x23

// Toolset colors
#define TOOLC_TEXT      RGBA(255, 255, 255, 255)
#define TOOLC_ACTIVE    RGBA(0, 127, 91, 255)
#define TOOLC_HIGHLIGHT RGBA(159, 159, 159, 255)

// Video\gameboy timing
#define VID_SYNC 59.73
#define VID_RATE 1000000 / VID_SYNC

#endif
