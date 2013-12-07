#ifndef H_SURFACE
#define H_SURFACE

#include <SDL/SDL.h>


typedef struct
{
    int32_t Width;
    int32_t Height;
    uint32_t dataLen;

    uint32_t* ptrData;
    uint32_t** ptrRows;
} Surface;


Surface*	surfaceCreate 			  (const uint32_t Width, const uint32_t Height);
void        surfaceCopyToSDL          (const Surface *srcSurf, SDL_Surface* destSurf);
void        surfaceCopyToSDLScaled    (const Surface* srcSurf, SDL_Surface* destSurf, uint32_t Scale);
void        surfaceCopyToSDLDouble    (const Surface* srcSurf, SDL_Surface* destSurf);
void        surfaceCopyToSDLQuadruple (const Surface* srcSurf, SDL_Surface* destSurf);
Surface*    surfaceReadFromPNG        (const char* fileName);
void		surfaceDestroy			  (Surface* Surf);
void		surfaceFill				  (const Surface* destSurf, const uint32_t Color);
void        surfaceExtract            (const Surface* srcSurf, const Surface* destSurf, const int32_t X, const int32_t Y);
void        surfaceSwapRG             (const Surface* Surf);
void        surfaceClear              (const Surface* Surf);

#endif
