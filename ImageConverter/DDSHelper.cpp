#include "DDS.h"
#include "DDSHelper.h"
#include <algorithm>

typedef enum DXGI_FORMAT { DXGI_FORMAT_BC1_TYPELESS };

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
#define DDSD_CAPS = 0x1;
#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH
#define DDSD_PITCH = 0x8;
#define DDSD_PIXELFORMAT = 0x1000;
//mipmap
#define DDSD_LINEARSIZE = 0x80000; // required when pitch is provided for a compressed texture
#define DDSD_DEPTH = 0x800000;

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

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