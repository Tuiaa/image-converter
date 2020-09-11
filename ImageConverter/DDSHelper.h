#pragma once
#include "DDS.h"

class DDSHelper {
public:
	DDS dds;
	void getSurfaceInfo(size_t width, size_t height, size_t* outNumBytes, size_t* outRowBytes, size_t* outNumRows);
	void readDDSImageFromFile(const char* fileName);
	void saveDDSValues(int width, int height, unsigned char *pixelDataFromFile);
	void writeDDSFile(const char* fileName, unsigned char* pixelDataFromBitmap);

	long totalSize;
	unsigned char* magicValue;
	unsigned char* readImagePixelDataAndEndSettings;
	unsigned char* readFullFile;
	unsigned char* readHeader;
	unsigned char* readHeader10;
	unsigned char* imagePixelDataMaybe;
	unsigned char* pixelData;
	unsigned char* endSettingsData;
};