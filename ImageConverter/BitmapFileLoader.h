#pragma once
#include <string>
#include <vector>
#include "Bitmap.h"

class BitmapFileLoader {
private:
	std::string bitmapFileName;
	Bitmap bitmap;

	//unsigned char info[54];
	//int width;
	//int height;
	//int size;
	//unsigned char* data_pix;
	//unsigned char* data;
public:
	BitmapFileLoader(std::string fileName);
	void createBitmap();
	void printBitmapFileName();
	void loadBitmapFromFolder();
	void createBitmapFile();

	void testHeaderCreation();

	void generateBitmapImage(unsigned char*, char* imageFileName);
	unsigned char* createBitmapFileHeader(int, int);
	unsigned char* createBitmapInfoHeader(int height, int width);

	
};