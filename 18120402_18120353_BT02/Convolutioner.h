#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;
using namespace std;

class Convolutioner
{
private:
	vector<float> _kernel;// kernel vector is used to do convolution
	int _kernelWidth;	  // kernel width
	int _kernelHeight;    // kernel height

public:
	// Get vector kernel
	vector<float> GetKernel() { return _kernel; };

	// Set kernel with parameters: kernel vector, kernel witdth, kernel height
	void setKernel(vector<float> kernel, int kWidth, int kHeight);

	/*
	Operate convolution using a declared kernel:
		sourceImage: input image
		destinationImage: output image
	Return:
		1: success
		0: fail (cannot read input image,...)
	*/
	int operateConvolution(const Mat& sourceImage, Mat& destinationImage);

	Convolutioner() {}
	~Convolutioner() {}
};

