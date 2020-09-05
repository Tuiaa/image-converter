#pragma once
#include <string>
#include <vector>
#include "Bitmap.h"

class BitmapHelper {
private:
	std::string bitmapFileName;
	

	//unsigned char info[54];
	//int width;
	//int height;
	//int size;
	//unsigned char* data_pix;
	//unsigned char* data;
public:
	Bitmap bitmap;
	void createBitmap();
	//void printBitmapFileName();
	//void loadBitmapFromFolder();
	//void readBitmapFileHeader();

	unsigned char* data_pix;
	unsigned char* data_pix_from_saved_bitmap;
	int size;
	int totalSize;

	void saveBitmapValues(int width, int height,int bytesPerPixel, unsigned char *pixelData);
	void readBitmapImageFromFile(const char *fileName, int *width, int *height, int *bytesPerPixel);
	void writeBitmap(const char *fileName, int width, int height, int bytesPerPixel);
};