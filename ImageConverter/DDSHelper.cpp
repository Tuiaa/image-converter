#define _CRT_SECURE_NO_WARNINGS
#include "DDS.h"
#include "DDSHelper.h"
#include <algorithm>

// DDS Header Flags

#define DDS_MAGIC 0x20534444 // "DDS "
/* When you write .dds files, you should set the DDSD_CAPS and DDSD_PIXELFORMAT flags,
and for mipmapped textures you should also set the DDSD_MIPMAPCOUNT flag. However, when you
read a .dds file, you should not rely on the DDSD_CAPS, DDSD_PIXELFORMAT, and DDSD_MIPMAPCOUNT
flags being set because some writers of such a file might not set these flags. */
#define DWSIZE = 124;

#define DDSD_CAPS						0x00000001		// required
#define DDSD_HEIGHT						0x00000002		// required
#define DDSD_WIDTH						0x00000004		// required
#define DDSD_PITCH						0x00000008
#define DDSD_PIXELFORMAT				0x00001000		// required
#define DDSD_MIPMAPCOUNT				0x00020000
#define DDSD_LINEARSIZE					0x00080000
#define DDSD_DEPTH						0x00800000


#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDPF_ALPHAPIXELS				0x00000001
#define DDPF_ALPHA						0x00000002
#define DDPF_FOURCC						0x00000004
#define DDPF_INDEXED					0x00000020
#define DDPF_RGB						0x00000040
#define DDPF_YUV						0x00000200
#define DDPF_LUMINANCE					0x00020000
#define DDPF_BUMPDUDV					0x00080000

#define DDSCAPS_TEXTURE					0x00001000

#define SIZE_T_BITSPERPIXEL = 4;	// DXGI_FORMAT_BC1_TYPELESS format size is 4

void DDSHelper::getSurfaceInfo(size_t width, size_t height, size_t* outNumBytes, size_t* outRowBytes, size_t* outNumRows) {
	size_t numBytes = 0;
	size_t rowBytes = 0;
	size_t numRows = 0;

	bool bc = false;
	bool packed = false;
	size_t bcnumBytesPerBlock = 0;

	// for DXGI_FORMAT_BC1_TYPELESS format
	bc = true;
	bcnumBytesPerBlock = 8;

	if (bc)
	{
		size_t numBlocksWide = 0;
		if (width > 0)
		{
			numBlocksWide = std::max<size_t>(1, (width + 3) / 4);
		}
		size_t numBlocksHigh = 0;
		if (height > 0)
		{
			numBlocksHigh = std::max<size_t>(1, (height + 3) / 4);
		}
		rowBytes = numBlocksWide * bcnumBytesPerBlock;
		numRows = numBlocksHigh;

		numBytes = rowBytes * numRows;
		if (outNumBytes)
		{
			*outNumBytes = numBytes;
		}
		if (outRowBytes)
		{
			*outRowBytes = rowBytes;
		}
		if (outNumRows)
		{
			*outNumRows = numRows;
		}
	}

	// get dxgi format

	// fill init data

	// create d3dresources

	// create texture from dds

	// create dds texture from memory

}

