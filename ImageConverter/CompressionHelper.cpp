#include "CompressionHelper.h"
#include <iostream>
#include <vector>
#include <algorithm>

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




	//// Initial test data
	//const char* testdata = "the quick brown fox jumps over the lazy dog.";

	//// Transform from 'const char*' to 'vector<unsigned char>'
	//std::string input(testdata);
	//std::vector<unsigned char> output(input.length());
	//std::transform(input.begin(), input.end(), output.begin(),
	//	[](char c)
	//{
	//	return static_cast<unsigned char>(c);
	//});

	//// Use the transformed data in 'output'...
	//
	//std::cout << "output: " + output[0];


	/*for (int i = 0; i < 48000; i++) {
		allPixelsFromImageVector[i] = pixelsFromUsedImage[i];
	}*/

	allPixelsFromImage = pixelsFromUsedImage;

	// USE THIS INSTEAD OF UNSIGNED CHAR LIST
	// TODO: GO THROUGH ALL R G AND B VALUES AND ADD THEM TO CHUNKS
	allPixelsFromImageVector = allPixelsFromBitmapVector;

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
		//std::vector<int> chunkOfPixels = initializeArrayOfPixelColorValues();
		PixelChunk tempPixelChunk;
		std::vector<PixelInfo> pixelInfo = initializeArrayOfPixelColorValues();
		tempPixelChunk.pixelInfo = pixelInfo;

		allChunks.push_back(tempPixelChunk);
	}
}

/*
 *		Initialize Vector Of PixelData
 *		- initializes the pixel data (basically color value) stored inside a chunk
 */
