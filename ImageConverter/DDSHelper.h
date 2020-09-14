#pragma once
#include "DDS.h"
#include <vector>

class DDSHelper {
private:
	DDS dds;

	std::vector<int> pixelDataFromDDSImageAsVector;
	std::vector<int> endSettingsVector;
public:
	
	void readDDSImageFromFile(const char* fileName);
	void saveDDSDefaultValues();
	void writeDDSFile(const char* fileName, std::vector<int> pixelDataAsVector);
};