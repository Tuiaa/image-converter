#pragma once
#include "DDS.h"
#include <vector>

class DDSHelper {
public:
	DDS dds;
	void readDDSImageFromFile(const char* fileName);
	void saveDDSDefaultValues();
	void writeDDSFile(const char* fileName, unsigned char* pixelDataFromBitmap, std::vector<int> pixelDataAsVector);

	long totalSize;
	unsigned char* magicValue;
	unsigned char* readHeaderUntilReserved;
	unsigned char* readHeaderAfterReserved;
	unsigned char* readImagePixelDataAndEndSettings;
	unsigned char* readHeader;
	unsigned char* readHeader10;
	unsigned char* pixelDataFromImageArray;
	unsigned char* pixelData;
	unsigned char* endSettingsData;

	std::vector<int> magicValueVector;
	std::vector<int> readFullFileVector;
	std::vector<int> readHeaderVector;
	std::vector<int> readHeader10Vector;
	std::vector<int> pixelDataFromDDSImageAsVector;
	std::vector<int> endSettingsVector;
};