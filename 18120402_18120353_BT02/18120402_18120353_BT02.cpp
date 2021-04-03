#include <iostream>
#include "opencv2/opencv.hpp "
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include "Edge_Detector.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//Mat grayscale_image = imread("C:\\Users\\pc\\Desktop\\Temparory\\Lenna.png", IMREAD_GRAYSCALE); // (2)
	//Mat dstimg;
	//ColorTransformer trans;
	//trans.ConvertGrayscaletoRGB(grayscale_image, dstimg);
	//trans.ConvertRGBtoGrayscale(grayscale_image, dstimg);

	// Check number of argument is correct
	if (argc < 2)
	{
		cout << "Photoshop program" << endl;
		return -1;
	}

	// Intialization
	Mat color_image, dst_img, grayscale_image;
	int code = atoi(argv[2]);
	int optional = atoi(argv[3]);
	Edge_Detector ed;

	// Load color image
	color_image = imread(argv[1], IMREAD_COLOR);
	if (!color_image.data)
	{
		cout << "Cannot open image" << std::endl;
		return -1;
	}

	// Edge detection using Sobel kernel
	if (code == 0)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", color_image);
		waitKey(0);

		ed.detectBySobel(color_image, dst_img);
	}

	// Edge detection using Prewitt kernel
	else if (code == 1)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", grayscale_image);
		waitKey(0);

		ed.detectByPrewitt(grayscale_image, dst_img);
	}

	// Edge detection using Laplace kernel
	else if (code == 2)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", color_image);
		waitKey(0);

		short brightness = atoi(argv[3]);
		ed.detectByLaplace(color_image, dst_img);
	}

	// Edge detection using Cany method
	else if (code == 3)
	{
		// Show destination image
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", color_image);
		waitKey(0);

		float contrast = atof(argv[3]);
		ed.detectByCany(color_image, dst_img);
	}

	// Show destination image
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", dst_img);
	waitKey(0);
	return 0;
}
