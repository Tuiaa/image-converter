#define _CRT_SECURE_NO_WARNINGS
//  Bitmap.cpp
//

#include <iostream>
#include <stdio.h>

#include "BitmapTest.h"
//
//BitmapTest::BitmapTest(const char* filename) {
//	FILE* file;
//	file = fopen(filename, "rb");
//
//	std::cout << "sizeof bitmapfileheader" <<sizeof(BITMAPFILEHEADER2) << std::endl;
//
//	if (file != NULL) { // file opened
//		BITMAPFILEHEADER2 h;
//		size_t x = fread(&h, sizeof(BITMAPFILEHEADER2), 1, file); //reading the FILEHEADER
//		std::cout << "Reading file header\n";
//		std::cout << x;
//		fread(&this->ih, sizeof(BITMAPINFOHEADER2), 1, file);
//
//		fclose(file);
//	}
//
//	std::cout << "\nfh.bfoffbits: " << fh.bfOffBits;
//	std::cout << "\n fh.bfsize: " << fh.bfSize;
//}