std::vector<PixelInfo> CompressionHelper::initializeArrayOfPixelColorValues() {

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
		//colorValuesOfPixel.push_back(0);
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

	combineChunksBackToPixelArray();
	std::cout << "jee image put back together!";

	return pixelDataArray;

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
	actualAmountOfChunksPopulated = 0;

	int howmanytimesrepeatedforinsliceimageintochunks = 0;
	int amountOfChunkRowsPopulated = 0;
	int chunkStartingPixelPosition = (lengthOfOneRowInPixels * 3) * 4 * amountOfChunkRowsPopulated;

	// Go through image one chunk row (height of chunk row is chunkSize) at a time
	for (int i = 0; i < totalAmountOfChunksVertically; i++) {
		// should insert pixel value, not chunk value
		//sliceImageIntoOneChunkRow(amountOfChunksPopulated);

		sliceImageIntoOneChunkRow(amountOfChunksPopulated, chunkStartingPixelPosition);
		amountOfChunksPopulated += totalAmountOfChunksHorizontally;
		howmanytimesrepeatedforinsliceimageintochunks++;
		amountOfChunkRowsPopulated++;

	}

	std::cout << "\n\ntotal amount of chunks vertically: " << totalAmountOfChunksVertically;
	std::cout << "\ntotal amount of chunks horizontally: " << totalAmountOfChunksHorizontally;
	std::cout << "\ntotal amount of chunks: " << totalAmountOfChunks;
	std::cout << "\namount of chunks populated: " << amountOfChunksPopulated;
	std::cout << "\nactual amount of chunks populated: " << actualAmountOfChunksPopulated;


	std::cout << "\namount of pixels in a row of chunks: " << amountOfPixelsInARowOfChunks;
	std::cout << "\npixels in one chunk: " << pixelsInOneChunk;
	std::cout << "\nlength of one row i pixels: " << lengthOfOneRowInPixels << "\n";

	std::cout << "\nhow many times repeated for in slice image into chunks: " << howmanytimesrepeatedforinsliceimageintochunks;
	std::cout << "\nslice image into one chunk row called this many times: " << sliceImageIntoOneChunkRowCalledThisManyTimes;
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
		pixelsNeededForOneRowOfChunks.push_back(allPixelsFromImage[i]);
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
	sliceImageIntoOneChunkRowCalledThisManyTimes++;
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
		
		// WHY WOULD PIXEL POSITION BE CALCULATED USING CHUNKS??
		//int currentRowLastPixelPosition = startingPixel + totalAmountOfChunksHorizontally;
		int currentRowLastPixelPosition = currentRowStartingPixelPosition + (lengthOfOneRowInPixels * 3);

		//SHOULD USE THIS????
		//pixelsOfOneRowOfImage
		int RGOrB = 0;
		int stayInThisPixel = 0;
		// Save the color value from image into chunk (currently uses temp data)
		//for (int chunk = startingPixel; chunk < currentRowLastPixelPosition; chunk++) {
		for (int chunk = currentRowStartingChunkPosition; chunk < currentRowLastChunkPosition; chunk++) {
			for (int pixelPosition = currentChunkStartingPixelPosition; pixelPosition < maxPixelPositionOnChunk; pixelPosition++) {
				
				for (int i = 0; i < 3; i++) {
					if (RGOrB == 0) {
						//allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixel = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelR = 255;
						//allChunks[chunk].colorValueOfPixel[pixelPosition] = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelRPositionInArray = currentPixelNumPosition;
						//allChunks[chunk].pixelPositionInArray = currentPixelNumPosition;
						//std::cout << "\nchunk: " << chunk;
						//std::cout << "\npixel position: " << pixelPosition;
						//std::cout << "\nRGorB: " << RGOrB << "\n";
						currentPixelNumPosition++;
						RGOrB++;
					}
					else if (RGOrB == 1) {
						//allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixel = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelG = 255;
						//allChunks[chunk].colorValueOfPixel[pixelPosition] = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelGPositionInArray = currentPixelNumPosition;
						//allChunks[chunk].pixelPositionInArray = currentPixelNumPosition;
						currentPixelNumPosition++;
						RGOrB++;
					}
					else if (RGOrB == 2) {
						//allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixel = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].colorValueOfPixelB = 255;
						//allChunks[chunk].colorValueOfPixel[pixelPosition] = (int)allPixelsFromImage[currentPixelNumPosition];
						allChunks[chunk].pixelInfo[pixelPosition].pixelBPositionInArray = currentPixelNumPosition;
						//allChunks[chunk].pixelPositionInArray = currentPixelNumPosition;
						currentPixelNumPosition++;
						RGOrB = 0;
						//stayInThisPixel++;
					}
				}

			}
			//actualAmountOfChunksPopulated++;
			currentChunk++;
			//std::cout << "\ncurrent chunk: " << currentChunk;
			//std::cout << "\nstay in this pixel: " << stayInThisPixel << "\n";
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

	InitializePixelDataArray();
	//pixelDataArray = std::vector<int>(currentPixelNumPosition + 1);

	std::cout << "\nstarting to fill pixel array\n";
	int RGOrB = 0;
	int stayInThisPixel = 0;
	for (int i = 0; i < totalAmountOfChunks; i++) {
		for (int j = 0; j < pixelsInOneChunk; j++) {
			//auto pixelPos = pixelDataArray.begin() + allChunks[i].pixelInfo[j].pixelPositionInArray;
			
				int pixelPosRFromPixelInfo = allChunks[i].pixelInfo[j].pixelRPositionInArray;
				//pixelDataArray.insert(pixelPos, allChunks[i].pixelInfo[j].colorValueOfPixel);
				pixelDataArray[pixelPosRFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelR;

				int pixelPosGFromPixelInfo = allChunks[i].pixelInfo[j].pixelGPositionInArray;
				//pixelDataArray.insert(pixelPos, allChunks[i].pixelInfo[j].colorValueOfPixel);
				pixelDataArray[pixelPosGFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelG;

				int pixelPosBFromPixelInfo = allChunks[i].pixelInfo[j].pixelBPositionInArray;
				//pixelDataArray.insert(pixelPos, allChunks[i].pixelInfo[j].colorValueOfPixel);
				pixelDataArray[pixelPosBFromPixelInfo] = allChunks[i].pixelInfo[j].colorValueOfPixelB;


			//std::cout << "\ninserting in pos: " + allChunks[i].pixelInfo[j].pixelPositionInArray;
			//std::cout << "\nstarting to fill pixel array\n";
		}
	}

	std::cout << "\npixel data array initialized\n";
}

void CompressionHelper::InitializePixelDataArray() {
	for (int i = 0; i < 144000; i++) {
		pixelDataArray.push_back(3);
	}
}