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

	FILE* f = fopen(fileName, "rb");

	// Get the whole size
	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	totalSize = file_size;

	// Reading magic value
	fread(&dds.dwMagic, 1, 4, f);

	// Reading header
	fread(&dds.header.dwSize, 1, 4, f);
	fread(&dds.header.dwFlags, 1, 4, f);
	fread(&dds.header.dwHeight, 1, 4, f);
	fread(&dds.header.dwWidth, 1, 4, f);
	fread(&dds.header.dwPitchOrLinearSize, 1, 4, f);
	fread(&dds.header.dwDepth, 1, 4, f);
	fread(&dds.header.dwMipMapCount, 1, 4, f);
	fread(&dds.header.dwReserved1, 1, 44, f);

	// Reading pixel format
	fread(&dds.header.pixelFormat.dwSize, 1, 4, f);
	fread(&dds.header.pixelFormat.dwFlags, 1, 4, f);
	fread(&dds.header.pixelFormat.dwFourCC, 1, 4, f);
	fread(&dds.header.pixelFormat.dwRGBBitCount, 1, 4, f);
	fread(&dds.header.pixelFormat.dwRBitMask, 1, 4, f);
	fread(&dds.header.pixelFormat.dwGBitMask, 1, 4, f);
	fread(&dds.header.pixelFormat.dwBBitMask, 1, 4, f);
	fread(&dds.header.pixelFormat.dwABitMask, 1, 4, f);

	// Continuing reading header
	fread(&dds.header.dwCaps, 1, 4, f);
	fread(&dds.header.dwCaps2, 1, 4, f);
	fread(&dds.header.dwCaps3, 1, 4, f);
	fread(&dds.header.dwCaps4, 1, 4, f);
	fread(&dds.header.dwReserved2, 1, 4, f);

	// Reading header10
	fread(&dds.header10.dxgiFormat, 1, 4, f);
	fread(&dds.header10.resourceDimension, 1, 4, f);
	fread(&dds.header10.miscFlag, 1, 4, f);
	fread(&dds.header10.arraySize, 1, 4, f);
	fread(&dds.header10.miscFlags2, 1, 4, f);

	// Reading image pixel data
	int imageSize = (dds.header.dwWidth * dds.header.dwHeight * 4) / 8;		// DXT1 compressed images have 4 bitsPerPixel --> width * height * 4 is in bits/pixel so needs to be divided by 8 to get bytes
	pixelDataFromImageArray = new unsigned char[imageSize];
	fread(pixelDataFromImageArray, 1, imageSize, f);

	// Reading end settings
	int endSettingsSize = totalSize - imageSize - 128 - 20;		// 128 = magic value and header size combined, 20 is header10 size
	endSettingsData = new unsigned char[endSettingsSize];
	fread(endSettingsData, 1, endSettingsSize, f);

	fclose(f);

	for (int i = 0; i < imageSize; i++) {
		pixelDataFromDDSImageAsVector.push_back(pixelDataFromImageArray[i]);
	}
}

/*
 *		Save DDS Default Values
 *		- When creating a new .dds file, sets the values that are needed for DXT1 BC1 compressed image
 */
void DDSHelper::saveDDSDefaultValues() {

	//Adding DDS magic number default value
	dds.dwMagic = DDS_MAGIC;

	// Adding DDS_HEADER default values
	dds.header.dwSize = DDS_HEADER_DWSIZE;
	dds.header.dwFlags = (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT);
	int width2 = dds.header.dwWidth;
	int height2 = dds.header.dwHeight;
	unsigned long pitch = std::max(1, ((width2 + 3) / 4)) * 8;		// block size is 8 bytes for DXT1 format, calculation is from documentation
	dds.header.dwPitchOrLinearSize = pitch;
	dds.header.dwDepth = 0;
	dds.header.dwMipMapCount = 0;
	for (int i = 0; i < 11; i++) {
		dds.header.dwReserved1[i] = 0;
	}

	// Adding DDS_PIXELFORMAT default values
	dds.header.pixelFormat.dwSize = DDS_PIXELFORMAT_DWSIZE;
	dds.header.pixelFormat.dwFlags = (DDPF_FOURCC);

	dds.header.pixelFormat.dwFourCC = 68888449;		// // Not sure why this was 827611204 when reading from my test dds file. DXT1 should be afaik 68888449
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
 *		Write DDS File
 *		- Creates a new .dds file
 *		- Currently takes both pixel data straight from bitmap as well as the pixel data that has been through compression for testing purposes
 */
void DDSHelper::writeDDSFile(const char *fileName, unsigned char* pixelDataStraightFromBitmap, std::vector<int> pixelDataAfterCompression)
{
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

	/* Uncomment this if want to use pixels that have been through compression */
	for (int i = 0; i < pixelDataAfterCompression.size(); i++)
	{
		fwrite(&pixelDataAfterCompression[i], 1, 1, outputFile);
	}

	/* Uncomment these if want to use pixels from bitmap without doing compression */
	//int imageSize = (dds.header.dwWidth * dds.header.dwHeight * 4) / 8;		// DXT1 compressed images have 4 bitsPerPixel --> width * height * 4 is in bits/pixel so needs to be divided by 8 to get bytes
	//fwrite(pixelDataFromImageArray, imageSize, 1, outputFile);

	/* Uncomment this if want to use pixels that have been saved from read .dds file */
	//for (int i = 0; i < pixelDataFromDDSImageAsVector.size(); i++)
	//{
	//	fwrite(&pixelDataFromDDSImageAsVector[i], 1, 1, outputFile);
	//}

	// Currently not using end settings, because I'm not sure I understood what those are
	//fwrite(&endSettingsVector, endSettingsSize, 1, outputFile);

	fclose(outputFile);
}