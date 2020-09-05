#pragma once
#include <string>
#include <vector>
#include "Bitmap.h"

class BitmapHelper {
private:
	std::string bitmapFileName;
	
public:
	Bitmap bitmap;
	void createBitmap();

	unsigned char* data_pix;
	//unsigned char* data_pix_from_saved_bitmap;
	int size;
	int totalSize;

	void saveBitmapValues(int width, int height,int bytesPerPixel, unsigned char *pixelData);
	void readBitmapImageFromFile(const char *fileName, int *width, int *height, int *bytesPerPixel);
	void writeBitmap(const char *fileName, int width, int height, int bytesPerPixel);

	void createTexelBlocksFromBitmap();
};