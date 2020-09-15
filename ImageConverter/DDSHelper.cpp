#define _CRT_SECURE_NO_WARNINGS
#include "DDS.h"
#include "DDSHelper.h"
#include <iostream>
#include <algorithm>
#include <vector>

/*
 *		Read DDS Image From File
 *		- reads the given .dds file
 */
void DDSHelper::readDDSImageFromFile(const char* fileName) {

	dds = DDS();

	FILE* imageFile = fopen(fileName, "rb");

	// Get the whole file size
	fseek(imageFile, 0, SEEK_END);
	long file_size = ftell(imageFile);
	fseek(imageFile, 0, SEEK_SET);

	dds.totalSizeOfTheFile = file_size;

	// Reading magic value
	fread(&dds.dwMagic, 1, 4, imageFile);

	// Reading header
	fread(&dds.header.dwSize, 1, 4, imageFile);
	fread(&dds.header.dwFlags, 1, 4, imageFile);
	fread(&dds.header.dwHeight, 1, 4, imageFile);
	fread(&dds.header.dwWidth, 1, 4, imageFile);
	fread(&dds.header.dwPitchOrLinearSize, 1, 4, imageFile);
	fread(&dds.header.dwDepth, 1, 4, imageFile);
	fread(&dds.header.dwMipMapCount, 1, 4, imageFile);
	fread(&dds.header.dwReserved1, 1, 44, imageFile);

	// Reading pixel format
	fread(&dds.header.pixelFormat.dwSize, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwFlags, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwFourCC, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwRGBBitCount, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwRBitMask, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwGBitMask, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwBBitMask, 1, 4, imageFile);
	fread(&dds.header.pixelFormat.dwABitMask, 1, 4, imageFile);

	// Continuing reading header
	fread(&dds.header.dwCaps, 1, 4, imageFile);
	fread(&dds.header.dwCaps2, 1, 4, imageFile);
	fread(&dds.header.dwCaps3, 1, 4, imageFile);
	fread(&dds.header.dwCaps4, 1, 4, imageFile);
	fread(&dds.header.dwReserved2, 1, 4, imageFile);

	// Reading header10
	fread(&dds.header10.dxgiFormat, 1, 4, imageFile);
	fread(&dds.header10.resourceDimension, 1, 4, imageFile);
	fread(&dds.header10.miscFlag, 1, 4, imageFile);
	fread(&dds.header10.arraySize, 1, 4, imageFile);
	fread(&dds.header10.miscFlags2, 1, 4, imageFile);

	// Reading image pixel data
	int imageSize = (dds.header.dwWidth * dds.header.dwHeight * 4) / 8;		// DXT1 compressed images have 4 bitsPerPixel --> (width * height * 4) is in bits/pixel so needs to be divided by 8 to get bytes
	dds.bdata = new unsigned char[imageSize];
	fread(dds.bdata, 1, imageSize, imageFile);

	// Reading end settings
	int endSettingsSize = dds.totalSizeOfTheFile - imageSize - 128 - 20;		// 128 = magic value and header size combined, 20 is header10 size
	dds.bdata2 = new unsigned char[endSettingsSize];
	fread(dds.bdata2, 1, endSettingsSize, imageFile);

	fclose(imageFile);

	// Also taking the data into a vector because it's easier for me to debug with this
	for (int i = 0; i < imageSize; i++) {
		pixelDataFromDDSImageAsVector.push_back(dds.bdata[i]);
	}
}

/*
 *		Save DDS Default Values
 *		- when creating a new .dds file, sets the values that are needed for DXT1 BC1 compressed image
 */
