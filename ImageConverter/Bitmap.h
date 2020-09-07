#pragma once

/*
 *		Bitmap File Header
 *		- stores general information about the bitmap image file
 *		- it's size is 14 bytes
 */
struct BitmapFileHeader
{
	// First 2 bytes are the characters of bitmap file are "B" and "M" in ASCII
	const char *BM = "BM";
	// Next is the size of the bitmap in bytes
	int filesize;
	// Next is reserved bytes, should be 0
	int reserved = 0x0000;
	// Data offset, i.e. starting address of the byte where the bitmap image data (pixel array) can be found
	int dataOffset;
};

/*
 *		DIB Header (bitmap informaion header)
 *		- Tells the application detailed information about the image
 *		- Immediately follows bitmap file header
 */
struct DIBHeader {
	// The size of this header
	int infoHeaderSize;
	// Bitmap width in pixels
	int width;
	// Bitmap height in pixels
	int height;

	// The number of color planes, must be 1
	int colorPlanes = 1;
	// The number of bits per pixel
	int bitsPerPixel = 3;		// red, green, blue

	//	Possible used compression method
	int compression;
	// The image size, the size of the raw bitmap data
	int imageSize;

	int horizontalResolution;
	int verticalResolution;

	// Number of colors in color palette --> 0 to default
	int colorsUsedInColorPalette;
	// Number of important color used --> 0 when every color is important
	int importantColors;
};

class Bitmap {
public:
	BitmapFileHeader bitmapFileHeader;
	DIBHeader dibHeader;

	// Saved so no need to calculate again (= bitsPerPixel / 8)
	int bytesPerPixel;
	
	// Array containing all the pixels from image
	unsigned char* pixelData;
};