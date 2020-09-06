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
void CompressionHelper::initialiseSettingsForCompression(int widthOfUsedImage, int heightOfUsedImage, unsigned char* pixelsFromUsedImage) {
	chunkSize = 4;

	totalAmountOfChunks = calculateHowManyChunksAreNeeded(widthOfUsedImage, heightOfUsedImage);
	totalAmountOfChunksHorizontally = widthOfUsedImage / chunkSize;
	totalAmountOfChunksVertically = heightOfUsedImage / chunkSize;

	amountOfPixelsInARowOfChunks = widthOfUsedImage * chunkSize;
	lengthOfOneRowInPixels = widthOfUsedImage;
	pixelsInOneChunk = chunkSize * chunkSize;

	allPixelsFromImage = pixelsFromUsedImage;

	initializeVectorOfChunks();
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

/*
 *		Initialize Vector Of Chunks
 *		- initializes correct amount of chunks
 *		(side note: I'm not sure if this should be done or not, should I just create them when reading the data from bitmap?
 *		 Do I even need to initialise them befrehand at all?)
 */
void CompressionHelper::initializeVectorOfChunks() {

	for (int i = 0; i < totalAmountOfChunks; i++) {
		std::vector<PixelData> chunk = initializeArrayOfPixelData();
		PixelChunk tempPixelChunk;
		tempPixelChunk.pixelData = chunk;

		allChunksFromImage.push_back(tempPixelChunk);
	}
}

/*
 *		Initialize Vector Of PixelData
 *		- initializes the pixel data (basically color value) stored inside a chunk
 */
std::vector<PixelData> CompressionHelper::initializeArrayOfPixelData() {

	std::vector<PixelData> pixelData;
	for (int i = 0; i < pixelsInOneChunk; i++) {
		PixelData tempPixelData;
		tempPixelData.x = 0;
		tempPixelData.y = 0;
		tempPixelData.colorValue = 0;

		pixelData.push_back(tempPixelData);
	}

	return pixelData;
}

void CompressionHelper::startCompression() {

	// Calculate current chunk using the value of current row and total amount of horizontal chunks
	whatcurrentChunkShouldBe = currentRow * totalAmountOfChunksHorizontally;
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		addFirstRowOfNewChunk();
	}

	std::cout << "\nAll chunks populated";

}

/*
 *		Get All Pixels Needed For Row Of Chunks
 *		- find all pixels that are needed to populate one row of chunks
 *		- one chunk is 4x4, in one row there are widthOfImage/chunkSize number of chunks
 */
void CompressionHelper::calculateAllPixelsNeededForRowOfChunks() {
	
	for (int i = 0; i < amountOfPixelsInARowOfChunks; i++) {
		pixelsNeededForOneRowOfChunks.push_back(allPixelsFromImage[i]);
		//std::cout << "\nvalue added to first values: " << (int)bitmapHelper.data_pix[i];
	}
}

/*
 *		Get All Pixels Needed For Row
 *		- find all pixels that are needed to populate one row
 *		- one row is 1 pixel * width of an image
 */
void CompressionHelper::calculateAllPixelsNeededForOneRowOfChunk() {
	// take first row of that chunk
	for (int j = 0; j < lengthOfOneRowInPixels; j++) {
		pixelsOfOneRowOfAnImage.push_back(pixelsNeededForOneRowOfChunks[j]);
		//std::cout << "\nvalue added to one row: " << (int)firstValues[j];
	}
}

void CompressionHelper::addFirstRowOfNewChunk() {
	calculateAllPixelsNeededForRowOfChunks();
	calculateAllPixelsNeededForOneRowOfChunk();
	
	currentPixelFromThisRow = 0;

	// Go through the saved row of pixels
	for (int chunk = 0; chunk < totalAmountOfChunksHorizontally; chunk++) {
		//std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int pixelNeededForChunk = 0; pixelNeededForChunk < chunkSize; pixelNeededForChunk++) {
			//std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
			//std::cout << "\ncurrentPixelData = " << currentPixelData;

			// add to current chunk
			//chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
			allChunksFromImage[currentChunk].pixelData[currentPixelData].colorValue = 5;
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
				allChunksFromImage[currentChunk].pixelData[currentPixelData].colorValue = 5;

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