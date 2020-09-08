#include "DDS.h"
#include "DDSHelper.h"
#include <algorithm>

typedef enum DXGI_FORMAT { DXGI_FORMAT_BC1_UNORM };

typedef enum D3D10_RESOURCE_DIMENSION {
	D3D10_RESOURCE_DIMENSION_UNKNOWN,
	D3D10_RESOURCE_DIMENSION_BUFFER,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D
};
// DDS Header Flags

#define DDS_MAGIC 0x20534444 // "DDS "
/* When you write .dds files, you should set the DDSD_CAPS and DDSD_PIXELFORMAT flags, 
and for mipmapped textures you should also set the DDSD_MIPMAPCOUNT flag. However, when you 
read a .dds file, you should not rely on the DDSD_CAPS, DDSD_PIXELFORMAT, and DDSD_MIPMAPCOUNT 
flags being set because some writers of such a file might not set these flags. */
#define DWSIZE = 124;

#define DDSD_CAPS                  0x00000001
#define DDSD_HEIGHT                0x00000002
#define DDSD_WIDTH                 0x00000004
#define DDSD_PITCH                 0x00000008
#define DDSD_PIXELFORMAT           0x00001000
#define DDSD_MIPMAPCOUNT           0x00020000
#define DDSD_LINEARSIZE            0x00080000
#define DDSD_DEPTH                 0x00800000


#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDPF_ALPHAPIXELS           0x00000001
#define DDPF_ALPHA                 0x00000002
#define DDPF_FOURCC                0x00000004
#define DDPF_INDEXED               0x00000020
#define DDPF_RGB                   0x00000040
#define DDPF_YUV                   0x00000200
#define DDPF_LUMINANCE             0x00020000
#define DDPF_BUMPDUDV              0x00080000

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
	int width;
	int height;
	int bytesPerPixel;

	FILE *imageFile = fopen(fileName, "rb");				// Opens the image file

	fseek(imageFile, DDS_WIDTH, SEEK_SET);
	fread(&width, 4, 1, imageFile);							// The size is 4 bytes (value from BMP table)
	fseek(imageFile, DDS_HEIGHT, SEEK_SET);
	fread(&height, 4, 1, imageFile);

	// save bits per pixel value
	short bitsPerPixel;
	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
	fread(&bitsPerPixel, 2, 1, imageFile);					// it's size 2 bytes so it has to be short
	bytesPerPixel = (int)bitsPerPixel / 8;

	// Size is the value of image height and width multiplied, and that's multiplied by 24*8 (24 bitmap)
	int size = (height * (width *(24 / 8)));
	int rowSize = width * bytesPerPixel;
	totalSize = rowSize * height;

	int dataOffset;
	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(&dataOffset, 4, 1, imageFile);

	unsigned char* tempPixelArray = new unsigned char[totalSize];
	fseek(imageFile, dataOffset, SEEK_SET);
	fread(tempPixelArray, sizeof(unsigned char), totalSize, imageFile);

	fclose(imageFile);
	int depth = 0;
	saveDDSValues(width, height, bytesPerPixel, depth, tempPixelArray);
}

void DDSHelper::saveDDSValues(int width, int height, int depth, int bytesPerPixel, unsigned char *pixelDataFromFile) {

	//bitmap = Bitmap();
	dds = DDS();

	/*// Bitmap File Header
	bitmap.bitmapFileHeader.filesize = height + HEADER_SIZE + INFO_HEADER_SIZE;
	bitmap.bitmapFileHeader.dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;

	// DIB Header
	bitmap.dibHeader.infoHeaderSize = INFO_HEADER_SIZE;
	bitmap.dibHeader.width = width;
	bitmap.dibHeader.height = height;
	bitmap.dibHeader.bitsPerPixel = bytesPerPixel * 8;
	bitmap.dibHeader.compression = NO_COMPRESION;
	bitmap.dibHeader.imageSize = width * height * bytesPerPixel;
	bitmap.dibHeader.horizontalResolution = 11811;						//300 dpi
	bitmap.dibHeader.verticalResolution = 11811;						//300 dpi
	bitmap.dibHeader.colorsUsedInColorPalette = MAX_NUMBER_OF_COLORS;
	bitmap.dibHeader.importantColors = ALL_COLORS_REQUIRED;

	// General
	bitmap.bytesPerPixel = bytesPerPixel;
	bitmap.pixelData = pixelDataFromFile;*/

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
	//pixelFormat.dwFlags = (DDPF_ALPHAPIXELS | DDPF_ALPHA | DDPF_FOURCC | DDPF_RGB | DDPF_YUV | DDPF_LUMINANCE);
	//pixelFormat.dwRBitMask = 0x00ff0000;
}