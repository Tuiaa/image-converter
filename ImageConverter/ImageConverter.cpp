// ImageConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include "BitmapTest.h"
#include "BitmapHelper.h"


#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

//unsigned char* data_pix;
//unsigned char* data_pix_from_saved_bitmap;
//int size;
//int totalSize;

//void ReadImage(const char *fileName, int *width, int *height, int *bytesPerPixel)
//{
//
//	FILE *imageFile = fopen(fileName, "rb");	// Opens the image file
//
//	// Seek width & height of the image and save the values
//	fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
//	fread(width, 4, 1, imageFile);
//
//	std::cout << "\nsaved width: " << width;
//
//	fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
//	fread(height, 4, 1, imageFile);	// bitmpheader size of with and height in bytes is 4
//
//	// save bits per pixel value
//	// it's size in bytes is two so it has to be short
//	short bitsPerPixel;
//	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
//	fread(&bitsPerPixel, 2, 1, imageFile);
//	*bytesPerPixel = ((int)bitsPerPixel) / 8;
//
//	size = (*height * (*width *(24 / 8)));	// size is the value of pointer height and width multplied, and that's multiplied by 24*8 (24bitmap)
//
//	int rowSize = (*width)*(*bytesPerPixel);
//	totalSize = rowSize * (*height);
//	//*pixels = (byte*)malloc(totalSize);
//	//*pixels = new unsigned char[size];
//
//	int dataOffset;
//	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);	// Seek from beginning of the file (SEEK_SET)
//	fread(&dataOffset, 4, 1, imageFile);
//
//	data_pix = new unsigned char[totalSize];
//	fseek(imageFile, dataOffset, SEEK_SET);
//	fread(data_pix, sizeof(unsigned char), totalSize, imageFile);
//
//	fclose(imageFile);
//
//	//// rotate bgr to rgb
//	//unsigned char tmp;
//	//for (int i = 0; i < size; i += 3)
//	//{
//	//	tmp = data_pix[i];
//
//	//	data_pix[i] = data_pix[i + 2];
//
//	//	data_pix[i + 2] = tmp;
//	//}
//}

//void WriteImage(const char *fileName, int width, int height, int bytesPerPixel)
//{
//
//	FILE *outputFile = fopen(fileName, "wb");
//
//	//*****HEADER************//
//	const char *BM = "BM";
//
//	//Bitmap bitmap = Bitmap();
//
//	fwrite(&BM[0], 1, 1, outputFile);
//	fwrite(&BM[1], 1, 1, outputFile);
//
//	int paddedRowSize = (int)(4 * ceil((float)width / 4.0f))*bytesPerPixel;
//	int fileSize = paddedRowSize * height + HEADER_SIZE + INFO_HEADER_SIZE;
//
//	//filesize
//	//int fileSize = (height * (width *(24 / 8)));
//	fwrite(&fileSize, 4, 1, outputFile);
//
//	//reserved
//	int reserved = 0x0000;
//	fwrite(&reserved, 4, 1, outputFile);
//
//	//dataoffset
//	int dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;
//	fwrite(&dataOffset, 4, 1, outputFile);
//
//	//*******INFO*HEADER******//
//	int infoHeaderSize = INFO_HEADER_SIZE;
//	fwrite(&infoHeaderSize, 4, 1, outputFile);
//
//	// width and height
//	fwrite(&width, 4, 1, outputFile);
//	fwrite(&height, 4, 1, outputFile);
//
//	// color planes
//	int planes = 1; //always 1
//	fwrite(&planes, 2, 1, outputFile);
//
//	// bitsperpixel
//	int bitsPerPixel = bytesPerPixel * 8;
//	fwrite(&bitsPerPixel, 2, 1, outputFile);
//
//
//	//write compression
//	int compression = NO_COMPRESION;
//	fwrite(&compression, 4, 1, outputFile);
//
//	//write image size(in bytes)
//	int imageSize = width * height*bytesPerPixel;
//	fwrite(&imageSize, 4, 1, outputFile);
//
//	// resolution
//	int resolutionX = 11811; //300 dpi
//	int resolutionY = 11811; //300 dpi
//	fwrite(&resolutionX, 4, 1, outputFile);
//	fwrite(&resolutionY, 4, 1, outputFile);
//
//	// color palette
//	int colorsUsed = MAX_NUMBER_OF_COLORS;
//	fwrite(&colorsUsed, 4, 1, outputFile);
//
//	// imporant colors
//	int importantColors = ALL_COLORS_REQUIRED;
//	fwrite(&importantColors, 4, 1, outputFile);
//
//	for (int i = 0; i < totalSize; i++)
//	{
//
//		fwrite(&data_pix_from_saved_bitmap[i], 1, 1, outputFile);
//	}
//	fclose(outputFile);
//}

int main()
{
	std::string inputtedFileName;
	std::cout << "Enter bitmap file name:\n";
	std::cin >> inputtedFileName;

	////inputtedFileName = "";
	//if (!inputtedFileName.empty()) {
	//	BitmapFileLoader bmpFileLoader = BitmapFileLoader(inputtedFileName);

	//	//bmpFileLoader.printBitmapFileName();
	//	bmpFileLoader.loadBitmapFromFolder();
	//}
	//else {
	//	std::cout << "Filename was not supported!";
	//}

	int width;
	int height;
	int bytesPerPixel;

	BitmapHelper bitmapHelper = BitmapHelper();
	bitmapHelper.createBitmap();

	bitmapHelper.readBitmapImageFromFile("bitmapTestImage.bmp", &width, &height, &bytesPerPixel);

	bitmapHelper.saveBitmapValues(width, height, bytesPerPixel, bitmapHelper.data_pix);

	bitmapHelper.data_pix_from_saved_bitmap = bitmapHelper.bitmap.pixelData;
	bitmapHelper.data_pix = bitmapHelper.data_pix_from_saved_bitmap;

	bitmapHelper.writeBitmap("img2.bmp", bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.bitmap.bytesPerPixel);

	std::cout << "\nPress any key to close the console\n";
	// Pauses the console until some key is pressed
	_getch();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
