#pragma once
#include <string>
#include <vector>
#include "Bitmap.h"

class BitmapHelper {
private:
	std::string bitmapFileName;
	
public:
	Bitmap bitmap;
	int totalSize;

	void saveBitmapValues(int width, int height,int bytesPerPixel, unsigned char *pixelData);
	void readBitmapImageFromFile(const char *fileName);
	void writeBitmap(const char *fileName);
};