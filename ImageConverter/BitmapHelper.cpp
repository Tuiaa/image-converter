#define _CRT_SECURE_NO_WARNINGS
#include "BitmapHelper.h"
#include <iostream>

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

void BitmapHelper::createBitmap() {
	bitmap = Bitmap();
}

void BitmapHelper::saveBitmapValues(int width, int height, int bytesPerPixel, unsigned char *pixelDataFromFile) {

	// header
	bitmap.bitmapFileHeader.filesize = height + HEADER_SIZE + INFO_HEADER_SIZE;
	bitmap.bitmapFileHeader.dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;

	//infoheader
	bitmap.dibHeader.infoHeaderSize = INFO_HEADER_SIZE;
	bitmap.dibHeader.width = width;
	bitmap.dibHeader.height = height;

	//extra
	bitmap.bytesPerPixel = bytesPerPixel;

	bitmap.dibHeader.bitsPerPixel = bytesPerPixel * 8;
	bitmap.dibHeader.compression = NO_COMPRESION;
	bitmap.dibHeader.imageSize = width * height*bytesPerPixel;
	bitmap.dibHeader.horizontalResolution = 11811; //300 dpi
	bitmap.dibHeader.verticalResolution = 11811; //300 dpi
	bitmap.dibHeader.colorsUsedInColorPalette = MAX_NUMBER_OF_COLORS;
	bitmap.dibHeader.importantColors = ALL_COLORS_REQUIRED;

	bitmap.pixelData = pixelDataFromFile;
}

void BitmapHelper::readBitmapImageFromFile(const char *fileName, int *width, int *height, int *bytesPerPixel)
{

	FILE *imageFile = fopen(fileName, "rb");	// Opens the image file

	// Seek width & height of the image and save the values
	fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
	fread(width, 4, 1, imageFile);

	std::cout << "\nsaved width: " << width;

	fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
	fread(height, 4, 1, imageFile);	// bitmpheader size of with and height in bytes is 4

	// save bits per pixel value
	// it's size in bytes is two so it has to be short
	short bitsPerPixel;
	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
	fread(&bitsPerPixel, 2, 1, imageFile);
	*bytesPerPixel = ((int)bitsPerPixel) / 8;

	size = (*height * (*width *(24 / 8)));	// size is the value of pointer height and width multplied, and that's multiplied by 24*8 (24bitmap)

	int rowSize = (*width)*(*bytesPerPixel);
	totalSize = rowSize * (*height);

	int dataOffset;
	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);	// Seek from beginning of the file (SEEK_SET)
	fread(&dataOffset, 4, 1, imageFile);

	data_pix = new unsigned char[totalSize];
	fseek(imageFile, dataOffset, SEEK_SET);
	fread(data_pix, sizeof(unsigned char), totalSize, imageFile);

	fclose(imageFile);

	//// rotate bgr to rgb
	//unsigned char tmp;
	//for (int i = 0; i < totalSize; i += 3)
	//{
	//	//tmp = data_pix[i];

	//	//data_pix[i] = data_pix[i + 2];

	//	//data_pix[i + 2] = tmp;

	//	std::cout << "\nthis pixel color value: " << (int)data_pix[i];
	//}

}

void BitmapHelper::writeBitmap(const char *fileName, int width, int height, int bytesPerPixel)
{

	FILE *outputFile = fopen(fileName, "wb");

	//*****HEADER************//
	//signature
	fwrite(&bitmap.bitmapFileHeader.BM[0], 1, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.BM[1], 1, 1, outputFile);

	//filesize
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);

	//reserved
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);

	// Data offset
	fwrite(&bitmap.bitmapFileHeader.dataOffset, 4, 1, outputFile);

	//*******INFO*HEADER******//
	//int infoHeaderSize
	fwrite(&bitmap.dibHeader.infoHeaderSize, 4, 1, outputFile);

	// width and height
	fwrite(&bitmap.dibHeader.width, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.height, 4, 1, outputFile);

	// color planes
	fwrite(&bitmap.dibHeader.colorPlanes, 2, 1, outputFile);

	// bitsperpixel
	fwrite(&bitmap.dibHeader.bitsPerPixel, 2, 1, outputFile);


	//write compression
	fwrite(&bitmap.dibHeader.compression, 4, 1, outputFile);

	//write image size(in bytes)
	fwrite(&bitmap.dibHeader.imageSize, 4, 1, outputFile);

	// resolution
	fwrite(&bitmap.dibHeader.horizontalResolution, 4, 1, outputFile);
	fwrite(&bitmap.dibHeader.verticalResolution, 4, 1, outputFile);

	// color palette
	fwrite(&bitmap.dibHeader.colorsUsedInColorPalette, 4, 1, outputFile);

	// imporant colors
	fwrite(&bitmap.dibHeader.importantColors, 4, 1, outputFile);

	for (int i = 0; i < totalSize; i++)
	{

		fwrite(&data_pix[i], 1, 1, outputFile);
	}
	fclose(outputFile);
}

void BitmapHelper::createTexelBlocksFromBitmap() {

}