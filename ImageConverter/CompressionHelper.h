#pragma once
#include <vector>

struct PixelChunk {
	std::vector<int> colorValueOfPixel;
};

class CompressionHelper {
private:
	/*	These values are set in initialiseSettingsForCompression method	*/
	int chunkSize;								// chunkSize is 4 because BC1 should use 4x4 blocks of texels
	int totalAmountOfChunks;					// total amound of chunks in one image
	int totalAmountOfChunksHorizontally;		// how many chunks there are horizontally in total
	int totalAmountOfChunksVertically;			// how many chunks there are vertically in total
	int amountOfPixelsInARowOfChunks;			// how many pixels in total there are in a row of 4x4 chunks
	int lengthOfOneRowInPixels;					// this is the same as the width of an image
	int pixelsInOneChunk;						// how many pixels in total are in one chunk (4x4 = 16)

	/*	General helper values	*/
	int currentPixelFromChunk = 0;
	int currentPixelFromThisRow = 0;
	int currentRowFromChunk = 0;
	int currentChunk = 0;

	std::vector<int> pixelsNeededForOneRowOfChunks;		// one row of chunks is 4 * image width pixels
	std::vector<int> pixelsOfOneRowOfImage;				// one row of image is image width amount of pixels

	std::vector<PixelChunk> allChunks;
	unsigned char* allPixelsFromImage;

	/*	Initialization functions	*/
	void initializeVectorOfChunks();
	std::vector<int> initializeArrayOfPixelColorValues();

	/*	Helper functions  */
	int calculateHowManyChunksAreNeeded(int width, int height);
	void calculateAllPixelsNeededForRowOfChunks(int startingPoint);
	void calculatePixelsFromOneRowOfImage(int startingPoint);
	int getCurrentChunkStartingPixelPosition();


	void sliceImageIntoOneChunkRow(int startingPoint);
	void sliceImageIntoChunks();

public:
	void initializeSettingsForCompression(int imageWidth, int imageHeight, unsigned char* pixelsFromImage);
	void startCompression();
};