#define _CRT_SECURE_NO_WARNINGS
#include "DDS.h"
#include "DDSHelper.h"
#include <iostream>
#include <algorithm>
#include <vector>

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

void DDSHelper::readDDSFileFromImageLonger(const char* fileName) {

	//GLuint tid = 0;
	dds = DDS();
	// open the DDS file for binary reading and get file size
	FILE* f = fopen(fileName, "rb");

	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	totalSize = file_size;

	//read magic value
	fread(&dds.dwMagic, 1, 4, f);

	// read header
	fread(&dds.header.dwSize, 1, 4, f);
	fread(&dds.header.dwFlags, 1, 4, f);
	fread(&dds.header.dwHeight, 1, 4, f);
	fread(&dds.header.dwWidth, 1, 4, f);
	fread(&dds.header.dwPitchOrLinearSize, 1, 4, f);
	fread(&dds.header.dwDepth, 1, 4, f);
	fread(&dds.header.dwMipMapCount, 1, 4, f);
	fread(&dds.header.dwReserved1, 1, 44, f);
	// read pixel format
	fread(&dds.header.ddspf.dwSize, 1, 4, f);
	fread(&dds.header.ddspf.dwFlags, 1, 4, f);
	fread(&dds.header.ddspf.dwFourCC, 1, 4, f);
	fread(&dds.header.ddspf.dwRGBBitCount, 1, 4, f);
	fread(&dds.header.ddspf.dwRBitMask, 1, 4, f);
	fread(&dds.header.ddspf.dwGBitMask, 1, 4, f);
	fread(&dds.header.ddspf.dwBBitMask, 1, 4, f);
	fread(&dds.header.ddspf.dwABitMask, 1, 4, f);

	////header continues
	fread(&dds.header.dwCaps, 1, 4, f);
	fread(&dds.header.dwCaps2, 1, 4, f);
	fread(&dds.header.dwCaps3, 1, 4, f);
	fread(&dds.header.dwCaps4, 1, 4, f);
	fread(&dds.header.dwReserved2, 1, 4, f);

	// header10 reading
	fread(&dds.header10.dxgiFormat, 1, 4, f);
	fread(&dds.header10.resourceDimension, 1, 4, f);
	fread(&dds.header10.miscFlag, 1, 4, f);
	fread(&dds.header10.arraySize, 1, 4, f);
	fread(&dds.header10.miscFlags2, 1, 4, f);




	/*readHeader10 = new unsigned char[20];
	//fseek(f, 0, SEEK_SET);
	fread(readHeader10, 1, 20, f);*/

	int sizewithoutstart = totalSize - 128 - 20;
	int imageSize = 131072;		// DXT1 compressed images have 4 bitsPerPixel --> 4 * imagewidth *imageheight
	endSettingsSize = sizewithoutstart - imageSize;

	// reads image pixel data
	imagePixelDataMaybe = new unsigned char[imageSize];
	//fseek(f, 0, SEEK_SET);
	fread(imagePixelDataMaybe, 1, imageSize, f);

	// reads end settings
	endSettingsData = new unsigned char[endSettingsSize];
	//fseek(f, 0, SEEK_SET);
	fread(endSettingsData, 1, endSettingsSize, f);



	readFullFile = new unsigned char[file_size];
	fseek(f, 0, SEEK_SET);
	fread(readFullFile, 1, file_size, f);


	fclose(f);
	//return tid;

	for (int i = 0; i < imageSize; i++) {
		imagePixelDataMaybeVector.push_back(imagePixelDataMaybe[i]);
	}

	for (int j = 0; j < file_size; j++) {
		readFullFileVector.push_back(readFullFile[j]);
	}


	saveDDSDefaultValues();
}


/*
 *		Save DDS Default Values
 *		- Uses the values that are needed for DXT1 BC1 compressed image
 */
