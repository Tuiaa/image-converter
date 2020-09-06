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
private:
	// Vector to save the pixels needed to populate one row of chunks (one chunk is 4x4 pixels)
	std::vector<int> pixelsNeededForOneRowOfChunks;
	// Vector to save pixels of one row of an image (so it basically has pixels the same number that's image's width)
	std::vector<int> pixelsOfOneRowOfAnImage;

	/*	These values are set in initialiseSettingsForCompression method	*/
	int chunkSize;								// chunkSize is 4 because BC1 should use 4x4 blocks of texels
	int totalAmountOfChunks;					// total amound of chunks in one image
	int totalAmountOfChunksHorizontally;		// how many chunks there are horizontally in total
	int totalAmountOfChunksVertically;			// how many chunks there are vertically in total
	int amountOfPixelsInARowOfChunks;			// how many pixels in total there are in a row of 4x4 chunks
	int lengthOfOneRowInPixels;					// this is the same as the width of an image

	/*	General helper values	*/
	int currentPixelData = 0;
	int currentPixelFromArray = 0;
	int currentPixelFromThisRow = 0;
	int currentRowFromChunk = 0;
	int currentChunk = 0;
	int currentRow = 0;
	int whatcurrentChunkShouldBe;

	int calculateHowManyChunksAreNeeded(int width, int height);

public:

	void initialiseSettingsForCompression(int arrayWidth, int arrayHeight);

	unsigned char* testSmallerDataPix;
	std::vector<PixelChunk> chunks;



	void initializeArrayOfChunks();
	std::vector<PixelData> initializeArrayOfPixelData(int amountOfPixelDatas);

	void createTestDataSet();

	void addChunksUsingRealValues(unsigned char* datapixelarray);
	void addFirstRow();
	void addNextRows();

	unsigned char* pixelDataCharArray;

};