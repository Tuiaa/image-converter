#define _CRT_SECURE_NO_WARNINGS
#include "BitmapHelper.h"
#include <iostream>

// Offsets gotten from BMP table
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C

// Default values gotten from BMP table
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

/*
 *		Read Bitmap Imge From File
 *		- uses seek to find correct point from bitmap file
 *		- gets all the info from bitmap
 */
void BitmapHelper::readBitmapImageFromFile(const char *fileName)
{
	int width;
	int height;
	int bytesPerPixel;

	FILE *imageFile = fopen(fileName, "rb");				// Opens the image file

	fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
	fread(&width, 4, 1, imageFile);							// The size is 4 bytes (value from BMP table)
	fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
	fread(&height, 4, 1, imageFile);

	// save bits per pixel value
	short bitsPerPixel;
	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
	fread(&bitsPerPixel, 2, 1, imageFile);					// it's size 2 bytes so it has to be short
	bytesPerPixel = (int)bitsPerPixel / 8;

	// Size is the value of image height and width multiplied, and that's multiplied by 24*8 (24 bitmap)
	int size = (height * (width *(24 / 8)));
	int rowSize = width * bytesPerPixel;
	totalSize = rowSize * height;

	int dataOffset;
	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(&dataOffset, 4, 1, imageFile);

	unsigned char* tempPixelArray = new unsigned char[totalSize];
	fseek(imageFile, dataOffset, SEEK_SET);
	fread(tempPixelArray, sizeof(unsigned char), totalSize, imageFile);

	fclose(imageFile);

	saveBitmapValues(width, height, bytesPerPixel, tempPixelArray);
}

/*
 *		Save Bitmap Values
 *		- takes the read values and saves them in the created bitmap object
 */
void BitmapHelper::saveBitmapValues(int width, int height, int bytesPerPixel, unsigned char *pixelDataFromFile) {

	bitmap = Bitmap();

	// Bitmap File Header
	bitmap.bitmapFileHeader.filesize = height + HEADER_SIZE + INFO_HEADER_SIZE;
	bitmap.bitmapFileHeader.dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;

	// DIB Header
	bitmap.dibHeader.infoHeaderSize = INFO_HEADER_SIZE;
	bitmap.dibHeader.width = width;
	bitmap.dibHeader.height = height;
	bitmap.dibHeader.bitsPerPixel = bytesPerPixel * 8;
	bitmap.dibHeader.compression = NO_COMPRESION;
	bitmap.dibHeader.imageSize = width * height * bytesPerPixel;
	bitmap.dibHeader.horizontalResolution = 11811;						//300 dpi
	bitmap.dibHeader.verticalResolution = 11811;						//300 dpi
	bitmap.dibHeader.colorsUsedInColorPalette = MAX_NUMBER_OF_COLORS;
	bitmap.dibHeader.importantColors = ALL_COLORS_REQUIRED;

	// General
	bitmap.bytesPerPixel = bytesPerPixel;
	bitmap.pixelData = pixelDataFromFile;
}

/*
 *		Write Bitmap
¨*		- creates a new bitmap file
 */
void BitmapHelper::writeBitmap(const char *fileName)
{
	FILE *outputFile = fopen(fileName, "wb");

	// Bitmap File Header
	fwrite(&bitmap.bitmapFileHeader.BM[0], 1, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.BM[1], 1, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.dataOffset, 4, 1, outputFile);

	// DIB Header
	fwrite(&bitmap.dibHeader.infoHeaderSize, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.width, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.height, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.colorPlanes, 2, 1, outputFile);
	fwrite(&bitmap.dibHeader.bitsPerPixel, 2, 1, outputFile);
	fwrite(&bitmap.dibHeader.compression, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.imageSize, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.horizontalResolution, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.verticalResolution, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.colorsUsedInColorPalette, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.importantColors, 4, 1, outputFile);

	// Add the pixel data
	for (int i = 0; i < totalSize; i++)
	{
		fwrite(&bitmap.pixelData[i], 1, 1, outputFile);
	}

	fclose(outputFile);
}