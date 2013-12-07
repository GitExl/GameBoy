#include "renderlib.h"
#include "png.h"


// Add a new chunk to the chunk list
//
uint32_t pngAddChunk(PNGContext* PNG, const char* Name, const uint32_t Size, const void* Data)
{
    PNGChunk* newChunk;


    if (!PNG)
        return 0;

    // Add new chunk in list
    newChunk = calloc(1, sizeof(PNGChunk));
    newChunk->Next = PNG->Chunks;
    PNG->Chunks = newChunk;

    // Copy name
    newChunk->Name = calloc(1, 5);
    memcpy(newChunk->Name, Name, 5);

    // Copy size and data
    newChunk->Size = Size;
    newChunk->Data = malloc(Size);
    memcpy(newChunk->Data, Data, Size);

    return 1;
}

// Callback for unknown chunks, store it in context chunk list
//
static int32_t pngUserReadChunk(const png_structp pngPtr, const png_unknown_chunkp Chunk)
{
    PNGContext* PNG;


    PNG = png_get_io_ptr(pngPtr);
    if (!PNG)
        return 1;

    pngAddChunk(PNG, (char*)Chunk->name, Chunk->size, Chunk->data);

    return 0;
}

// User error callback
//
static void pngUserError(const png_structp pngPtr, const png_const_charp errorMsg)
{
    return;
}

/*// Create a PNG for saving
//
PNGContext* pngCreate()
{
	PNGContext* PNG;


	// Allocate contexts
    PNG = calloc(1, sizeof(PNGContext));
	PNG->pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!PNG->pngPtr)
    {
        pngClose(PNG);
		return NULL;
    }

	PNG->infoPtr = png_create_info_struct(PNG->pngPtr);
	if (!PNG->infoPtr)
	{
		pngClose(PNG);
		return NULL;
	}

	// Start IO
	png_init_io(PNG->pngPtr, (png_FILE_p)PNG);

	// Custom error handler
	png_set_error_fn(PNG->pngPtr, NULL, NULL, pngUserError);

	// This context is for writing
	PNG->Write = 1;

	return PNG;
}

// Save an image to a PNG
//
uint32_t pngSave(PNGContext* PNG, const void* Src, const uint8_t compressLevel)
{
	uint32_t Y = 0;
	uint32_t Stride = 0;
	png_bytep* Rows;


	if (!PNG)
		return 0;

	if (!PNG->Write)
		return 0;

	if (compressLevel < 1 || compressLevel > 9)
		return 0;

	// Error handling return
	if (setjmp(png_jmpbuf(PNG->pngPtr)))
    {
		pngClose(PNG);
        return 0;
    }


	// PNG filtering and ZLIB compression
	png_set_filter(PNG->pngPtr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(PNG->pngPtr, compressLevel);

	// Set image info
	png_set_IHDR(PNG->pngPtr, PNG->infoPtr, PNG->Width, PNG->Height, PNG->bitDepth, PNG->colorType,
				 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// Pack < 8 bits per pixel
	png_set_packing(PNG->pngPtr);

	// Write header
	png_write_info(PNG->pngPtr, PNG->infoPtr);

	// Allocate memory for the row pointers
    Rows = (png_bytep*)calloc(PNG->Height, sizeof(png_bytep));

	// Point the row pointers into the destination
	Stride = png_get_rowbytes(PNG->pngPtr, PNG->infoPtr);
	for (Y = 0; Y < PNG->Height; Y++)
		Rows[Y] = (png_byte*)(Src + (Y * Stride));

	// Write image
	png_write_image(PNG->pngPtr, Rows);
	png_write_end(PNG->pngPtr, NULL);

	free(Rows);

	return 1;
}*/

