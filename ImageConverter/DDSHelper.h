#pragma once
#include "DDS.h"
#include <vector>

class DDSHelper {
public:
	DDS dds;
	void readDDSFileFromImageLonger(const char* fileName);
	void saveDDSDefaultValues();
	void writeDDSFile(const char* fileName, unsigned char* pixelDataFromBitmap, std::vector<int> pixelDataAsVector);

	long totalSize;
	unsigned char* magicValue;
	unsigned char* readHeaderUntilReserved;
	unsigned char* readHeaderAfterReserved;
	unsigned char* readImagePixelDataAndEndSettings;
	unsigned char* readFullFile;
	unsigned char* readHeader;
	unsigned char* readHeader10;
	unsigned char* imagePixelDataMaybe;
	unsigned char* pixelData;
	unsigned char* endSettingsData;

	std::vector<int> magicValueVector;
	//unsigned char* readImagePixelDataAndEndSettings;
	std::vector<int> readFullFileVector;
	std::vector<int> readHeaderVector;
	std::vector<int> readHeader10Vector;
	std::vector<int> imagePixelDataMaybeVector;

	int endSettingsSize;
	std::vector<int> endSettingsVector;
};