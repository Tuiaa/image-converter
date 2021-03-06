#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include "BitmapHelper.h"
#include "CompressionHelper.h"

int main()
{
	std::string inputtedFileNameString;
	std::cout << "Enter the name of bitmap file you wish to compress (without .bmp):\n";
	std::cin >> inputtedFileNameString;

	std::string inputtedFileNameStringWithoutExtension = inputtedFileNameString;
	inputtedFileNameString = inputtedFileNameString + ".bmp";
	const char *inputtedFileName = inputtedFileNameString.c_str();

	std::cout << "The file you input was:" << inputtedFileNameString;

	std::string outputFileNameString = inputtedFileNameStringWithoutExtension + "_compressed.bmp";
	const char *outputFileName = outputFileNameString.c_str();

	if (!inputtedFileNameString.empty()) {
		BitmapHelper bitmapHelper = BitmapHelper();

		/*	 READING	*/
		bitmapHelper.readBitmapImageFromFile(inputtedFileName);

		/*	 COMPRESSION	*/
		CompressionHelper compressionHelper = CompressionHelper();
		compressionHelper.initializeSettingsForCompression(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.bitmap.pixelData);
		compressionHelper.startCompression();

		/*	 SAVING		*/
		bitmapHelper.writeBitmap(outputFileName);
	}
	else {
		std::cout << "Filename was not supported, please restart the app and try again!";
	}

	std::cout << "\nPress any key to close the console\n";
	_getch();
}