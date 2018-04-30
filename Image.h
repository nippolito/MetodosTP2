#ifndef __IMAGE_H__
#define __IMAGE_H__


#include "ppmloader/ppmloader.cpp"
#include <string>

class Image
{
public:
	Image(std::string newFilePath);
	~Image();

	void SaveImage(std::string newFilePath);
	uchar obtainPixelValue(unsigned int pixelNumber);
	void aplanateImageArray(uchar* array);

	std::string filePath;
	int height;
	int width;
private:
	uchar* imageBuffer;

	
};

#endif //__IMAGE_H__
	