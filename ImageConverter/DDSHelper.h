#pragma once

class DDSHelper {
public:
	DDS dds;
	void getSurfaceInfo(size_t width, size_t height, size_t* outNumBytes, size_t* outRowBytes, size_t* outNumRows);
	void readDDSImageFromFile(const char* fileName);
	void saveDDSValues(int width, int height, int depth, int bytesPerPixel, unsigned char *pixelDataFromFile);
};