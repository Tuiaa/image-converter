#include "CompressionHelper.h"
#include "BitmapHelper.h"
#include "Bitmap.h"
#include <iostream>
#include <vector>

int CompressionHelper::calculateHowManyChunksAreNeeded(int width, int height) {

	//int horizontalChunks = width / 4;
	int horizontalChunks = width / 2;
	//int verticalChunks = height / 4;
	int verticalChunks = height / 2;

	int amountOfChunks = horizontalChunks * verticalChunks;
	totalAmountOfChunks = amountOfChunks;

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

	addChunksUsingRealValues();

	std::cout << "\nout of for loop ";
	// print values for test

}

void CompressionHelper::setTheSettingsToBeUsed(int givenChunkSize, int givenArrayWidth, int givenArrayHeight) {
	chunkSize = givenChunkSize;	//should be 4
	// otetaan ekojen chunkkien määrä arraysta (testi arraysta 12 ekaa numeroa) --> arraywidth * chunksize
	amountOfFirstValues = givenArrayWidth * chunkSize;	//array width * chunksize
	howManyRowsInTotal = amountOfFirstValues / givenArrayWidth;	// amountOfFirstValues / arrayWidth

	amountOfChunksFromWidth = givenArrayWidth / chunkSize;		// width / chunksize
	amountOfChunksFromHeight = givenArrayHeight / chunkSize;		// height / chunksize

	rowLength = givenArrayWidth;	// arraywidth
}

void CompressionHelper::addChunksUsingRealValues() {


	int whatcurrentChunkShouldBe = currentRow * amountOfChunksFromWidth;
	for (int i = 0; i < amountOfChunksFromHeight; i++) {
		addFirstRow();
		currentChunk = whatcurrentChunkShouldBe;
		addNextRows();

		whatcurrentChunkShouldBe = (currentRow - 1) * amountOfChunksFromWidth;
		currentChunk = whatcurrentChunkShouldBe;
		currentPixelData = 0;
		currentRowFromChunk = 0;
	}

	std::cout << "\nAll chunks populated";

}

void CompressionHelper::addFirstRow() {

	// amount of first values means how many items there are in the first row of chunks
	for (int i = 0; i < amountOfFirstValues; i++) {
		firstValues.push_back(testSmallerDataPix[i]);
		std::cout << "\nvalue added to first values: " << (int)testSmallerDataPix[i];
	}

	// take first row of that chunk
	for (int j = 0; j < rowLength; j++) {
		oneRow.push_back(firstValues[j]);
		std::cout << "\nvalue added to one row: " << (int)firstValues[j];
	}

	currentPixelFromThisRow = 0;
	std::cout << "\nadding first row values to chunks";
	// go through saved row and add the values to chunks
	for (int x = 0; x < amountOfChunksFromWidth; x++) {
		std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
		for (int y = 0; y < chunkSize; y++) {
			std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
			std::cout << "\ncurrentPixelData = " << currentPixelData;
			// add to current chunk
			chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
			currentPixelFromArray++;
			currentPixelData++;
		}
		currentChunk++;
		currentPixelData = 0;
	}

	currentRow++;
	std::cout << "first row values added to chunks";

}

void CompressionHelper::addNextRows() {

	int howManyRepeats = 0;
	if (chunkSize == 4) {
		howManyRepeats = 3;
	}
	else if (chunkSize == 2) {
		howManyRepeats == 2;
	}

	for (int repeats = 0; repeats < howManyRepeats; repeats++) {
		// add next rows
		currentRowFromChunk++;
		std::vector<int> nextRow;

		int nextRowLastItem = (currentRowFromChunk + 1) * rowLength;

		// calculate the next row starting point
		int startingNumber = currentRowFromChunk * rowLength;
		for (int a = startingNumber; a < nextRowLastItem; a++) {
			nextRow.push_back(firstValues[a]);
			std::cout << "\nvalue added to next row: " << (int)firstValues[a];
		}
		oneRow.clear();
		oneRow = nextRow;

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
		std::cout << "\nadding next row values to chunks";
		// go through saved row and add the values to chunks
		for (int x = 0; x < amountOfChunksFromWidth; x++) {
			std::cout << "\ninside for x < rowlength, current pixel from array: " << currentPixelFromArray;
			for (int y = 0; y < chunkSize; y++) {
				std::cout << "\ninside for y < chunkSize, currentChunk = " << currentChunk;
				std::cout << "\ncurrentPixelData = " << currentPixelData;
				// add to current chunk
				chunks[currentChunk].pixelData[currentPixelData].colorValue = oneRow[currentPixelFromThisRow];
				currentPixelFromArray++;
				currentPixelData++;
			}
			currentChunk++;
			currentPixelData = currentPixelDataOnThisRow;
		}
		std::cout << "\nnext row values added to chunks, currentPixelFromArray = " << currentPixelFromArray;

		currentRow++;
	}

	std::cout << "\nOut of for loop";
}