#include "CompressionHelper.h"
#include "BitmapHelper.h"
#include "Bitmap.h"
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

void CompressionHelper::populateChunks() {

}