// Open a PNG file and read its basic information
//
PNGContext* pngOpen(const char* fileName)
{
    PNGContext* PNG;
    FILE*       fp;
    png_byte    header[8];


    // File
    fp = fopen(fileName, "rb");
    if (!fp)
        return NULL;

    // Test for PNG signature
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
        return NULL;

    // Allocate PNG context and init libpng read structure
    PNG = calloc(1, sizeof(PNGContext));
    PNG->pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!PNG->pngPtr)
    {
        pngClose(PNG);
        return NULL;
    }

    // Set error handling return
    if (setjmp(png_jmpbuf(PNG->pngPtr)))
    {
        pngClose(PNG);
        return NULL;
    }

    // Allocate info
    PNG->infoPtr = png_create_info_struct(PNG->pngPtr);
    if (!PNG->infoPtr)
    {
        pngClose(PNG);
        return NULL;
    }

    // Start IO
    png_init_io(PNG->pngPtr, fp);
    png_set_sig_bytes(PNG->pngPtr, 8);

    // Set custom error handler
    png_set_error_fn(PNG->pngPtr, (png_voidp)NULL, (png_error_ptr)NULL, pngUserError);

    // Set unknown chunk callback
    png_set_read_user_chunk_fn(PNG->pngPtr, NULL, pngUserReadChunk);


    // Read PNG info
    png_read_info(PNG->pngPtr, PNG->infoPtr);

    // Update context with information from libpng
    PNG->Width = png_get_image_width(PNG->pngPtr, PNG->infoPtr);
    PNG->Height = png_get_image_height(PNG->pngPtr, PNG->infoPtr);
    PNG->bitDepth = png_get_bit_depth(PNG->pngPtr, PNG->infoPtr);
    PNG->colorType = png_get_color_type(PNG->pngPtr, PNG->infoPtr);

    // Strip 16 bits per channel images down to 8 bits per channel
    png_set_strip_16(PNG->pngPtr);
    if (PNG->bitDepth == 16)
        PNG->bitDepth = 8;

    // Expand 1, 2 or 4 bits pixels to 8 bits
    png_set_packing(PNG->pngPtr);

    // Expand grayscale < 8 bpp to 8 bpp
    if (PNG->colorType == PNG_COLOR_TYPE_GRAY && PNG->bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(PNG->pngPtr);

    png_read_update_info(PNG->pngPtr, PNG->infoPtr);

    return PNG;
}

// Read a PNG file's image and chunk data
//
uint32_t pngRead(PNGContext* PNG, void* Dest)
{
    uint32_t Y = 0;
    uint32_t Stride = 0;
    png_bytep* Rows;


    if (!PNG || !Dest)
        return 0;

    if (PNG->Write)
        return 0;

    // Allocate memory for the image's row pointers
    Rows = (png_bytep*)calloc(PNG->Height, sizeof(png_bytep));

    // Point the row pointers to their destinations
    Stride = png_get_rowbytes(PNG->pngPtr, PNG->infoPtr);
    for (Y = 0; Y < PNG->Height; Y++)
        Rows[Y] = (png_byte*)(Dest + (Y * Stride));

    // Set error handling return
    if (setjmp(png_jmpbuf(PNG->pngPtr)))
    {
        pngClose(PNG);
        return 0;
    }

    // Read and decompress image data
    png_read_image(PNG->pngPtr, Rows);
    png_read_end(PNG->pngPtr, NULL);
    free(Rows);

    return 1;
}

// Close a PNG context
//
void pngClose(PNGContext* PNG)
{
    PNGChunk* Chunk;
    PNGChunk* prevChunk;


    if (!PNG)
        return;

    if (PNG->Write)
        png_destroy_write_struct(&PNG->pngPtr, &PNG->infoPtr);
    else
        png_destroy_read_struct(&PNG->pngPtr, &PNG->infoPtr, NULL);

    // Free up chunks
    Chunk = PNG->Chunks;
    while(Chunk)
    {
        prevChunk = Chunk;
        Chunk = Chunk->Next;
        free(prevChunk->Name);
        free(prevChunk->Data);
        free(prevChunk);
    }

    free(PNG);
}
