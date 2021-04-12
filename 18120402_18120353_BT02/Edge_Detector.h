#pragma once
#include <iostream>
#include "opencv2/opencv.hpp "
#include "opencv2/highgui/highgui.hpp"
#include <string>

using namespace cv;
using namespace std;
class Edge_Detector
{
public:
	/* Edge detection using Sobel kernel:
	   Parameters:
			src: source image, import as a matrix
			dst: destination image, export as a matrix
			"..." : other parameters in other case
	*/
	int detectBySobel(Mat src, Mat dst);

 	/* Edge detection using Prewitt kernel:
       Parameters:
			src: source image, import as a matrix
			dst: destination image, export as a matrix
			"..." : other parameters in other case
	*/
	int detectByPrewitt(Mat src, Mat dst);

	/* Edge detection using Laplace kernel:
       Parameters:
			src: source image, import as a matrix
			dst: destination image, export as a matrix
			"..." : other parameters in other case
	*/
	int detectByLaplace(Mat src, Mat des);

	/* Edge detection using Cany method:
	   Parameters:
			src: source image, import as a matrix
			dst: destination image, export as a matrix
	*/
	int detectByCanny(Mat sourceImage, Mat destinationImage);
};

