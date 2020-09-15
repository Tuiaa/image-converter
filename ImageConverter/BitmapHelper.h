#pragma once
#include "Bitmap.h"
#include <vector>

class BitmapHelper {
public:
	Bitmap bitmap;

	void readBitmapImageFromFile(const char *fileName);
	void saveBitmapDefaultValues();
	void saveValuesReadFromFile(int width, int height);
	void writeBitmap(const char *fileName, std::vector<int> compressedPixels);
};