void DDSHelper::saveDDSDefaultValues() {

	//Adding DDS magic number default value
	dds.dwMagic = DDS_MAGIC;

	// Adding DDS_HEADER default values
	dds.header.dwSize = DDS_HEADER_DWSIZE;
	dds.header.dwFlags = (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT);

	dds.header.dwDepth = 0;
	dds.header.dwMipMapCount = 0;

	for (int i = 0; i < 11; i++) {
		dds.header.dwReserved1[i] = 0;
	}

	// Adding DDS_PIXELFORMAT default values
	dds.header.pixelFormat.dwSize = DDS_PIXELFORMAT_DWSIZE;
	dds.header.pixelFormat.dwFlags = (DDPF_FOURCC);
	dds.header.pixelFormat.dwFourCC = 827611204;		// Not sure why this was 827611204 when reading from my test dds file. DXT1 should be, if I understood correctly, 68888449 (DXT1 in ASCII)
	dds.header.pixelFormat.dwRGBBitCount = DDS_PIXELFORMAT_DWRGBBITCOUNT;
	dds.header.pixelFormat.dwRBitMask = 0x00ff0000;
	dds.header.pixelFormat.dwGBitMask = 0x0000ff00;
	dds.header.pixelFormat.dwBBitMask = 0x000000ff;
	dds.header.pixelFormat.dwABitMask = 0xff000000;

	// Continuing adding DDS_HEADER default values
	dds.header.dwCaps = DDSCAPS_TEXTURE;
	dds.header.dwCaps2 = 0;
	dds.header.dwCaps3 = 0;
	dds.header.dwCaps4 = 0;
	dds.header.dwReserved2 = 0;

	// Adding DDS_HEADER_DXT10 default values
	dds.header10.dxgiFormat = DXGI_FORMAT_BC1_UNORM;
	dds.header10.resourceDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
	dds.header10.miscFlag = 0;
	dds.header10.arraySize = 0;
	dds.header10.miscFlags2 = 0;
}

/*
 *		Save Values Read From File
 *		- saves other than default DDS values
 *		- values have been read from other file
 */
void DDSHelper::saveValuesReadFromFile(int width, int height) {
	dds.header.dwWidth = width;
	dds.header.dwHeight = height;

	unsigned long pitch = std::max(1, ((width + 3) / 4)) * 8;	// block size is 8 bytes for DXT1 format, calculation is from documentation
	dds.header.dwPitchOrLinearSize = pitch;
}

/*
 *		Write DDS File
 *		- Creates a new .dds file
 *		- Currently takes both pixel data straight from bitmap as well as the pixel data that has been through compression for testing purposes
 */
void DDSHelper::writeDDSFile(const char *fileName, std::vector<int> pixelDataAfterCompression) {

	FILE *outputFile = fopen(fileName, "wb");

	// Writing DDS Magic Number that contains four character code value 'DDS '
	fwrite(&dds.dwMagic, 4, 1, outputFile);

	// Writing DDS_HEADER
	fwrite(&dds.header.dwSize, 4, 1, outputFile);
	fwrite(&dds.header.dwFlags, 4, 1, outputFile);
	fwrite(&dds.header.dwHeight, 4, 1, outputFile);
	fwrite(&dds.header.dwWidth, 4, 1, outputFile);
	fwrite(&dds.header.dwPitchOrLinearSize, 4, 1, outputFile);
	fwrite(&dds.header.dwDepth, 4, 1, outputFile);
	fwrite(&dds.header.dwMipMapCount, 4, 1, outputFile);
	fwrite(&dds.header.dwReserved1, 44, 1, outputFile);

	// Writing DDS_PIXELFORMAT that's inside the header
	fwrite(&dds.header.pixelFormat.dwSize, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwFlags, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwFourCC, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwRGBBitCount, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwRBitMask, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwGBitMask, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwBBitMask, 4, 1, outputFile);
	fwrite(&dds.header.pixelFormat.dwABitMask, 4, 1, outputFile);

	// Continuing writing DDS_HEADER
	fwrite(&dds.header.dwCaps, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps2, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps3, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps4, 4, 1, outputFile);
	fwrite(&dds.header.dwReserved2, 4, 1, outputFile);

	// Writing DDS_HEADER_DXT10
	fwrite(&dds.header10.dxgiFormat, 4, 1, outputFile);
	fwrite(&dds.header10.resourceDimension, 4, 1, outputFile);
	fwrite(&dds.header10.miscFlag, 4, 1, outputFile);
	fwrite(&dds.header10.arraySize, 4, 1, outputFile);
	fwrite(&dds.header10.miscFlags2, 4, 1, outputFile);

	// Writing the pixel data
	for (int i = 0; i < pixelDataAfterCompression.size(); i++)
	{
		fwrite(&pixelDataAfterCompression[i], 1, 1, outputFile);
	}

	fclose(outputFile);
}