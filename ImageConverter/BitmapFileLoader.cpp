#define _CRT_SECURE_NO_WARNINGS
#include "BitmapFileLoader.h"
#include <iostream>
#include <d3d10.h>
#include <Windows.h>
#include <windowsx.h>
#include <vector>


BitmapFileLoader::BitmapFileLoader(std::string fileName) {
	bitmapFileName = fileName;
}

void BitmapFileLoader::createBitmap() {
	bitmap = Bitmap();
}

void BitmapFileLoader::printBitmapFileName() {
	std::cout << "Loaded file name: " + bitmapFileName;
}

void BitmapFileLoader::loadBitmapFromFolder() {

	bitmap = Bitmap();

	FILE* file;
	file = fopen("bitmapTestImage.bmp", "rb");

	std::cout << sizeof(BITMAPFILEHEADER) << std::endl;

	if (file != NULL) { // file opened
		//BITMAPFILEHEADER h;
		size_t x = fread(&bitmap.bitmapFileHeader, bitmap.bitmapFileHeader.sizeOfBitmapHeader, 1, file); //reading the FILEHEADER

		std::cout << x;
		fread(&this->bitmap.dibHeader, bitmap.dibHeader.sizeOfDIB, 1, file);	// reading DIB file

		fclose(file);
	}

	//std::cout << 

}

void BitmapFileLoader::readBitmapFileHeader() {

}