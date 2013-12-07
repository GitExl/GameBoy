#ifndef H_PNG
#define H_PNG

#include <png.h>


// Single PNG chunk
typedef struct
{
    char*     Name;
    uint32_t  Size;
    void*     Data;

    void*     Next;
} PNGChunk;

// PNG decode\encode context
typedef struct
{

    // libpng pointers
    png_structp pngPtr;
    png_infop infoPtr;

    // Opened for writing
    uint8_t Write;

    // Basic info
    uint32_t Width;
    uint32_t Height;
    uint32_t bitDepth;
    uint32_t colorType;

    // Linked list of chunks
    PNGChunk* Chunks;

} PNGContext;


void        pngClose    (PNGContext* Ctx);
PNGContext* pngOpen     (const char* fileName);
PNGContext* pngCreate   ();
uint32_t    pngAddChunk (PNGContext* PNG, const char* Name, const uint32_t Size, const void* Data);
uint32_t    pngRead     (PNGContext* Ctx, void* Dest);
uint32_t    pngSave     (PNGContext* PNG, const void* Src, const uint8_t compressLevel);

#endif