void DDSHelper::saveDDSDefaultValues() {

	//Adding DDS magic number default value
	dds.dwMagic = DDS_MAGIC;

	// Adding DDS_HEADER default values
	dds.header.dwSize = 124;
	dds.header.dwFlags = (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT);
	int width2 = dds.header.dwWidth;
	int height2 = dds.header.dwHeight;
	unsigned long pitch = std::max(1, ((width2 + 3) / 4)) * 8;		// block size is 8 bytes for DXT1 format, calculation is from documentation
	dds.header.dwPitchOrLinearSize = pitch;
	dds.header.dwDepth = 0;
	dds.header.dwMipMapCount = 0;
	for (int i = 0; i < 11; i++) {
		dds.header.dwReserved1[i] = 0;
	}

	// Adding DDS_PIXELFORMAT default values
	dds.header.ddspf.dwSize = 32;
	dds.header.ddspf.dwFlags = (DDPF_FOURCC);
	dds.header.ddspf.dwRGBBitCount = 16;
	dds.header.ddspf.dwRBitMask = 0x00ff0000;
	dds.header.ddspf.dwGBitMask = 0x0000ff00;
	dds.header.ddspf.dwBBitMask = 0x000000ff;
	dds.header.ddspf.dwABitMask = 0xff000000;

	// Continuing adding DDS_HEADER default values
	dds.header.dwCaps = DDSCAPS_TEXTURE;
	dds.header.dwCaps2 = 0;
	dds.header.dwCaps3 = 0;
	dds.header.dwCaps4 = 0;
	dds.header.dwReserved2 = 0;

	// Adding DDS_HEADER_DXT10 default values
	dds.header10.dxgiFormat = DXGI_FORMAT_BC1_UNORM;
	dds.header10.resourceDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
	dds.header10.miscFlag = 0;
	dds.header10.arraySize = 0;
	dds.header10.miscFlags2 = 0;

	/*for (int j = 0; j < endSettingsSize; j++) {
		endSettingsVector.push_back(0);
	}*/
}

void DDSHelper::writeDDSFile(const char *fileName, unsigned char* pixelDataFromBitmap, std::vector<int> pixelDataAsVector)
{
	FILE *outputFile = fopen(fileName, "wb");

	// Writing DDS Magic Number that contains four character code value 'DDS '
	fwrite(&dds.dwMagic, 4, 1, outputFile);

	// Writing DDS_HEADER
	fwrite(&dds.header.dwSize, 4, 1, outputFile);
	fwrite(&dds.header.dwFlags, 4, 1, outputFile);
	fwrite(&dds.header.dwHeight, 4, 1, outputFile);
	fwrite(&dds.header.dwWidth, 4, 1, outputFile);
	fwrite(&dds.header.dwPitchOrLinearSize, 4, 1, outputFile);
	fwrite(&dds.header.dwDepth, 4, 1, outputFile);
	fwrite(&dds.header.dwMipMapCount, 4, 1, outputFile);
	fwrite(&dds.header.dwReserved1, 44, 1, outputFile);

	// Writing DDS_PIXELFORMAT that's inside the header
	fwrite(&dds.header.ddspf.dwSize, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwFlags, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwFourCC, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwRGBBitCount, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwRBitMask, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwGBitMask, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwBBitMask, 4, 1, outputFile);
	fwrite(&dds.header.ddspf.dwABitMask, 4, 1, outputFile);

	// Continuing writing DDS_HEADER
	fwrite(&dds.header.dwCaps, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps2, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps3, 4, 1, outputFile);
	fwrite(&dds.header.dwCaps4, 4, 1, outputFile);
	fwrite(&dds.header.dwReserved2, 4, 1, outputFile);

	// Writing DDS_HEADER_DXT10
	fwrite(&dds.header10.dxgiFormat, 4, 1, outputFile);
	fwrite(&dds.header10.resourceDimension, 4, 1, outputFile);
	fwrite(&dds.header10.miscFlag, 4, 1, outputFile);
	fwrite(&dds.header10.arraySize, 4, 1, outputFile);
	fwrite(&dds.header10.miscFlags2, 4, 1, outputFile);

	int sizewithoutstart = totalSize - 128 - 20;
	int imageSize = 131072;
	int endSettingsSize = sizewithoutstart - imageSize;

	// Writing the pixel data
	fwrite(imagePixelDataMaybe, imageSize, 1, outputFile);


	/*for (int i = 0; i < pixelDataAsVector.size(); i++)
	{
		fwrite(&pixelDataAsVector[i], 1, 1, outputFile);
	}*/

	//fwrite(&endSettingsVector, endSettingsSize, 1, outputFile);
	fclose(outputFile);
}