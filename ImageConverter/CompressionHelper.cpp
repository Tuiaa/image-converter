#define _CRT_SECURE_NO_WARNINGS
#include "CompressionHelper.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

/*
 *		Initialize Settings For Compression
 *		- takes image width and height as parameters
 *		- amount of chunks the image should be sliced into can be calculated using that info
 */
void CompressionHelper::initializeSettingsForCompression(int widthOfUsedImage, int heightOfUsedImage, std::vector<int> allPixelsFromBitmapVector, unsigned char* pixelsFromUsedImage) {
	
	chunkSize = 4;

	totalAmountOfChunks = calculateHowManyChunksAreNeeded(widthOfUsedImage, heightOfUsedImage);
	totalAmountOfChunksHorizontally = widthOfUsedImage / chunkSize;
	totalAmountOfChunksVertically = heightOfUsedImage / chunkSize;

	amountOfPixelsInARowOfChunks = widthOfUsedImage * chunkSize;
	lengthOfOneRowInPixels = widthOfUsedImage;
	pixelsInOneChunk = chunkSize * chunkSize;

	pixelDataFromBitmap = allPixelsFromBitmapVector;

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
		PixelChunk tempPixelChunk;
		std::vector<PixelInfo> pixelInfo = initializeVectorOfPixelInfo();
		tempPixelChunk.pixelInfo = pixelInfo;

		allChunks.push_back(tempPixelChunk);
	}
}

/*
 *		Initialize Vector Of PixelData
 *		- initializes the pixel data (basically color value) stored inside a chunk
 */
std::vector<PixelInfo> CompressionHelper::initializeVectorOfPixelInfo() {

	std::vector<PixelInfo> pixelInfoVector;

	for (int i = 0; i < pixelsInOneChunk; i++) {
		PixelInfo pixelInfo;
		pixelInfo.colorValueOfPixelR = 0;
		pixelInfo.colorValueOfPixelG = 0;
		pixelInfo.colorValueOfPixelB = 0;
		pixelInfo.pixelRPositionInArray = 0;
		pixelInfo.pixelGPositionInArray = 0;
		pixelInfo.pixelBPositionInArray = 0;
		pixelInfoVector.push_back(pixelInfo);
	}

	return pixelInfoVector;
}

/*
 *		Start Compression
 *		- method that handles the compression steps
 *		- DXT1 (BC1) block compression uses 4x4 pixel blocks (chunks)
 *		  and compresses their color data
 */
std::vector<int> CompressionHelper::startCompression() {

	sliceImageIntoChunks();
	std::cout << "\n\njee image sliced!";

	goThroughPixelDataAndCompress();
	combineChunksBackToPixelArray();
	std::cout << "jee image put back together!";

	return pixelDataAfterCompression;
}

/*
 *		Slice Image Into Chunks
 *		- image should be sliced into 4x4 pixel chunks
 *		- one chunk row constains 4 * width of image amount of pixels
 */
void CompressionHelper::sliceImageIntoChunks() {
	
	int amountOfChunksPopulated = 0;
	actualAmountOfChunksPopulated = 0;

	int howmanytimesrepeatedforinsliceimageintochunks = 0;
	int amountOfChunkRowsPopulated = 0;
	int chunkStartingPixelPosition = (lengthOfOneRowInPixels * 3) * 4 * amountOfChunkRowsPopulated;

	// Go through image one chunk row (height of chunk row is chunkSize) at a time
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		sliceImageIntoOneChunkRow(amountOfChunksPopulated, chunkStartingPixelPosition);
		amountOfChunksPopulated += totalAmountOfChunksHorizontally;
		howmanytimesrepeatedforinsliceimageintochunks++;
		amountOfChunkRowsPopulated++;

	}
}

/*
 *		Calculate All Pixels Needed For Row Of Chunks
 *		- find all pixels that are needed to populate one row of chunks
 *		- one chunk is 4x4, in one row there are widthOfImage/chunkSize number of chunks
 */
