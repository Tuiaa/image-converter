#pragma once

class DDSHelper {
public:
	void getSurfaceInfo(size_t width, size_t height, size_t* outNumBytes, size_t* outRowBytes, size_t* outNumRows);
};