void DDSHelper::readDDSImageFromFile(const char *fileName)
{
	//int width;
	//int height;
	//int bytesPerPixel;

	//FILE *imageFile = fopen(fileName, "rb");				// Opens the image file

		// lay out variables to be used
	

	unsigned int width;
	unsigned int height;
	//unsigned int mipMapCount;

	unsigned int blockSize;
	unsigned int format;

	unsigned int w;
	unsigned int h;


	//GLuint tid = 0;
	dds = DDS();
	// open the DDS file for binary reading and get file size
	FILE* f = fopen(fileName, "rb");

	// get file size
	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	totalSize = file_size;

	magicValue = new unsigned char[4];
	fread(magicValue, 1, 4, f);

	// reads file header
	readHeader = new unsigned char[124];
	//fseek(f, 0, SEEK_SET);
	fread(readHeader, 1, 124, f);

	// reads rest of the file
	int sizewithoutstart = totalSize - 128;
	readFullFileExceptHeaderAndMagic = new unsigned char[sizewithoutstart];
	//fseek(f, 127, SEEK_SET);
	fread(readFullFileExceptHeaderAndMagic, 1, sizewithoutstart, f);

	//readFullFileExceptHeaderAndMagic = new unsigned char[totalSize - 128];
	//fread(readFullFileExceptHeaderAndMagic, 1, totalSize, f);

	readFullFile = new unsigned char[file_size];
	fseek(f, 0, SEEK_SET);
	fread(readFullFile, 1, file_size, f);

	//readHeader10 = new unsigned char[32];
	//fseek(f, 128, SEEK_SET);
	//fread(readHeader10, 1, 32, f);



	// compare the `DDS ` signature
	//if (memcmp(header, "DDS ", 4) != 0)
		//goto exit;

	// extract height, width, and amount of mipmaps - yes it is stored height then width
	height = (readHeader[12]) | (readHeader[13] << 8) | (readHeader[14] << 16) | (readHeader[15] << 24);
	width = (readHeader[16]) | (readHeader[17] << 8) | (readHeader[18] << 16) | (readHeader[19] << 24);
	//mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);

	// figure out what format to use for what fourCC file type it is
	// block size is about physical chunk storage of compressed data in file (important)
	format = DXGI_FORMAT_BC1_UNORM;
	blockSize = 8;

	// allocate new unsigned char space with file_size - (file_code + header_size) magnitude
	// read rest of file
	long size = file_size - 128 - 32;
	pixelData = new unsigned char[file_size - 128];
	//buffer = malloc(file_size - 128);
	//if (buffer == 0)
	//	goto exit;
	//fseek(f, 128, SEEK_SET);		// 128 + 32
	//fread(pixelData, sizeof(unsigned char), file_size, f);

	// prepare new incomplete texture
	//glGenTextures(1, &tid);
	//if (tid == 0)
	//	goto exit;

	// bind the texture
	// make it complete by specifying all needed parameters and ensuring all mipmaps are filled
	//glBindTexture(GL_TEXTURE_2D, tid);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1); // opengl likes array length of mipmaps
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// prepare some variables
	//unsigned int offset = 0;
	//unsigned int size = 0;
	//w = width;
	//h = height;

	// loop through sending block at a time with the magic formula
	// upload to opengl properly, note the offset transverses the pointer
	// assumes each mipmap is 1/2 the size of the previous mipmap
	/*for (unsigned int i = 0; i < mipMapCount; i++) {
		if (w == 0 || h == 0) { // discard any odd mipmaps 0x1 0x2 resolutions
			mipMapCount--;
			continue;
		}
		size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, w, h, 0, size, buffer + offset);
		offset += size;
		w /= 2;
		h /= 2;
	}
	// discard any odd mipmaps, ensure a complete texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	// easy macro to get out quick and uniform (minus like 15 lines of bulk)
exit:
	free(buffer);
	free(header);*/
	fclose(f);
	//return tid;

	saveDDSValues(width, height, pixelData);

}

void DDSHelper::saveDDSValues(int width, int height, unsigned char *pixelDataFromFile) {

	//dds = DDS();

	dds.dwMagic = DDS_MAGIC;

	dds.header.dwSize = 124;
	dds.header.dwFlags = (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT);
	dds.header.dwHeight = height;
	dds.header.dwWidth = width;
	unsigned long pitch = std::max(1, ((width + 3) / 4)) * 8;		// block size is 8 bytes for DXT1, BC1 formats
	dds.header.dwPitchOrLinearSize = pitch;

	// is this needed? dds.header.dwDepth = depth;	// deph of a volume texture (in pixels), otherwise unused
	// mipmaps unused
	// dwreserved, unused
	DDS_PIXELFORMAT pixelFormat;
	pixelFormat.dwSize = 32;		// structure size, is set to 32
	pixelFormat.dwFlags = (DDPF_RGB | DDPF_LUMINANCE);
	//pixelFormat.dwRBitMask = 0x00ff0000;
	pixelFormat.dwRGBBitCount = 16;
	pixelFormat.dwRBitMask = 0x00ff0000;
	pixelFormat.dwGBitMask = 0x0000ff00;
	pixelFormat.dwBBitMask = 0x000000ff;

	dds.header.ddspf = pixelFormat;
	dds.header.dwCaps = DDSCAPS_TEXTURE;

	dds.header10.dxgiFormat = DXGI_FORMAT_BC1_UNORM;
	dds.header10.resourceDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
	dds.bdata = pixelDataFromFile;
}

void DDSHelper::writeDDSFile(const char *fileName)
{
	FILE *outputFile = fopen(fileName, "wb");

	//int totalSize = 64 + 124 + 32;

	// Bitmap File Header
	//fwrite(&bitmap.bitmapFileHeader.BM[0], 1, 1, outputFile);
	//fwrite(&dds.dwMagic, 4, 1, outputFile);								// length 4
	//fwrite(&bitmap.bitmapFileHeader.BM[1], 1, 1, outputFile);
	fwrite(magicValue, 4, 1, outputFile);
	fwrite(readHeader, 124, 1, outputFile);
	long size = totalSize - 128;
	fwrite(readFullFileExceptHeaderAndMagic, size, 1, outputFile);
	//fwrite(readHeader10, 32, 1, outputFile);

	// Add the pixel data
	/*for (int i = 0; i < totalSize; i++)
	{
		fwrite(&pixelData, 1, 1, outputFile);
	}*/

	fclose(outputFile);
}