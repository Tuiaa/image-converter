#include "CompressionHelper.h"
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <iostream>
#include <vector>

int CompressionHelper::calculateHowManyChunksAreNeeded(int width, int height) {
	int horizontalChunks = width / 4;
	int verticalChunks = height / 4;

	int amountOfChunks = horizontalChunks * verticalChunks;

	return amountOfChunks;
}

void CompressionHelper::initializeArrayOfChunks(int amountOfChunks) {
	std::vector<PixelChunk> chunks;
	for (int i = 0; i < amountOfChunks; i++) {
		std::vector<PixelData> chunk = initializeArrayOfPixelData(16);
		PixelChunk tempPixelChunk;
		tempPixelChunk.pixelData = chunk;

		chunks.push_back(tempPixelChunk);
	}
}

std::vector<PixelData> CompressionHelper::initializeArrayOfPixelData(int amountOfPixelDatas) {
	std::vector<PixelData> pixelData;
	for (int i = 0; i < amountOfPixelDatas; i++) {
		PixelData tempPixelData;
		tempPixelData.x = 0;
		tempPixelData.y = 0;
		tempPixelData.colorValue = 0;

		pixelData.push_back(tempPixelData);
	}

	return pixelData;
}

void CompressionHelper::populateChunks(BitmapHelper bitmapHelper) {
	int currentRow = 0;
	int currentColumn = 0;

	int currentChunk = 0;

	int currentPixelInARow = 0;
	int howManySlices = bitmapHelper.bitmap.dibHeader.width / 4;
	int currentChunkEndPoint = howManySlices;

	int arrayWidth = bitmapHelper.bitmap.dibHeader.width - 1;

	// create test data set
	unsigned char* testSmallerDataPix = new unsigned char[24];
	std::fill(testSmallerDataPix, testSmallerDataPix + 24, 5);

	for (int y = 0; y < 24; y++) {
		std::cout << "\nvalue in smaller data pix: " << (int)testSmallerDataPix[y];
	}

	// first row
	/*for (int i = 0; i < arrayWidth; i++) {
		if (i == currentChunkEndPoint) {
			// chunk from this row is done, move to other chunk
			currentChunkEndPoint = currentChunkEndPoint + 4;	// adding 4 because 4x4 pixel grid, first on row is 0, 2nd = 1, 3rd = 2, 4th = 3 <-- endpoint, new endpoint is at 7 (3+4)
			currentChunk++;

			PixelData pixelData;
			pixelData.x = i;
			//pixelData.y = something
			//pixelData.colorValue = bitmapHelper.data_pix[i];
			pixelData.colorValue = testSmallerDataPix[i];

			chunks.at(currentChunk).pixelData.at(currentChunk) = pixelData;
			currentPixelInARow++;
		}
		else {
			//still on current chunk
			PixelData pixelData;
			pixelData.x = i;
			//pixelData.y = something
			//pixelData.colorValue = bitmapHelper.data_pix[i];
			pixelData.colorValue = testSmallerDataPix[i];

			chunks.at(currentChunk).pixelData.at(currentChunk) = pixelData;
			currentPixelInARow++;
		}
	}*/

	// print values for test

}