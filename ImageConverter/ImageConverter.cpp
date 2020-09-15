#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include "BitmapHelper.h"
#include "CompressionHelper.h"
#include "DDSHelper.h"

void convertAndCreateDDSFile(std::string inputtedFileNameString, std::string outputFileNameString) {

	if (!inputtedFileNameString.empty() && !outputFileNameString.empty()) {
		BitmapHelper bitmapHelper = BitmapHelper();
		DDSHelper ddsHelper = DDSHelper();

		/*	 READING	*/
		std::cout << "\n\nReading the file";
		bitmapHelper.readBitmapImageFromFile(inputtedFileNameString.c_str());

		/*	 COMPRESSION	*/
		std::cout << "\nDoing compression";
		CompressionHelper compressionHelper = CompressionHelper();
		compressionHelper.initializeSettingsForCompression(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.bitmap.pixelData, bitmapHelper.bitmap.pixelDataAsIntVector);
		std::vector<int> compressedPixels = compressionHelper.startCompression();

		/*	 SAVING		*/
		std::cout << "\nSaving as .dds";
		ddsHelper.dds = DDS();
		ddsHelper.saveDDSDefaultValues();
		ddsHelper.saveValuesReadFromFile(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height);
		ddsHelper.writeDDSFile(outputFileNameString.c_str(), compressedPixels);
	}
}

void convertAndCreateBitmapFile(std::string inputtedFileNameString, std::string outputFileNameString) {

	if (!inputtedFileNameString.empty() && !outputFileNameString.empty()) {
		BitmapHelper bitmapHelper = BitmapHelper();
		DDSHelper ddsHelper = DDSHelper();

		/*	 READING	*/
		std::cout << "\n\nReading the file";
		ddsHelper.readDDSImageFromFile(inputtedFileNameString.c_str());

		/*	 COMPRESSION	*/
		std::cout << "\nDoing decompression";
		// Didn't finish compression/decompression so for now just uses the pixel data gotten straight from dds file
		std::vector<int> decompressedPixels = ddsHelper.pixelDataFromDDSImageAsVector;

		/*	 SAVING		*/
		std::cout << "\nSaving as bitmap";
		bitmapHelper.bitmap = Bitmap();
		bitmapHelper.saveBitmapDefaultValues();
		bitmapHelper.saveValuesReadFromFile(ddsHelper.dds.header.dwWidth, ddsHelper.dds.header.dwHeight);
		bitmapHelper.writeBitmap(outputFileNameString.c_str(), decompressedPixels);
	}
}

int main()
{
	std::string inputtedFileNameString;
	std::cout << "Enter the name of the file you wish to convert (write the whole name, file extension included)\n";
	std::cin >> inputtedFileNameString;

	size_t extensionPosition;
	std::string inputtedFileNameWithoutExtension;
	std::string inputtedFileNameExtension;

	try
	{
		extensionPosition = inputtedFileNameString.find_last_of(".");
		inputtedFileNameWithoutExtension = inputtedFileNameString.substr(0, extensionPosition);
		inputtedFileNameExtension = inputtedFileNameString.substr(inputtedFileNameString.find_last_of("."));

		std::cout << "\nThe file you input was: " << inputtedFileNameWithoutExtension << inputtedFileNameExtension;

		if (inputtedFileNameExtension == ".bmp") {
			std::string outputFileNameString = inputtedFileNameWithoutExtension + "-converted" + ".dds";
			convertAndCreateDDSFile(inputtedFileNameString, outputFileNameString);

		}
		else if (inputtedFileNameExtension == ".dds") {
			std::string outputFileNameString = inputtedFileNameWithoutExtension + "-converted" + ".bmp";
			convertAndCreateBitmapFile(inputtedFileNameString, outputFileNameString);
		}
		else {
			std::cout << "\nFile was not .bmp or .dds";
		}
	}
	catch (std::out_of_range& exception)
	{
		std::cout << "\nFile was not found";
	}

	std::cout << "\nPress any key to close the console\n";
	_getch();


	//std::string inputtedFileNameString;
	//std::cout << "Enter the name of bitmap file you wish to compress (without .bmp):\n";
	//std::cin >> inputtedFileNameString;

	//std::string inputtedFileNameStringWithoutExtension = inputtedFileNameString;
	//inputtedFileNameString = inputtedFileNameString + ".bmp";
	//const char *inputtedFileName = inputtedFileNameString.c_str();

	//std::cout << "The file you input was:" << inputtedFileNameString;

	//std::string outputFileNameString = inputtedFileNameStringWithoutExtension + "_compressed.bmp";
	//const char *outputFileName = outputFileNameString.c_str();

	//if (!inputtedFileNameString.empty()) {
	//	BitmapHelper bitmapHelper = BitmapHelper();
	//	DDSHelper ddsHelper = DDSHelper();
	//	inputtedFileName = "test-bmp-file.bmp";
	//	outputFileName = "test-bmp-file-compressed.bmp";

	//	std::cout << "\n\ngoing into reading bitmap stuff";
	//	/*	 READING	*/
	//	bitmapHelper.readBitmapImageFromFile(inputtedFileName);

	//	std::cout << "\n\ngoing into compression stuff";
	//	/*	 COMPRESSION	*/
	//	CompressionHelper compressionHelper = CompressionHelper();
	//	compressionHelper.initializeSettingsForCompression(bitmapHelper.bitmap.dibHeader.width, bitmapHelper.bitmap.dibHeader.height, bitmapHelper.bitmap.pixelData, bitmapHelper.bitmap.pixelDataAsIntVector);
	//	std::vector<int> compressedPixels = compressionHelper.startCompression();

	//	std::cout << "\n\ngoing into saving stuff";
	//	/*	 SAVING		*/
	//	bitmapHelper.writeBitmap(outputFileName, compressedPixels);

	//	ddsHelper.readDDSImageFromFile("test-dxt1-dds-file.dds");
	//	ddsHelper.writeDDSFile("new-test-dxt1-dds-file.dds", compressedPixels);
	//}
	//else {
	//	std::cout << "Filename was not supported, please restart the app and try again!";
	//}
}