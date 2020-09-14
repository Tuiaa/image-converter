#pragma once
#include <vector>
#include <string>

/*
 *		Pixel Info
 *		- has the color value information and also the position it was in the original
 *		  pixel array so it's easier to put this back together after splitting the image into chunks
 */
struct PixelInfo {
	int colorValueOfPixelR;
	char colorValueOfPixelRchar;
	int colorValueOfPixelG;
	int colorValueOfPixelB;
	int pixelRPositionInArray;
	int pixelGPositionInArray;
	int pixelBPositionInArray;
};

/*
 *		Pixel Chunk
 *		- Has PixelInfo vector inside of it, which stores the RGB value of the pixel
 *		- color_0, color_1, color_2 and color_3 are the color palette created after doing BC1 compression
 *		- indices are the 2 bit values representing which color each of the pixel in 4 x 4 texel chunk has
 *		  after compression
 */
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
	int currentChunk = 0;
	int currentPixelNumPosition = 0;

	std::vector<int> pixelsNeededForOneRowOfChunks;		// one row of chunks is 4 * image width pixels
	std::vector<int> pixelsOfOneRowOfImage;				// one row of image is image width amount of pixels

	std::vector<PixelChunk> allChunks;
	std::vector<int> vectorPixelDataFromBitmap;
	unsigned char* charArrayPixelDataFromBitmap;
	std::vector<int> pixelDataAfterCompression;

	/*	Initialization functions	*/
	void initializeVectorOfChunks();
	std::vector<PixelInfo> initializeVectorOfPixelInfo();
	void initializePixelDataAfterCompressionVector();

	/*	Helper functions  */
	int calculateHowManyChunksAreNeeded(int width, int height);
	void calculateAllPixelsNeededForRowOfChunks(int startingPoint);
	void calculatePixelsFromOneRowOfImage(int startingPoint);
	int getCurrentChunkStartingPixelPosition(int row);

	/*	 Image slicing	 */
	void sliceImageIntoOneChunkRow(int amountOfChunksPopulated, int startingPoint);
	void sliceImageIntoChunks();

	/*	 Compression	 */
	void goThroughPixelDataAndCompress();
	void calculateColorTableFromOneChunk(int chunkIndex);

	/*	 Combine image back into one array	 */
	void combineChunksBackToPixelVector();

public:
	void initializeSettingsForCompression(int imageWidth, int imageHeight, unsigned char* charArrayOfPixelsFromBitmap, std::vector<int> allPixelsFromBitmapVector);
	std::vector<int> startCompression();
};