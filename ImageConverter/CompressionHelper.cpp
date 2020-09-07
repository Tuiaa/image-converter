#include "CompressionHelper.h"
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

	int horizontalChunks = width / chunkSize;
	int verticalChunks = height / chunkSize;

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

		allChunks.push_back(tempPixelChunk);
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

/*
 *		Start Compression
 *		- method that handles the compression steps
 *		- DXT1 (BC1) block compression uses 4x4 pixel blocks (chunks)
 *		  and compresses their color data 
 */
void CompressionHelper::startCompression() {
	sliceImageIntoChunks();
	// TODO calculate min and max color values of one chunk (and also two intermediate colors)
	// TODO apply the calculated colors into the chunk and repeat process for all chunks
	// TODO take all chunks and put them back to the uncharted char array of pixels
}

/*
 *		Slice Image Into Chunks
 *		- image should be sliced into 4x4 pixel chunks
 *		- one chunk row constains 4 * width of image amount of pixels
 */
void CompressionHelper::sliceImageIntoChunks() {
	int amountOfChunksPopulated = 0;

	// Go through image one chunk row (height of chunk row is chunkSize) at a time
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		sliceImageIntoOneChunkRow(amountOfChunksPopulated);
		amountOfChunksPopulated += totalAmountOfChunksHorizontally;
	}
}

/*
 *		Calculate All Pixels Needed For Row Of Chunks
 *		- find all pixels that are needed to populate one row of chunks
 *		- one chunk is 4x4, in one row there are widthOfImage/chunkSize number of chunks
 */
void CompressionHelper::calculateAllPixelsNeededForRowOfChunks(int startingPoint) {

	for (int i = startingPoint; i < amountOfPixelsInARowOfChunks; i++) {
		pixelsNeededForOneRowOfChunks.push_back(allPixelsFromImage[i]);
	}
}

/*
 *		Calculate Pixels From One Row Of Image
 *		- find all pixels that are needed to populate one row
 *		- one row is 1 pixel * width of an image
 */
void CompressionHelper::calculatePixelsFromOneRowOfImage(int startingPoint) {
	for (int i = startingPoint; i < lengthOfOneRowInPixels; i++) {
		pixelsOfOneRowOfImage.push_back(pixelsNeededForOneRowOfChunks[i]);
	}
}

/*
 *		Slice Image Into One Chunk Row
 *		- One chunk row height is chunkSize so total amount of pixels in chunk row
 *		  is chunkSize * width of an image
 *		- calculates all pixels needed for one chunk row
 *		- populates pixels of one row of an image into chunks and repeats it for all 
 *		  rows (amount of rows from image = chunkSize)
 */
void CompressionHelper::sliceImageIntoOneChunkRow(int startingPixel) {

	calculateAllPixelsNeededForRowOfChunks(startingPixel);

	currentRowFromChunk = 0;
	int howManyRepeats = chunkSize;

	for (int i = 0; i < howManyRepeats; i++) {
		int currentRowStartingPixelPosition = currentRowFromChunk * lengthOfOneRowInPixels;	

		calculatePixelsFromOneRowOfImage(currentRowStartingPixelPosition);

		int currentChunkStartingPixelPosition = getCurrentChunkStartingPixelPosition();
		int maxPixelPositionOnChunk = currentChunkStartingPixelPosition + chunkSize;
		int currentRowLastPixelPosition = startingPixel + totalAmountOfChunksHorizontally;

		// Save the color value from image into chunk (currently uses temp data)
		for (int chunk = startingPixel; chunk < currentRowLastPixelPosition; chunk++) {
			for (int pixelPosition = currentChunkStartingPixelPosition; pixelPosition < maxPixelPositionOnChunk; pixelPosition++) {

				allChunks[chunk].colorValueOfPixel[pixelPosition] = 5;
			}

			currentChunk++;
		}

		currentRowFromChunk++;
	}

	// reset current row of chunk because this set of chunks is added
	currentRowFromChunk = 0;
}

/*
 *		Get Current Chunk Starting Pixel Position
 *		- hardcoded values, should be refactored
 *		- returns the starting position of color value saved in chunk in this row
 */
int CompressionHelper::getCurrentChunkStartingPixelPosition() {

	int currentPixelFromChunk = 0;

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

	return currentPixelFromChunk;
}