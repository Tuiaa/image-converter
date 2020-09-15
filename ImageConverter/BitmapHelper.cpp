#define _CRT_SECURE_NO_WARNINGS
#include "BitmapHelper.h"
#include <iostream>
#include <vector>
#include <sstream>

/*
 *		Read Bitmap Imge From File
 *		- uses seek to find correct point from bitmap file
 *		- gets all the info from bitmap
 */
void BitmapHelper::readBitmapImageFromFile(const char *fileName) {

	bitmap = Bitmap();

	FILE *imageFile = fopen(fileName, "rb");
	fseek(imageFile, 0, SEEK_SET);

	// Reading file header
	fread(&bitmap.bitmapFileHeader.headerField, 1, 2, imageFile);
	fread(&bitmap.bitmapFileHeader.filesize, 1, 4, imageFile);
	fread(&bitmap.bitmapFileHeader.reserved, 1, 4, imageFile);
	fread(&bitmap.bitmapFileHeader.dataOffset, 1, 4, imageFile);

	// Reading DIB header
	fread(&bitmap.dibHeader.infoHeaderSize, 1, 4, imageFile);
	fread(&bitmap.dibHeader.width, 1, 4, imageFile);
	fread(&bitmap.dibHeader.height, 1, 4, imageFile);
	fread(&bitmap.dibHeader.colorPlanes, 1, 2, imageFile);
	fread(&bitmap.dibHeader.bitsPerPixel, 1, 2, imageFile);
	fread(&bitmap.dibHeader.compression, 1, 4, imageFile);
	fread(&bitmap.dibHeader.imageSize, 1, 4, imageFile);
	fread(&bitmap.dibHeader.horizontalResolution, 1, 4, imageFile);
	fread(&bitmap.dibHeader.verticalResolution, 1, 4, imageFile);
	fread(&bitmap.dibHeader.colorsUsedInColorPalette, 1, 4, imageFile);
	fread(&bitmap.dibHeader.importantColors, 1, 4, imageFile);

	// Reading the pixel data
	bitmap.pixelData = new unsigned char[bitmap.dibHeader.imageSize];
	fseek(imageFile, bitmap.bitmapFileHeader.dataOffset, SEEK_SET);
	fread(bitmap.pixelData, 1, bitmap.dibHeader.imageSize, imageFile);

	fclose(imageFile);

	// Also taking the data into a vector because it's easier for me to debug with this
	for (int i = 0; i < bitmap.dibHeader.imageSize; i++) {
		bitmap.pixelDataAsIntVector.push_back(bitmap.pixelData[i]);
	}
}

/*
 *		Save Bitmap Values
 *		- makes sure the read bitmap has the required fields set
 *		- also used when wanting to create a new bitmap file after reading .dds file
 */
void BitmapHelper::saveBitmapDefaultValues() {

	// Bitmap File Header
	bitmap.bitmapFileHeader.headerField = "BM";
	bitmap.bitmapFileHeader.reserved = 0;
	bitmap.bitmapFileHeader.dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;

	// DIB Header
	bitmap.dibHeader.infoHeaderSize = INFO_HEADER_SIZE;
	bitmap.dibHeader.colorPlanes = 1;
	bitmap.dibHeader.bitsPerPixel = 24;
	bitmap.dibHeader.compression = NO_COMPRESSION;
	bitmap.dibHeader.horizontalResolution = 11811;						//300 dpi
	bitmap.dibHeader.verticalResolution = 11811;						//300 dpi
	bitmap.dibHeader.colorsUsedInColorPalette = MAX_NUMBER_OF_COLORS;
	bitmap.dibHeader.importantColors = ALL_COLORS_REQUIRED;
}

/*
 *		Save Values Read From File
 *		- saves other than default bitmap values
 *		- values have been read from other file
 */
void BitmapHelper::saveValuesReadFromFile(int width, int height) {
	bitmap.dibHeader.width = width;
	bitmap.dibHeader.height = height;
	bitmap.bitmapFileHeader.filesize = (width * height * 3) + HEADER_SIZE + INFO_HEADER_SIZE;	// times 3, because 3 color values, R, G an B
	bitmap.dibHeader.imageSize = width * height * 3;
}

/*
 *		Write Bitmap
¨*		- creates a new bitmap file
 */
void BitmapHelper::writeBitmap(const char *fileName, std::vector<int> pixelData)
{
	FILE *outputFile = fopen(fileName, "wb");

	// Writing Bitmap File Header
	fwrite(&bitmap.bitmapFileHeader.headerField[0], 1, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.headerField[1], 1, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.filesize, 4, 1, outputFile);
	fwrite(&bitmap.bitmapFileHeader.dataOffset, 4, 1, outputFile);

	// Writing DIB Header
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

	// Writing the pixel data
	for (int i = 0; i < bitmap.dibHeader.imageSize; i++)
	{
		// Writing temp data so the file will be created because decompression was not finished
		if (i >= pixelData.size() - 1)
		{
			fwrite("0", 1, 1, outputFile);
		}
		else {
			fwrite(&pixelData[i], 1, 1, outputFile);
		}
	}

	fclose(outputFile);
}