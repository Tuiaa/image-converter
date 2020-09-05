#pragma once

/*
 *	- Block of bytes is at the start of the bitmap file
 *	- Used to identify the file
 *	- It's size is 14 bytes
 */
struct BITMAPFILEHEADER2
{
	// First 2 bytes are the characters of bitmap file are "B" and "M" in ASCII
	unsigned char fileTypeSignature[2] = { 'B', 'M' };
	// Next is the size of the BMP in bytes (Bitmap file header 14 bytes + DIB header 40 bytes)
	unsigned char sizeOfBitmapHeader = 14;
	// Next is reserved bytes, should be 0?
	unsigned char reservedBytes = 0;
	// Data offset, i.e. starting address of the byte where the bitmap image data (pixel array) can be found
	unsigned char info[54];
};

/*
 *	- Bitmap information header
 *	- Tells the application detailed information about the image
 *	- Information is used to display the image on the screen
 */
struct DIBHEADER2 {
	// 0E the size of this header
	unsigned char sizeOfDIB = 40;
	// 12 the bitmap width in pixels
	int width;
	// 14 the bitmap height in pixels
	int height;
	// 16 the number of color planes, must be 1?
	/*
	 *	- Occurs after bitmap header and dib header (and optional extra bit masks)
	 *	- Is a block of bytes (a table) listing the colors used by the image
	 *	- Number of entries in the color table palette is either 2*n (where n is number of bits per pixel)
	 *	  or a smaller number specified in the header
	 */
	unsigned char colorPlanes = 1;
	// 18 the number of bits per pixel
	int bitsPerPixel = 3;		// red, green, blue
	// image size
	// horizontal reso
	// vertical reso
	// number of colors in color palette --> 0 to default
	// num of important color used --> bytes/pixel * 8?
};



/*
 *	- The bits representing the bitmap pixels are packed in rows
 *	- The size of each row is rounded up to a multiple of 4 bytes by padding
 *	- For images with height above 1, multiple padded rows are stored consecutively, forming a pixel array
 */
struct PIXELARRAY2 {

};

class Bitmap {
public:
	// Bitmap file header
	BITMAPFILEHEADER2 bitmapFileHeader;
	// DIB header?
	DIBHEADER2 dibHeader;

	// Color table, its offset is the size of the BITMAPFILEHEADER + the size of the DIBHEADER (+ optional masks)
	//COLORTABLE2 colorTable;

	int bytesPerPixel;
	// Pixel array
	unsigned char* pixelData;

};