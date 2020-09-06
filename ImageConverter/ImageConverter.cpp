// ImageConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include "BitmapHelper.h"
#include "CompressionHelper.h"



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

	bitmapHelper.writeBitmap("img2.bmp", bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.bitmap.bytesPerPixel);

	// COMPRESSION
	CompressionHelper compressionHelper = CompressionHelper();

	//int amountOfChunks = compressionHelper.calculateHowManyChunksAreNeeded(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height);
	//int amountOfChunks = compressionHelper.calculateHowManyChunksAreNeeded(6, 4);

	// Only works with width = 6 and height = 4
	//compressionHelper.createTestDataSet();
	// So test values can be used

	compressionHelper.initialiseSettingsForCompression(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.data_pix);
	compressionHelper.startCompression();

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
