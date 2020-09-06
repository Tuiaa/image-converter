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

	int horizontalChunks = width / 4;
	int verticalChunks = height / 4;

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
		std::vector<int> chunkOfPixels = initializeArrayOfPixelColorValues();

		PixelChunk tempPixelChunk;
		tempPixelChunk.colorValueOfPixel = chunkOfPixels;

		allChunksFromImage.push_back(tempPixelChunk);
	}
}

/*
 *		Initialize Vector Of PixelData
 *		- initializes the pixel data (basically color value) stored inside a chunk
 */
std::vector<int> CompressionHelper::initializeArrayOfPixelColorValues() {

	std::vector<int> colorValuesOfPixel;
	for (int i = 0; i < pixelsInOneChunk; i++) {
		colorValuesOfPixel.push_back(0);
	}

	return colorValuesOfPixel;
}

void CompressionHelper::startCompression() {

	int amountOfChunksPopulated = 0;
	// Calculate current chunk using the value of current row and total amount of horizontal chunks
	whatcurrentChunkShouldBe = currentRow * totalAmountOfChunksHorizontally;
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		//addFirstRowOfNewChunk();
		addNextRows();
		amountOfChunksPopulated++;
		//std::cout << "\nAmount of chunks populated: " << amountOfChunksPopulated;
		if (amountOfChunksPopulated == 6) {
			//std::cout << "\nbreak";
		}
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
		pixelsOfOneRowOfImage.push_back(pixelsNeededForOneRowOfChunks[j]);
											//std::cout << "\nvalue added to one row: " << (int)firstValues[j];
	}
}

void CompressionHelper::addFirstRowOfNewChunk() {
	calculateAllPixelsNeededForRowOfChunks();
	calculateAllPixelsNeededForOneRowOfChunk();

	currentPixelFromChunk = 0;
	// Go through the saved row of pixels
	for (int chunk = 0; chunk < totalAmountOfChunksHorizontally; chunk++) {
										//std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int pixelNeededForChunk = 0; pixelNeededForChunk < chunkSize; pixelNeededForChunk++) {
											//std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
											//std::cout << "\ncurrentPixelData = " << currentPixelData;

			// add to chunk that's currently being populated, currentPixelData tells which of the pixels inside of a chunk is being populated
											//chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
			allChunksFromImage[currentChunk].colorValueOfPixel[currentPixelFromChunk] = 5;
			currentPixelFromArray++;
			currentPixelFromChunk++;
		}

		// One chunk gone through, jump to next one
		currentChunk++;
		// Jumping to next chunk so resetting the position of currenly being filled pixel
		currentPixelFromChunk = 0;
	}

	// One row added so adding more to the current row calculation
	currentRow++;

	currentChunk = whatcurrentChunkShouldBe;
	addNextRows();
}

void CompressionHelper::addNextRows() {

	calculateAllPixelsNeededForRowOfChunks();
	calculateAllPixelsNeededForOneRowOfChunk();

	// there's 4 rows in one chunk so 3 more rows needed
	int howManyRepeats = 4;
	currentRowFromChunk = 0;

	//std::cout << "\ngoing into repeats";
	for (int b = 0; b < howManyRepeats; b++) {
		//std::cout << "\n\nIN THE START OF HOW MANY REPEATS SO START OF ADDING NEW ROW\n\n";
		//currentRowFromChunk++;		// new row from chunk
		std::vector<int> tempPixelsOfThisRowOfImage;

		// calculate the next row starting point
		//int nextRowStartingPosition = currentRowFromChunk * lengthOfOneRowInPixels;		// first row = 0, second: 400 etc

		int currentRowStartingPixelPosition = currentRowFromChunk * lengthOfOneRowInPixels;		// first row = 0*400 = 0, second = 1*400 = 400
		//std::cout << "\ncurrent row starting pixel position: " << currentRowStartingPixelPosition;

		int thisRowLastPixel = (currentRowFromChunk + 1) * lengthOfOneRowInPixels;		// with image of width 400, first row: 0+1 * 400 = 400, second: 1+1 * 800 etc
		//std::cout << "\nthis row last pixel: " << thisRowLastPixel;
		//std::cout << "\ngoing to fetch pixels of this row";
		// Get next saved row of pixels from pixelsNeededForOneRowOfChunks
		for (int a = currentRowStartingPixelPosition; a < thisRowLastPixel; a++) {
			tempPixelsOfThisRowOfImage.push_back(pixelsNeededForOneRowOfChunks[a]);
									//std::cout << "\nvalue added to next row: " << (int)firstValues[a];
		}

		//std::cout << "\npixels of this row fetched";
		// Add calculated pixels to the row vector
		pixelsOfOneRowOfImage.clear();
		pixelsOfOneRowOfImage = tempPixelsOfThisRowOfImage;

		// Hardcoded values because no time to refactor
		if (currentRowFromChunk == 0) {
			currentPixelFromChunk = 0;
		}
		else if (currentRowFromChunk == 1) {
			currentPixelFromChunk = 4;
		}
		else if (currentRowFromChunk == 2) {
			currentPixelFromChunk = 8;
		}
		else if (currentRowFromChunk == 3) {
			currentPixelFromChunk = 12;
		}

		//int currentPixelOnThisRow = currentPixelFromChunk;

		//std::cout << "\ngoing into adding pixel values into chunks";

		int maxPixelNum = currentPixelFromChunk + 4;

		// Go through the saved row of pixels
		for (int chunk = 0; chunk < totalAmountOfChunksHorizontally; chunk++) {
			//std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
			//std::cout << "\ninside for(int chunk = 0; chunk < totalAmountOfChunksHorizontally; chunk++)";
			//std::cout << "\ntotalAmountOfChunksHorizontally = " << totalAmountOfChunksHorizontally;

			for (int pixelNeededForChunk = currentPixelFromChunk; pixelNeededForChunk < maxPixelNum; pixelNeededForChunk++) {
				//std::cout << "\ninside for(int pixelNeededForChunk = 0; pixelNeededForChunk < chunkSize; pixelNeededForChunk++)";
				//std::cout << "\n\ncurrentChunk = " << currentChunk;
				//std::cout << "\nchunk = " << chunk;
				//std::cout << "\ncurrentPixelOnThisRow = " << currentPixelOnThisRow;
				//std::cout << "\npixelNeededForChunk = " << pixelNeededForChunk;
				//std::cout << "\ncurrentPixelFromChunk = " << currentPixelFromChunk << "\n";
				//std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;

// add to chunk that's currently being populated, currentPixelData tells which of the pixels inside of a chunk is being populated
								//chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
				allChunksFromImage[chunk].colorValueOfPixel[pixelNeededForChunk] = 5;
				currentPixelFromArray++;
				//currentPixelOnThisRow++;
				//currentPixelFromChunk++;
			}

			// One chunk gone through, jump to next one
			currentChunk++;
			// Jumping to next chunk so resetting the position of currenly being filled pixel
			//currentPixelFromChunk = 0;
		}
		std::cout << "\nONE ROW ADDED";

		currentRow++;
		//currentPixelOnThisRow = 0;
		currentRowFromChunk++;
	}

	//whatcurrentChunkShouldBe = (currentRow - 1) * totalAmountOfChunksHorizontally;
	//currentChunk = whatcurrentChunkShouldBe;
	//currentPixelFromChunk = 0;

	// reset current row of chunk because this set of chunks is added
	currentRowFromChunk = 0;

	//std::cout << "\nOut of for loop in the end of adding next rows";
}