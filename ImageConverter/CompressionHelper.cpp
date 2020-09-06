#include "CompressionHelper.h"
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <iostream>
#include <vector>

int CompressionHelper::calculateHowManyChunksAreNeeded(int width, int height) {

	// switch back!
	//int horizontalChunks = width / 4;
	int horizontalChunks = width / 2;
	//int verticalChunks = height / 4;
	int verticalChunks = height / 2;

	int amountOfChunks = horizontalChunks * verticalChunks;

	std::cout << "\namount of chunks: " << amountOfChunks;
	return amountOfChunks;
}

void CompressionHelper::initializeArrayOfChunks(int amountOfChunks) {
	std::vector<PixelChunk> createdChunks;
	for (int i = 0; i < amountOfChunks; i++) {
		//std::vector<PixelData> chunk = initializeArrayOfPixelData(16);
		std::vector<PixelData> chunk = initializeArrayOfPixelData(4);
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

void CompressionHelper::populateChunks(BitmapHelper bitmapHelper) {
	int currentRow = 0;
	int currentColumn = 0;

	int currentChunk = 0;
	int currentPixelData = 0;

	int currentPixelInARow = 0;
	//int howManySlices = bitmapHelper.bitmap.dibHeader.width / 4;	// switch back to 4
	int howManySlices = 2;	// switch back to smthing
	int currentChunkEndPoint = howManySlices;

	//int arrayWidth = bitmapHelper.bitmap.dibHeader.width - 1;
	int arrayWidth = 6;
	int arrayHeight = 4;

	// create test data set
	testSmallerDataPix = new unsigned char[24];
	std::fill(testSmallerDataPix, testSmallerDataPix + 24, 5);

	for (int y = 0; y < 24; y++) {
		//std::cout << "\nvalue in smaller data pix: " << (int)testSmallerDataPix[y];
	}

	// first row
	addFirstChunks();

	std::cout << "\nout of for loop ";
	// print values for test

}

void CompressionHelper::addFirstChunks() {
	// otetaan ekojen chunkkien määrä arraysta (testi arraysta 12 ekaa numeroa) --> arraywidth * chunksize
	int amountOfFirstValues = 6 * 2;	//array width * chunksize
	std::vector<int> firstValues;
	int howManyRowsInTotal = 12 / 6;	// amountOfFirstValues / arrayWidth

	int currentPixelData = 0;
	int currentPixelFromArray = 0;

	int amountOfChunks = 6 / 2;		// width / chunksize

	int currentRow = 0;
	int currentChunk = 0;

	int chunkSize = 2;	//should be 4

	for (int i = 0; i < amountOfFirstValues; i++) {
		firstValues.push_back(testSmallerDataPix[i]);
		std::cout << "\nvalue added to first values: " << (int)testSmallerDataPix[i];
	}

	int rowLength = 6;	// arraywidth
	std::vector<int> oneRow;

	for (int j = 0; j < rowLength; j++) {
		oneRow.push_back(firstValues[j]);
		std::cout << "\nvalue added to one row: " << (int)firstValues[j];
	}

	std::cout << "\nadding first row values to chunks";
	// go through saved row and add the values to chunks
	for (int x = 0; x < amountOfChunks; x++) {
		std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int y = 0; y < chunkSize; y++) {
			std::cout << "\ninside for y < chunkSize";
			// add to current chunk
			chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromArray];
			currentPixelFromArray++;
			currentPixelData++;
		}
		currentChunk++;
		currentPixelData = 0;
	}
	std::cout << "first row values added to chunks";

	// add next row
	currentRow++;
	currentChunk = 0;
	std::vector<int> nextRow;

	int nextRowLastItem = (currentRow + 1) * rowLength;

	// calculate the next row starting point
	int startingNumber = currentRow * rowLength;
	for (int a = startingNumber; a < nextRowLastItem; a++) {
		nextRow.push_back(firstValues[a]);
		std::cout << "\nvalue added to next row: " << (int)firstValues[a];
	}
	oneRow.clear();
	oneRow = nextRow;

	if (currentRow == 1) {
		if(chunkSize == 2) currentPixelData = 2;
		if (chunkSize == 4) currentPixelData = 4;
	}
	else if (currentRow == 2) {
		//if (chunkSize == 2) currentPixelData = 2;
		if (chunkSize == 4) currentPixelData = 8;
	}
	else if (currentRow == 3) {
		//if (chunkSize == 2) currentPixelData = 2;
		if (chunkSize == 4) currentPixelData = 12;
	}

	int currentPixelDataOnThisRow = currentPixelData;
	int currentPixelFromThisRow = 0;

	std::cout << "\nadding next row values to chunks";
	// go through saved row and add the values to chunks
	for (int x = 0; x < amountOfChunks; x++) {
		std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int y = 0; y < chunkSize; y++) {
			std::cout << "\ninside for y < chunkSize";
			// add to current chunk
			chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
			currentPixelFromArray++;
			currentPixelData++;
		}
		currentChunk++;
		currentPixelData = currentPixelDataOnThisRow;
	}
	std::cout << "next row values added to chunks";

	std::cout << "\nOut of for loop";
}