#pragma once

// DDS Header Flags
#define DDS_MAGIC						0x20534444		// "DDS "
#define PIXELFORMAT_DXT1				68888449
#define DDS_HEADER_DWSIZE				124				// DDS header size is always 124
#define DDSD_CAPS						0x00000001		// Required in every .dds file
#define DDSD_HEIGHT						0x00000002		// Required in every .dds file
#define DDSD_WIDTH						0x00000004		// Required in every .dds file
#define DDSD_PIXELFORMAT				0x00001000		// Required in every .dds file
#define DDSD_LINEARSIZE					0x00080000		// Required when pitch is provided for compressed texture

#define DDSCAPS_TEXTURE					0x00001000		// DDS_HEADER documentary says this is required

#define DDS_PIXELFORMAT_DWSIZE			32				// This is always 32
#define DDS_PIXELFORMAT_DWRGBBITCOUNT	16				// Number of bits in an RGB
#define DDPF_FOURCC						0x00000004		// Tells that image contains compressed RGB data

typedef unsigned long DWORD;
typedef unsigned int UINT;

typedef enum DXGI_FORMAT { DXGI_FORMAT_BC1_UNORM };
typedef enum D3D10_RESOURCE_DIMENSION {	D3D10_RESOURCE_DIMENSION_TEXTURE2D };

/*
 *		DDS Pixel Format
 *		- surface pixel format
 */
struct DDS_PIXELFORMAT {
	DWORD dwSize;									// Structure size, always 32		
	DWORD dwFlags;									// Values which indicate what type of data is in the surface
	DWORD dwFourCC;									// Four-character codes for specifying compressed or custom formats
	DWORD dwRGBBitCount;							// Number of bits in an RGB
	DWORD dwRBitMask;								// Red mask for reading color data
	DWORD dwGBitMask;								// Green mask for reading color data
	DWORD dwBBitMask;								// Blue mask for reading color data
	DWORD dwABitMask;								// Alpha mask for reading color data
};

/*
 *		DDS Header
 *		- describes the file header
 */
struct DDS_HEADER {
	DWORD           dwSize;						// Size of structure, is 124
	DWORD           dwFlags;					// Flags that indicate which members contain valid data
	DWORD           dwHeight;					// Surface height (in pixels)
	DWORD           dwWidth;					// Surface width (in pixels)
	DWORD           dwPitchOrLinearSize;		// The total number of bytes in the top level texture for a compressed texture
	DWORD           dwDepth;					// Depth of a volume texture (in pixels), otherwise unused.
	DWORD           dwMipMapCount;				// Number of mipmap levels, otherwise unused
	DWORD           dwReserved1[11];			// Unused
	DDS_PIXELFORMAT pixelFormat;				// Surface pixel format
	DWORD           dwCaps;						// Specifies the complexity of the surfaces stored.
	DWORD           dwCaps2;					// Additional detail about the surfaces stored
	DWORD           dwCaps3;					// Unused
	DWORD           dwCaps4;					// Unused
	DWORD           dwReserved2;				// Unused
};

/*
 *		DDS Header DXT10
 *		- DDS header extension that handles resource arrays and additional data
 */
struct DDS_HEADER_DXT10 {
	DXGI_FORMAT              dxgiFormat;		// The surface pixel format
	D3D10_RESOURCE_DIMENSION resourceDimension;	// Identifies the type of resource
	UINT                     miscFlag;			// Identifies other, less common options for resources
	UINT                     arraySize;			// The number of elements in the array
	UINT                     miscFlags2;		// Contains additional metadata (formerly was reserved)
};

class DDS {
public:
	DWORD dwMagic;
	DDS_HEADER header;
	DDS_HEADER_DXT10 header10;
	unsigned char* bdata;
	unsigned char* bdata2;

	int totalSizeOfTheFile;
};