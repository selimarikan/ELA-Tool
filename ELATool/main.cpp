#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>

using namespace cv;

const int			jpegQMax = 100;
const char*			windowName = "ELATool - S.Arikan (selimarikan@gmail.com)";
char*				originalName = "image.jpg";
char*				imageName;
char*				lowQImgName = "imagelow.jpg";
int					jpegQVal = 80;
Mat					image, imageLow, resultImg;

void OnJpegTrackbar(int, void*)
{
	std::vector<int> args;
	args.push_back(CV_IMWRITE_JPEG_QUALITY);
	args.push_back(jpegQVal);
	imwrite(lowQImgName, image, args);
}

int main(int argc, char** argv) 
{
	if (argv[1] == nullptr)
	{
		std::cout << "Usage: ELATool image.jpg\nUsage: Drag & Drop image file over EXE\n";
		return 0;
	}

	// Read the image for ELA procedure.
	image = imread(originalName, 1);

	if (!image.data) 
	{
		imageName = argv[1];
		image = imread(imageName, 1);
	}
	else 
	{
		imageName = originalName;
	}
	
	namedWindow(windowName, CV_WINDOW_KEEPRATIO);
	createTrackbar("Level", windowName, &jpegQVal, jpegQMax, OnJpegTrackbar); 

	// Initialize low quality image for comparison
	OnJpegTrackbar(80, 0);
	imageLow = imread(lowQImgName, 1);

	// Compute absolute difference of two images (original and low quality)
	absdiff(image, imageLow, resultImg);

	for (;;) 
	{
		image = imread(imageName, 1);
		imageLow = imread(lowQImgName, 1);
		absdiff(image, imageLow, resultImg);
		// Multiply the result for easy recognition
		multiply(resultImg, resultImg, resultImg);
		imshow(windowName, resultImg);
		OnJpegTrackbar(jpegQVal, 0);
		waitKey(1);
	}
	return 0;
}