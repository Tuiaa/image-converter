#pragma once
#include <vector>

// Offsets gotten from BMP table
#define DATA_OFFSET_OFFSET		0x000A
#define WIDTH_OFFSET			0x0012
#define HEIGHT_OFFSET			0x0016
#define BITS_PER_PIXEL_OFFSET	0x001C

// Default values gotten from BMP table
#define HEADER_SIZE				14
#define INFO_HEADER_SIZE		40
#define NO_COMPRESSION			0
#define MAX_NUMBER_OF_COLORS	0
#define ALL_COLORS_REQUIRED		0

/*
 *		Bitmap File Header
 *		- stores general information about the bitmap image file
 *		- it's size is 14 bytes
 */
struct BitmapFileHeader
{
	const char *headerField = "BM";	// First 2 bytes are the characters of bitmap file are "B" and "M" in ASCII
	int filesize;					// Next is the size of the bitmap in bytes
	int reserved = 0x0000;			// Next is reserved bytes, should be 0
	int dataOffset;					// Data offset, i.e. starting address of the byte where the bitmap image data (pixel array) can be found
};

/*
 *		DIB Header (bitmap informaion header)
 *		- Tells the application detailed information about the image
 *		- Immediately follows bitmap file header
 */
struct DIBHeader {
	
	int infoHeaderSize;				// The size of this header
	int width;						// Bitmap width in pixels
	int height;						// Bitmap height in pixels
	int colorPlanes = 1;			// The number of color planes, must be 1
	int bitsPerPixel = 3;			// The number of bits per pixel (red, green, blue)
	int compression;				// Possible used compression method
	int imageSize;					// The image size, the size of the raw bitmap data
	int horizontalResolution;		// The horizontal resolution of the image.
	int verticalResolution;			// The vertical resolution of the image
	int colorsUsedInColorPalette;	// Number of colors in color palette --> 0 to default
	int importantColors;			// Number of important color used --> 0 when every color is important
};

class Bitmap {
public:
	BitmapFileHeader bitmapFileHeader;
	DIBHeader dibHeader;

	// Saved so no need to calculate again (= bitsPerPixel / 8)
	int bytesPerPixel;
	
	// Array containing all the pixels from image
	unsigned char* pixelData;

	std::vector<int> pixelDataAsIntVector;
};