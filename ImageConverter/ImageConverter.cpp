#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include "BitmapHelper.h"
#include "CompressionHelper.h"
#include "DDSHelper.h"

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
		DDSHelper ddsHelper = DDSHelper();
		inputtedFileName = "test-bmp-file.bmp";
		outputFileName = "test-bmp-file-compressed.bmp";

		std::cout << "\n\ngoing into reading bitmap stuff";
		/*	 READING	*/
		bitmapHelper.readBitmapImageFromFile(inputtedFileName);

		std::cout << "\n\ngoing into compression stuff";
		/*	 COMPRESSION	*/
		CompressionHelper compressionHelper = CompressionHelper();
		compressionHelper.initializeSettingsForCompression(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height,bitmapHelper.bitmap.RGBPixelData, bitmapHelper.bitmap.pixelData);
		std::vector<int> compressedPixels = compressionHelper.startCompression();

		std::cout << "\n\ngoing into saving stuff";
		/*	 SAVING		*/
		bitmapHelper.writeBitmap(outputFileName, compressedPixels);

		// TODO: save bitmap values in dds helper --> try to create a dds file from bitmap data

		//ddsHelper.readDDSImageFromFile("test-dxt1-dds-file.dds");
		ddsHelper.readDDSFileFromImageLonger("test-dxt1-dds-file.dds");
		//ddsHelper.saveDDSValues(400, 120, bitmapHelper.bitmap.pixelData);
		ddsHelper.writeDDSFile("new-test-dxt1-dds-file.dds", bitmapHelper.bitmap.pixelData, compressedPixels);
	}
	else {
		std::cout << "Filename was not supported, please restart the app and try again!";
	}

	std::cout << "\nPress any key to close the console\n";
	_getch();
}