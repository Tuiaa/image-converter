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

struct HEADERBMP {

};

unsigned char info[54];
int width;
int height;
int size;
unsigned char* data_pix;
unsigned char* data;

void BitmapFileLoader::loadBitmapFromFolder() {

	createBitmap();

	FILE* file;
	file = fopen("bitmapTestImage.bmp", "rb");

	//unsigned char info[54];


	// read the 54-byte header
	if (file != NULL) {
		fread(info, sizeof(unsigned char), 54, file);

		// extract image height and width from header

		//int width = *(int*)&info[18];
		//width = *(int*)&info[18];
		width = 400;

		std::cout << "\nwidth from header: " << width;

		//int height = *(int*)&info[22];
		//height = *(int*)&info[22];
		height = 120;

		// allocate 3 bytes per pixel

		//int size = (height * (width * (24 / 8)));
		size = (height * (width * (24 / 8)));

		//unsigned char* data_pix = new unsigned char[size];
		data_pix = new unsigned char[size];

		//unsigned char* data = new unsigned char[size];
		data = new unsigned char[size];

		// read the rest of the data at once

		fread(data_pix, sizeof(unsigned char), size, file);

		fclose(file);

		unsigned char tmp;

		/*for (int j = 0; j < size; j++) {
			std::cout << (int)data_pix[j];
		}*/

		//Converting BGR to RGB

		for (int i = 0; i < size; i += 3)
		{
			tmp = data_pix[i];

			data_pix[i] = data_pix[i + 2];

			data_pix[i + 2] = tmp;
		}

		//taking avg of RBG val

		/*for (j = 0; j < 76800; j++)
		{
			data[j] = ((data_rev[i] + data_rev[i + 1] + data_rev[i + 2]) / 3);

			i = i + 3;
		}*/

		createBitmapFile();

	}
}

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

//unsigned char* allocatedSpace;
/* This monster is a three dimensional vector that's representing the image */
/*std::vector<std::vector<std::vector<unsigned char>>> imagevector =
std::vector<std::vector<std::vector<unsigned char>>>
(height, std::vector<std::vector<unsigned char>>
(width, std::vector <unsigned char>(BYTES_PER_PIXEL)));*/
void BitmapFileLoader::createBitmapFile() {


	const int height2 = 120;
	const int width2 = 400;
	unsigned char image[height2][width2][BYTES_PER_PIXEL];
	//allocatedSpace = (unsigned char*)malloc(height*width*BYTES_PER_PIXEL);



	std::cout << "\nimage unsigned char: " << (int)image;
	//std::cout << "\nimagevector unsigned char: " << imagevector;

	char* imageFileName = (char*) "loadedAndThenSavedBitmap.bmp";

	/*int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			imagevector[i][j][2] = (unsigned char)(i * 255 / height);             ///red
			imagevector[i][j][1] = (unsigned char)(j * 255 / width);              ///green
			imagevector[i][j][0] = (unsigned char)((i + j) * 255 / (height + width)); ///blue
		}
	}*/
	
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			image[i][j][2] = (unsigned char)data_pix[3 * (i * width + j)];             ///red
			image[i][j][1] = (unsigned char)data_pix[3 * (i*width + j) + 1];              ///green
			image[i][j][0] = (unsigned char)data_pix[3 * (i*width + j) + 2]; ///blue
			//std::cout << "\ndata rgb value: " << (int)data_pix[3* (i * width + j)];
		}
	}

	generateBitmapImage((unsigned char*)image, imageFileName);
	printf("Image generated!!");
}


void BitmapFileLoader::generateBitmapImage(unsigned char *image3, char* imageFileName3)
{

	int widthInBytes = width * BYTES_PER_PIXEL;

	unsigned char padding[3] = { 0, 0, 0 };
	int paddingSize = (4 - (widthInBytes) % 4) % 4;

	int stride = (widthInBytes)+paddingSize;

	FILE* imageFile = fopen(imageFileName3, "wb");

	unsigned char* fileHeader = createBitmapFileHeader(height, stride);
	fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

	unsigned char* infoHeader = createBitmapInfoHeader(height, width);
	fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

	int i;
	for (i = 0; i < height; i++) {
		fwrite(image3 + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
		fwrite(padding, 1, paddingSize, imageFile);
	}

	fclose(imageFile);
}

unsigned char* BitmapFileLoader::createBitmapFileHeader(int height4, int stride4)
{
	int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride4 * height4);

	static unsigned char fileHeader[] = {
		0,0,     /// signature
		0,0,0,0, /// image file size in bytes
		0,0,0,0, /// reserved
		0,0,0,0, /// start of pixel array
	};

	fileHeader[0] = (unsigned char)('B');
	fileHeader[1] = (unsigned char)('M');
	fileHeader[2] = (unsigned char)(fileSize);
	fileHeader[3] = (unsigned char)(fileSize >> 8);
	fileHeader[4] = (unsigned char)(fileSize >> 16);
	fileHeader[5] = (unsigned char)(fileSize >> 24);
	fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

	return fileHeader;
}

unsigned char* BitmapFileLoader::createBitmapInfoHeader(int height5, int width5)
{
	static unsigned char infoHeader[] = {
		0,0,0,0, /// header size
		0,0,0,0, /// image width
		0,0,0,0, /// image height
		0,0,     /// number of color planes
		0,0,     /// bits per pixel
		0,0,0,0, /// compression
		0,0,0,0, /// image size
		0,0,0,0, /// horizontal resolution
		0,0,0,0, /// vertical resolution
		0,0,0,0, /// colors in color table
		0,0,0,0, /// important color count
	};

	infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
	infoHeader[4] = (unsigned char)(width5);
	infoHeader[5] = (unsigned char)(width5 >> 8);
	infoHeader[6] = (unsigned char)(width5 >> 16);
	infoHeader[7] = (unsigned char)(width5 >> 24);
	infoHeader[8] = (unsigned char)(height5);
	infoHeader[9] = (unsigned char)(height5 >> 8);
	infoHeader[10] = (unsigned char)(height5 >> 16);
	infoHeader[11] = (unsigned char)(height5 >> 24);
	infoHeader[12] = (unsigned char)(1);
	infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL * 8);

	return infoHeader;
}