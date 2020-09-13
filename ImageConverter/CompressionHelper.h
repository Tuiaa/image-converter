#pragma once
#include <vector>
#include <string>

struct PixelInfo {
	int colorValueOfPixelR;
	int colorValueOfPixelG;
	int colorValueOfPixelB;
	int pixelRPositionInArray;
	int pixelGPositionInArray;
	int pixelBPositionInArray;
};

struct PixelChunk {
	std::vector<PixelInfo> pixelInfo;
	int color_0;
	int color_1;
	int color_2;
	int color_3;
	unsigned char indices;
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
	int currentPixelNumPosition = 0;
	int actualAmountOfChunksPopulated = 0;

	int sliceImageIntoOneChunkRowCalledThisManyTimes = 0;

	std::vector<int> pixelsNeededForOneRowOfChunks;		// one row of chunks is 4 * image width pixels
	std::vector<int> pixelsOfOneRowOfImage;				// one row of image is image width amount of pixels

	std::vector<PixelChunk> allChunks;
	unsigned char* allPixelsFromImage;
	std::vector<int> pixelDataArray;
	std::vector<int> earlierPixelDataArray;
	std::vector<int> allPixelsFromImageVector;

	/*	Initialization functions	*/
	void initializeVectorOfChunks();
	std::vector<PixelInfo> initializeArrayOfPixelColorValues();



	void initializePixelDataArray();

	/*	Helper functions  */
	int calculateHowManyChunksAreNeeded(int width, int height);
	void calculateAllPixelsNeededForRowOfChunks(int startingPoint);
	void calculatePixelsFromOneRowOfImage(int startingPoint);
	int getCurrentChunkStartingPixelPosition();


	void sliceImageIntoOneChunkRow(int amountOfChunksPopulated, int startingPoint);
	void sliceImageIntoChunks();

	void combineChunksBackToPixelArray();


	void goThroughPixelDataAndCompress();
	void calculateColorTableFromOneChunk(int chunkIndex);

	int color_0;
	int color_1;
	int color_2;
	int color_3;
	unsigned char* indices = new unsigned char[32];

public:
	void initializeSettingsForCompression(int imageWidth, int imageHeight, std::vector<int> allPixelsFromBitmapVector, unsigned char* pixelsFromImage);
	std::vector<int> startCompression();
};