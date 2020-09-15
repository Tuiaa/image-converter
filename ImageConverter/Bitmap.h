#pragma once
#include <vector>

// Default values gotten from BMP table
#define HEADER_SIZE				14
#define INFO_HEADER_SIZE		40
#define NO_COMPRESSION			0
#define MAX_NUMBER_OF_COLORS	0
#define ALL_COLORS_REQUIRED		0
#define COLOR_PLANES			1

/*
 *		Bitmap File Header
 *		- stores general information about the bitmap image file
 *		- it's size is 14 bytes
 */
struct BitmapFileHeader
{
	const char* headerField = "BM";	// First 2 bytes are the characters of bitmap file are "B" and "M" in ASCII
	int filesize;					// Next is the size of the bitmap in bytes
	int reserved;					// Next is reserved bytes, should be 0
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
	short colorPlanes;				// The number of color planes, must be 1
	short bitsPerPixel;				// The number of bits per pixel (red, green, blue)
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
	unsigned char* pixelData;

	// Extra
	std::vector<int> pixelDataAsIntVector;
};