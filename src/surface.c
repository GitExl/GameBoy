//
// Surfaces
// A surface is always a 32-bit RGBA image.
//
#include "renderlib.h"
#include "surface.h"
#include "png.h"
#include "utils.h"


/*// Write a surface to a PNG file
//
uint32_t surfaceWriteToPNG(const Surface* Surf, const char* fileName, const uint8_t compressLevel)
{

	PNGContext* png;
	FILE* fp;
	uint32_t* imgData;
	uint32_t Y;


	if (!Surf)
		return 0;

	if (compressLevel < 1 || compressLevel > 9)
		return 0;

	png = pngCreate();
	png->Width = Surf->Width;
	png->Height = Surf->Height;
	png->colorType = PNG_COLOR_TYPE_RGB_ALPHA;
	png->bitDepth = 8;

	// Copy image data into single buffer
	imgData = malloc(png->Width * png->Height * sizeof(uint32_t));
	for (Y = 0; Y < png->Height; Y++)
	    memcpy(imgData + (Y * png->Width), Surf->ptrRows[Y], png->Width * sizeof(uint32_t));

	pngSave(png, imgData, compressLevel);

	fp = fopen(fileName, "wb");
	if (!fp)
	{
		free(imgData);
		pngClose(png);
		return 0;
	}
	if (fwrite(png->Data, 1, png->dataSize, fp) == 0)
	{
		free(imgData);
		fclose(fp);
		pngClose(png);
		return 0;
	}

	free(imgData);
	fclose(fp);
	pngClose(png);

	return 1;

}*/


// Load a surface from a PNG file
//
Surface* surfaceReadFromPNG(const char* fileName)
{

    PNGContext* png;
    Surface* Surf;
    void* imgData;
    uint32_t Col;

    int32_t X;
    int32_t Y;


    // Read PNG
    png = pngOpen(fileName);
    if (!png)
        return NULL;

    // Only 8 bits per channel is supported
    if (png->bitDepth != 8)
    {
        pngClose(png);
        return NULL;
    }

    // Create new surface
    Surf = surfaceCreate(png->Width, png->Height);
    if (!Surf)
    {
        pngClose(png);
        return NULL;
    }

    // The PNG file is greyscale, it becomes a surface with only alpha channel info
    if (png->bitDepth == 8 && png->colorType == PNG_COLOR_TYPE_GRAY)
    {
        imgData = malloc(png->Width * png->Height);
        pngRead(png, imgData);

        for(Y = 0; Y < Surf->Height; Y++)
            for(X = 0; X < Surf->Width; X++)
                Surf->ptrRows[Y][X] = *(uint8_t*)(imgData + X + Y * Surf->Width) << 24;

        free(imgData);
    }

    // The PNG file is RGB only, add an empty alpha channel
    else if (png->bitDepth == 8 && png->colorType == PNG_COLOR_TYPE_RGB)
    {
        imgData = malloc(png->Width * png->Height * 3);
        pngRead(png, imgData);

        for(Y = 0; Y < Surf->Height; Y++)
            for(X = 0; X < Surf->Width; X++)
            {
                Col = *(uint32_t*)(imgData + (X + Y * Surf->Width) * 3);
                Surf->ptrRows[Y][X] = RGBA(RED(Col), GREEN(Col), BLUE(Col), 0xFF);
            }

        free(imgData);
        surfaceSwapRG(Surf);
    }

    // The PNG file is RGBA, simply read image data into the surface
    else if (png->bitDepth == 8 && png->colorType == PNG_COLOR_TYPE_RGB_ALPHA)
    {
        pngRead(png, Surf->ptrData);
        surfaceSwapRG(Surf);
    }

    // Unsupported format
    else
    {
        pngClose(png);
        surfaceDestroy(Surf);
        return NULL;
    }

    pngClose(png);

    return Surf;

}


// Swap the red and green channels in an image
//
void surfaceSwapRG(const Surface* Surf)
{

    uint32_t* i;
    uint32_t Col;


    if (!Surf)
        return;

    for(i = Surf->ptrData; i < Surf->ptrData + (Surf->Width * Surf->Height); i++)
    {
        Col = *i;
        *i = RGBA(BLUE(Col), GREEN(Col), RED(Col), ALPHA(Col));
    }

    return;

}


// Allocate a new, empty surface
//
Surface* surfaceCreate(const uint32_t Width, const uint32_t Height)
{

    uint32_t	Row;
    Surface*	Surf;


    Surf = malloc(sizeof(Surface));
    Surf->Width   = Width;
    Surf->Height  = Height;
    Surf->dataLen = Surf->Width * Surf->Height * sizeof(uint32_t);
    Surf->ptrData = calloc(1, Surf->dataLen);
    Surf->ptrRows = malloc(Surf->Height * sizeof(uint32_t*));

    // Assign row pointers
    for (Row = 0; Row < Surf->Height; Row++)
        Surf->ptrRows[Row] = (Surf->ptrData + Row * Surf->Width);

    return Surf;

}


