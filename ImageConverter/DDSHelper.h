#pragma once
#include "DDS.h"
#include <vector>

class DDSHelper {
public:
	DDS dds;

	std::vector<int> pixelDataFromDDSImageAsVector;

	void readDDSImageFromFile(const char* fileName);
	void saveDDSDefaultValues();
	void saveValuesReadFromFile(int width, int height);
	void writeDDSFile(const char* fileName, std::vector<int> pixelDataAsVector);
};