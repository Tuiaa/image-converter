#include "CompressionHelper.h"
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <iostream>
#include <vector>

/*
 *		Initialise Settings For Compression
 *		- takes image width and height as parameters
 *		- amount of chunks the image should be sliced into can be calculated using that info
 */
void CompressionHelper::initialiseSettingsForCompression(int widthOfUsedImage, int heightOfUsedImage) {
	chunkSize = 4;
	totalAmountOfChunks = calculateHowManyChunksAreNeeded(widthOfUsedImage, heightOfUsedImage);

	totalAmountOfChunksHorizontally = widthOfUsedImage / chunkSize;
	totalAmountOfChunksVertically = heightOfUsedImage / chunkSize;

	amountOfPixelsInARowOfChunks = widthOfUsedImage * chunkSize;
	lengthOfOneRowInPixels = widthOfUsedImage;
}

/*
 *		Calculate How Many Chunks Are Needed
 *		- helper function to get the amount
 */
int CompressionHelper::calculateHowManyChunksAreNeeded(int width, int height) {

	// CRITICAL ERROR
	// also add to use chunksize
	//int horizontalChunks = width / 4;
	int horizontalChunks = width / 2;
	//int verticalChunks = height / 4;
	int verticalChunks = height / 2;

	int amountOfChunks = horizontalChunks * verticalChunks;
	return amountOfChunks;
}


void CompressionHelper::initializeArrayOfChunks() {
	std::vector<PixelChunk> createdChunks;
	for (int i = 0; i < totalAmountOfChunks; i++) {
		//std::vector<PixelData> chunk = initializeArrayOfPixelData(16);
		std::vector<PixelData> chunk = initializeArrayOfPixelData(16);
		PixelChunk tempPixelChunk;
		tempPixelChunk.pixelData = chunk;

		createdChunks.push_back(tempPixelChunk);
		//std::cout << "\nchunk created with pixeldata ";
	}

	chunks = createdChunks;
	std::cout << "\nchunks done";
}

std::vector<PixelData> CompressionHelper::initializeArrayOfPixelData(int amountOfPixelDatas) {
	std::vector<PixelData> pixelData;
	for (int i = 0; i < amountOfPixelDatas; i++) {
		PixelData tempPixelData;
		tempPixelData.x = 0;
		tempPixelData.y = 0;
		tempPixelData.colorValue = 0;

		pixelData.push_back(tempPixelData);
		//std::cout << "\npixeldata created ";
	}

	return pixelData;
}

void CompressionHelper::createTestDataSet() {
	int arrayWidth = 6;
	int arrayHeight = 4;

	// create test data set
	testSmallerDataPix = new unsigned char[24];
	std::fill(testSmallerDataPix, testSmallerDataPix + 24, 5);

	for (int y = 0; y < 24; y++) {
		//std::cout << "\nvalue in smaller data pix: " << (int)testSmallerDataPix[y];
	}
}

void CompressionHelper::addChunksUsingRealValues(unsigned char* datapixelarray) {

	pixelDataCharArray = datapixelarray;

	whatcurrentChunkShouldBe = currentRow * totalAmountOfChunksHorizontally;
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		addFirstRow();
	}

	std::cout << "\nAll chunks populated";

}

void CompressionHelper::addFirstRow() {

	// amount of first values means how many items there are in the first row of chunks
	for (int i = 0; i < amountOfPixelsInARowOfChunks; i++) {
		pixelsNeededForOneRowOfChunks.push_back(pixelDataCharArray[i]);
		//std::cout << "\nvalue added to first values: " << (int)bitmapHelper.data_pix[i];
	}

	// take first row of that chunk
	for (int j = 0; j < lengthOfOneRowInPixels; j++) {
		pixelsOfOneRowOfAnImage.push_back(pixelsNeededForOneRowOfChunks[j]);
		//std::cout << "\nvalue added to one row: " << (int)firstValues[j];
	}

	currentPixelFromThisRow = 0;
	//std::cout << "\nadding first row values to chunks";
	// go through saved row and add the values to chunks
	for (int x = 0; x < totalAmountOfChunksHorizontally; x++) {
		//std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int y = 0; y < chunkSize; y++) {
			//std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
			//std::cout << "\ncurrentPixelData = " << currentPixelData;

			// add to current chunk
			//chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
			chunks[currentChunk].pixelData[currentPixelData].colorValue = 5;
			currentPixelFromArray++;
			currentPixelData++;
		}
		currentChunk++;
		currentPixelData = 0;
	}

	currentRow++;
	//std::cout << "\nfirst row values added to chunks";

	currentChunk = whatcurrentChunkShouldBe;
	addNextRows();
}

void CompressionHelper::addNextRows() {

	//std::cout << "\nADDING NEXT ROWS STARING";

	int howManyRepeats = 3;

	// WHY ISN'T THIS TRIGGERING????
	if (chunkSize == 4) {
		howManyRepeats = 3;
	}
	else if (chunkSize == 2) {
		howManyRepeats == 1;
	}

	for (int b = 0; b < howManyRepeats; b++) {

		//std::cout << "\nINSIDE REPEATS";
		// add next rows
		currentRowFromChunk++;
		std::vector<int> nextRow;

		int nextRowLastItem = (currentRowFromChunk + 1) * lengthOfOneRowInPixels;

		// calculate the next row starting point
		int startingNumber = currentRowFromChunk * lengthOfOneRowInPixels;
		for (int a = startingNumber; a < nextRowLastItem; a++) {
			nextRow.push_back(pixelsNeededForOneRowOfChunks[a]);
			//std::cout << "\nvalue added to next row: " << (int)firstValues[a];
		}
		pixelsOfOneRowOfAnImage.clear();
		pixelsOfOneRowOfAnImage = nextRow;

		if (currentRowFromChunk == 1) {
			if (chunkSize == 2) currentPixelData = 2;
			if (chunkSize == 4) currentPixelData = 4;
		}
		else if (currentRowFromChunk == 2) {
			//if (chunkSize == 2) currentPixelData = 2;
			if (chunkSize == 4) currentPixelData = 8;
		}
		else if (currentRowFromChunk == 3) {
			//if (chunkSize == 2) currentPixelData = 2;
			if (chunkSize == 4) currentPixelData = 12;
		}

		int currentPixelDataOnThisRow = currentPixelData;

		currentPixelFromThisRow = 0;
		//std::cout << "\nadding next row values to chunks";
		// go through saved row and add the values to chunks
		for (int x = 0; x < totalAmountOfChunksHorizontally; x++) {
			//std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
			for (int y = 0; y < chunkSize; y++) {
				//std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
				//std::cout << "\ncurrentPixelData = " << currentPixelData;

				// add to current chunk
				//chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
				chunks[currentChunk].pixelData[currentPixelData].colorValue = 5;

				currentPixelFromArray++;
				currentPixelData++;
			}
			currentChunk++;
			currentPixelData = currentPixelDataOnThisRow;
		}
		//std::cout << "\nnext row values added to chunks, currentPixelFromArray = " << currentPixelFromArray;

		currentRow++;
	}

	whatcurrentChunkShouldBe = (currentRow - 1) * totalAmountOfChunksHorizontally;
	currentChunk = whatcurrentChunkShouldBe;
	currentPixelData = 0;
	currentRowFromChunk = 0;

	//std::cout << "\nOut of for loop in the end of adding next rows";
}