void CompressionHelper::calculateAllPixelsNeededForRowOfChunks(int startingPoint) {
	
	pixelsNeededForOneRowOfChunks.clear();
	int length = amountOfPixelsInARowOfChunks * 3;
	for (int i = startingPoint; i < length; i++) {
		pixelsNeededForOneRowOfChunks.push_back(pixelDataFromBitmap[i]);
	}
}

/*
 *		Calculate Pixels From One Row Of Image
 *		- find all pixels that are needed to populate one row
 *		- one row is 1 pixel * width of an image
 */
void CompressionHelper::calculatePixelsFromOneRowOfImage(int startingPoint) {
	
	pixelsOfOneRowOfImage.clear();
	int length = lengthOfOneRowInPixels * 3;
	for (int i = startingPoint; i < length; i++) {
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
void CompressionHelper::sliceImageIntoOneChunkRow(int amountOfChunksPopulated, int startingPixel) {
	
	calculateAllPixelsNeededForRowOfChunks(startingPixel);

	int currentRowStartingChunkPosition = amountOfChunksPopulated;	// 100
	int currentRowLastChunkPosition = currentRowStartingChunkPosition + totalAmountOfChunksHorizontally;	// 199

	currentRowFromChunk = 0;
	int howManyRepeats = chunkSize;

	for (int i = 0; i < howManyRepeats; i++) {
		int currentRowStartingPixelPosition = currentRowFromChunk * lengthOfOneRowInPixels;

		calculatePixelsFromOneRowOfImage(currentRowStartingPixelPosition);

		int currentChunkStartingPixelPosition = getCurrentChunkStartingPixelPosition();
		int maxPixelPositionOnChunk = currentChunkStartingPixelPosition + chunkSize;
		int currentRowLastPixelPosition = currentRowStartingPixelPosition + (lengthOfOneRowInPixels * 3);

		// Save the color value from image into chunk (currently uses temp data)
		for (int chunk = currentRowStartingChunkPosition; chunk < currentRowLastChunkPosition; chunk++) {
			for (int pixelPosition = currentChunkStartingPixelPosition; pixelPosition < maxPixelPositionOnChunk; pixelPosition++) {
				
				for (int i = 0; i < 3; i++) {
					if (i == 0) {
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelR = pixelDataFromBitmap[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelRPositionInArray = currentPixelNumPosition;
						currentPixelNumPosition++;
					}
					else if (i == 1) {
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelG = pixelDataFromBitmap[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelGPositionInArray = currentPixelNumPosition;
						currentPixelNumPosition++;
					}
					else if (i == 2) {
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelB = pixelDataFromBitmap[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelBPositionInArray = currentPixelNumPosition;
						currentPixelNumPosition++;
					}
				}
			}
			currentChunk++;
		}
		currentRowFromChunk++;
	}

	// reset current row of chunk because this set of chunks is added
	currentRowFromChunk = 0;
	actualAmountOfChunksPopulated = actualAmountOfChunksPopulated + totalAmountOfChunksHorizontally;
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

void CompressionHelper::combineChunksBackToPixelArray() {

	initializePixelDataAfterCompressionVector();

	for (int i = 0; i < totalAmountOfChunks; i++) {
		for (int j = 0; j < pixelsInOneChunk; j++) {
				int pixelPosRFromPixelInfo = allChunks[i].pixelInfo[j].pixelRPositionInArray;
				pixelDataAfterCompression[pixelPosRFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelR;

				int pixelPosGFromPixelInfo = allChunks[i].pixelInfo[j].pixelGPositionInArray;
				pixelDataAfterCompression[pixelPosGFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelG;

				int pixelPosBFromPixelInfo = allChunks[i].pixelInfo[j].pixelBPositionInArray;
				pixelDataAfterCompression[pixelPosBFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelB;
		}
	}
}

void CompressionHelper::initializePixelDataAfterCompressionVector() {

	int totalPixelsAmount = pixelDataFromBitmap.size();
	for (int i = 0; i < totalPixelsAmount; i++) {
		pixelDataAfterCompression.push_back(3);
	}
}

void CompressionHelper::goThroughPixelDataAndCompress() {

	for (int i = 0; i < allChunks.size(); i++) {
		calculateColorTableFromOneChunk(i);
	}
}

void CompressionHelper::calculateColorTableFromOneChunk(int chunkIndex) {
	
	int pixelInfoSize = allChunks[chunkIndex].pixelInfo.size();
	
	/* CREATING THE COLOR TABLE */

	// color_0 value (min)
	int tempMinR = 255;
	int tempMinG = 255;
	int tempMinB = 255;

	// color_1 value (max)
	int tempMaxR = 0;
	int tempMaxG = 0;
	int tempMaxB = 0;

	// These are calculated using linear interpolation
	// color_2 = 2/3*color_0 + 1/3*color_1
	int tempColor2R = 0;
	int tempColor2G = 0;
	int tempColor2B = 0;

	// color_3 = 1/3*color_0 + 2/3*color_1
	int tempColor3R = 0;
	int tempColor3G = 0;
	int tempColor3B = 0;

	for (int i = 0; i < pixelInfoSize; i++) {
		// find max and min red
		if (tempMaxR < allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelR) {
			tempMaxR = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelR;
		}

		if (tempMinR > allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelR) {
			tempMinR = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelR;
		}

		// find max and min green
		if (tempMaxG < allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelG) {
			tempMaxG = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelG;
		}

		if (tempMinG > allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelG) {
			tempMinG = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelG;
		}

		// fid max and min blue
		if (tempMaxB < allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelB) {
			tempMaxB = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelB;
		}

		if (tempMinB > allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelB) {
			tempMinB = allChunks[chunkIndex].pixelInfo[i].colorValueOfPixelB;
		}
	}

	/* ASSIGNING INDEX VALUES INTO ALL PIXELS OF A CHUNK */

	// So each of the colors in the color table have 2 bit index value
	//	color_0 = 00
	//	color_1 = 01
	//	color_2 = 10
	//	color_3 = 11

	// These index values are saved in an unsigned char array that has size of 4 bytes (32 bits)
	unsigned char* indices = new unsigned char[32];

	// So when all this is done we save 50% of the memory

	// I'm a bit confused how this array is combined
	// One place that sounded the most logical said: 4x4 texel chunk size after compression is 8 bytes (2 byte color_0, 2 byte color_1, 
	//												 and 4 bytes index value array for each of the pixels in 4 x 4 texel chunk, meaning 
	//												 16 x 2 bit values, which equals to 32bits = 4 bytes)
	// http://www.reedbeta.com/blog/understanding-bcn-texture-compression-formats/ (BC1 paragraph)

	// but for example Microsoft documentation about BC1 says that: "Instead of storing 16 colors, the algorithm saves 2 reference 
	//												 colors (color_0 and color_1) and 16 2-bit color indices (blocks a–p), as shown in 
	//												 the following diagram", but this diagram is saying that these a-p color indices are 
	//												 just 1 byte, but 16 x 2 bits = 16 bits = 2 bytes?
	// It's also confusing in the diagram that what's the total size of the array? It shows that there's 2 bytes for color_0 and 
	//												 nothing marked for color_1 (or does that mean there are 2 bytes in total for color_0 
	//												 and color_1 or does this mean both color_0 and color_1 are 2 bytes?) What are the 
	//												 rest of the bytes when the diagram only shows 3 (or 5?) bytes?
	// https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression?redirectedfrom=MSDN

	// Well I guess the Microsoft documentation means basically the same as the other place where I was reading from, but the main issue why
	// I wasn't able to continue forward from this is because I don't understand how I can save color value using format (5:6:5) in 2 bytes

	// I tried switching rgb to hexadecimal color value but it takes 3 bytes to store that
	// I even tried to do that different ways using binary but I guess that also uses too much memory

	// So I don't understand enough either about how pixels or colors are created, or about memory optimization or something like that :)
}