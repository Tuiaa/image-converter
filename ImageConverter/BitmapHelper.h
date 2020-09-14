#pragma once
#include "Bitmap.h"
#include <vector>

class BitmapHelper {
private:
	int totalSize;

	void saveBitmapDefaultValues();
public:
	Bitmap bitmap;

	void readBitmapImageFromFile(const char *fileName);
	void writeBitmap(const char *fileName, std::vector<int> compressedPixels);
};