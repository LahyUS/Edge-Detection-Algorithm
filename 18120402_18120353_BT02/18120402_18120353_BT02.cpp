#include <iostream>
#include "opencv2/opencv.hpp "
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include "Edge_Detector.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Check number of argument is correct
	if (argc < 2)
	{
		cout << "Photoshop program" << endl;
		return -1;
	}

	// Intialization
	Mat src_img, dst_img;
	int code = atoi(argv[2]);
	int optional = atoi(argv[3]);
	Edge_Detector ed;

	// Load color image
	src_img = imread(argv[1], IMREAD_COLOR);
	if (!src_img.data)
	{
		cout << "Cannot open image" << std::endl;
		return -1;
	}

	// Edge detection using Sobel kernel
	if (code == 0)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", src_img);
		waitKey(0);

		ed.detectBySobel(src_img, dst_img);
	}

	// Edge detection using Prewitt kernel
	else if (code == 1)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", src_img);
		waitKey(0);

		ed.detectByPrewitt(src_img, dst_img);
	}

	// Edge detection using Laplace kernel
	else if (code == 2)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", src_img);
		waitKey(0);

		short brightness = atoi(argv[3]);
		ed.detectByLaplace(src_img, dst_img);
	}

	// Edge detection using Cany method
	else if (code == 3)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", src_img);
		waitKey(0);

		float contrast = atof(argv[3]);
		ed.detectByCany(src_img, dst_img);
	}

	// Show destination image
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", dst_img);
	waitKey(0);
	return 0;
}
