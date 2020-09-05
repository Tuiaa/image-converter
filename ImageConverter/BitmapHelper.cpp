#define _CRT_SECURE_NO_WARNINGS
#include "BitmapHelper.h"
#include <iostream>

void BitmapHelper::createBitmap() {
	bitmap = Bitmap();
}

void BitmapHelper::saveBitmapValues(int width, int height, int bytesPerPixel, unsigned char *pixelDataFromFile) {
	bitmap.dibHeader.width = width;
	bitmap.dibHeader.height = height;
	bitmap.bytesPerPixel = bytesPerPixel;
	bitmap.dibHeader.bitsPerPixel = bytesPerPixel * 8;
	bitmap.pixelData = pixelDataFromFile;
}