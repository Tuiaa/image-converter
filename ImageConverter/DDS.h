#pragma once


typedef enum DXGI_FORMAT { DXGI_FORMAT_BC1_UNORM };

typedef enum D3D10_RESOURCE_DIMENSION {
	D3D10_RESOURCE_DIMENSION_UNKNOWN,
	D3D10_RESOURCE_DIMENSION_BUFFER,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D
};

// typedef commonly used with DDS files
typedef unsigned long DWORD;

typedef unsigned int UINT;

/*
 *		DDS Header
 *		- describes the file header
 */
struct DDS_HEADER {
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwHeight;
	DWORD           dwWidth;
	DWORD           dwPitchOrLinearSize;
	DWORD           dwDepth;
	DWORD           dwMipMapCount;
	DWORD           dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	DWORD           dwCaps;
	DWORD           dwCaps2;
	DWORD           dwCaps3;
	DWORD           dwCaps4;
	DWORD           dwReserved2;
};

/*
 *		DDS Pixel Format
 *		- surface pixel format
 */
struct DDS_PIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};

/*
 *		DDS Header DXT10
 *		- DDS header extension that handles resource arrays and additional data
 */
struct DDS_HEADER_DXT10 {
	DXGI_FORMAT              dxgiFormat;
	D3D10_RESOURCE_DIMENSION resourceDimension;
	UINT                     miscFlag;
	UINT                     arraySize;
	UINT                     miscFlags2;
};

class DDS {
public:
	DWORD dwMagic;
	DDS_HEADER header;
	DDS_HEADER_DXT10 header10;
	unsigned char* bdata;
	unsigned char* bdata2;
};