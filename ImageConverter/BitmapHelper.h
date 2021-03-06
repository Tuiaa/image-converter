#pragma once
#include "Bitmap.h"

class BitmapHelper {
private:
	int totalSize;

	void saveBitmapValues(int width, int height, int bytesPerPixel, unsigned char *pixelData);
public:
	Bitmap bitmap;

	void readBitmapImageFromFile(const char *fileName);
	void writeBitmap(const char *fileName);
};