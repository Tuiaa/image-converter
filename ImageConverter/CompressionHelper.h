#pragma once
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <vector>

#define CHUNKSIZE = 4;

struct PixelData {
	int x;
	int y;
	unsigned char colorValue;
};

struct PixelChunk {
	std::vector<PixelData> pixelData;
};

class CompressionHelper {
public:
	
	std::vector<PixelChunk> chunks;

	int calculateHowManyChunksAreNeeded(int width, int height);

	void initializeArrayOfChunks(int amountOfChunks);
	std::vector<PixelData> initializeArrayOfPixelData(int amountOfPixelDatas);

	void populateChunks(BitmapHelper bitmapHelper);
};