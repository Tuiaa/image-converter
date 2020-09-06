#pragma once
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <vector>

#define CHUNKSIZE = 4;

struct PixelData {
	int x;
	int y;
	unsigned char colorValue;
};

struct PixelChunk {
	std::vector<PixelData> pixelData;
};

class CompressionHelper {
public:
	unsigned char* testSmallerDataPix;
	std::vector<PixelChunk> chunks;

	int calculateHowManyChunksAreNeeded(int width, int height);

	void initializeArrayOfChunks(int amountOfChunks);
	std::vector<PixelData> initializeArrayOfPixelData(int amountOfPixelDatas);

	void populateChunks(BitmapHelper bitmapHelper);
	void addFirstChunks();
	void addFirstRow();
	void addNextRows();



	int chunkSize = 2;	//should be 4
	// otetaan ekojen chunkkien määrä arraysta (testi arraysta 12 ekaa numeroa) --> arraywidth * chunksize
	int amountOfFirstValues = 6 * 2;	//array width * chunksize
	std::vector<int> firstValues;
	int howManyRowsInTotal = 12 / 6;	// amountOfFirstValues / arrayWidth

	int currentPixelData = 0;
	int currentPixelFromArray = 0;
	int currentPixelFromThisRow = 0;

	int totalAmountOfChunks;
	int amountOfChunksFromWidth = 6 / 2;		// width / chunksize

	int currentRowFromChunk = 0;
	int currentChunk = 0;



	int rowLength = 6;	// arraywidth
	std::vector<int> oneRow;
};