// Destroy a surface
//
void surfaceDestroy(Surface* Surf)
{

    if (!Surf)
        return;

    free(Surf->ptrData);
    free(Surf->ptrRows);
    free(Surf);

}


// Copy a surface to an SDL surface, scaling each pixel
//
void surfaceCopyToSDLScaled(const Surface* srcSurf, SDL_Surface* destSurf, uint32_t Scale)
{

    uint32_t   cU, cV;
    uint32_t*  Dest;

    uint32_t   cX, cY;
    uint32_t*  destRow;
    uint32_t   destWidth;


    if (!srcSurf || !destSurf)
        return;

    destWidth = destSurf->w;
    destRow = destSurf->pixels;
    cV = 0;
    Scale = 0x8000 * (Scale - 1);

    SDL_LockSurface(destSurf);

    cY = destSurf->h;
    while(cY--)
    {
        cU = 0;
        Dest = destRow;

        cX = destWidth;
        while(cX--)
        {
            *Dest++ = srcSurf->ptrRows[cV >> 16][cU >> 16];
            cU += Scale;
        }

        cV += Scale;
        destRow += destWidth;
    }

    SDL_UnlockSurface(destSurf);

}


// Copy a surface to an SDL surface
// Special unrolled version of 2x scaling
//
void surfaceCopyToSDLDouble(const Surface* srcSurf, SDL_Surface* destSurf)
{

    uint32_t   cV;
    uint32_t   cX, cY;

    uint32_t*  Src;
    uint32_t*  Dest;
    uint32_t   destWidth;


    destWidth = destSurf->w >> 1;
    Dest = destSurf->pixels;
    cV = 0;

    SDL_LockSurface(destSurf);

    cY = destSurf->h >> 1;
    while(cY--)
    {
        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            *Dest++ = *Src;
            *Dest++ = *Src;
            Src++;
        }

        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            *Dest++ = *Src;
            *Dest++ = *Src;
            Src++;
        }

        cV++;
    }

    SDL_UnlockSurface(destSurf);

}


// Copy a surface to an SDL surface
// Special unrolled version of 4x scaling
//
void surfaceCopyToSDLQuadruple(const Surface* srcSurf, SDL_Surface* destSurf)
{

    uint32_t   cV = 0;
    uint32_t   cX, cY;

    uint32_t*  Src;
    uint32_t*  Dest;
    uint32_t   destWidth;
    uint32_t   srcVal;


    destWidth = destSurf->w >> 2;
    Dest = destSurf->pixels;

    cY = destSurf->h >> 2;
    while(cY--)
    {
        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            srcVal = *Src;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            Src++;
        }

        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            srcVal = *Src;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            Src++;
        }

        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            srcVal = *Src;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            Src++;
        }

        Src = srcSurf->ptrRows[cV];
        cX = destWidth;
        while(cX--)
        {
            srcVal = *Src;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            *Dest++ = srcVal;
            Src++;
        }

        cV++;
    }

}


// Copy a surface to an SDL surface
//
void surfaceCopyToSDL(const Surface *srcSurf, SDL_Surface* destSurf)
{

    if (!srcSurf || !destSurf)
        return;

    SDL_LockSurface(destSurf);
    memcpy(destSurf->pixels, srcSurf->ptrData, srcSurf->dataLen);
    SDL_UnlockSurface(destSurf);

}


// Extract a portion of a surface onto another.
// destSurf's size determines how much is extracted from srcSurf.
//
void surfaceExtract(const Surface* srcSurf, const Surface* destSurf, const int32_t X, const int32_t Y)
{

    int32_t		Row, Len;
    uint32_t*	Src;
    uint32_t*	Dest;


    if (!srcSurf || !destSurf)
        return;

    Len = destSurf->Width << 2;
    Src = srcSurf->ptrRows[Y] + X;
    Dest = destSurf->ptrData;

    for (Row = 0; Row < destSurf->Height; Row++)
    {
        memcpy(Dest, Src, Len);
        Src += srcSurf->Width;
        Dest += destSurf->Width;
    }

}


// Fill this surface with a single color
//
void surfaceFill(const Surface* destSurf, const uint32_t Color)
{

    int32_t	Pix;


    if (!destSurf) return;

    for(Pix = 0; Pix < destSurf->Width * destSurf->Height; Pix++)
        *(destSurf->ptrData + Pix) = Color;

}


// Clear a surface
//
void surfaceClear(const Surface* Surf)
{

    if (!Surf)
        return;

    memset(Surf->ptrData, 0, Surf->dataLen);

}
