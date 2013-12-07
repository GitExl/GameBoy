#ifndef H_UTILS
#define H_UTILS


// RGB component splitting
#define RED(rgb)    (uint8_t) rgb
#define GREEN(rgb)  (uint8_t)(rgb >> 8)
#define BLUE(rgb)   (uint8_t)(rgb >> 16)
#define ALPHA(rgb)  (uint8_t)(rgb >> 24)

// RGB color combining
#define RGB(r, g, b)     (uint32_t)r | (g << 8) | (b << 16)
#define RGBA(r, g, b, a) (uint32_t)r | (g << 8) | (b << 16) | (a << 24)
#define GRAY(v)          (uint32_t)v | (v << 8) | (v << 16)
#define GRAYA(v, a)      (uint32_t)v | (v << 8) | (v << 16) | (a << 24)

// Byte swapping
#define SWAP2(Value)    (((Value) & 0xFF) << 8) | (Value >> 8)
#define SWAP4(Value)    (((Value >> 24) & 0xFF) | ((Value >> 8) & 0xFF00) | ((Value << 8) & 0xFF0000) | ((Value << 24) & 0xFF000000))

// Math
#define MAX(a, b)    a > b ? a : b
#define MIN(a, b)    a < b ? a : b


#endif
