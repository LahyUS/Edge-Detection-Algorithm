<<<<<<< HEAD
#include <iostream>
#include <iostream>
=======
>>>>>>> Completed
#include "opencv2/opencv.hpp "
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include "Edge_Detector.h"
#include "Convolutioner.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Check number of argument is correct
	if (argc < 2 && argc > 3)
	{
		cout << "Photoshop program" << endl;
		return -1;
	}

<<<<<<< HEAD
	// Intialization
	Mat src_img, dst_img, grayscale_image;
	dst_img.create(src_img.rows, src_img.cols, src_img.type());
	int code = atoi(argv[2]);
	int optional = atoi(argv[3]);
	Edge_Detector ed;
	int result = 0;

	// Load color image
	src_img = imread(argv[1], IMREAD_COLOR);
=======
	// Load color image
	Mat src_img = imread(argv[1], IMREAD_COLOR);
>>>>>>> Completed
	if (!src_img.data)
	{
		cout << "Cannot open image" << std::endl;
		return -1;
	}

	// Intialization
	Mat dst_img;
	dst_img.create(src_img.rows, src_img.cols, src_img.type());
	int code = atoi(argv[2]);
	//int optional = atoi(argv[3]);
	Edge_Detector ed;
	int result;

	// Edge detection using Sobel kernel
	if (code == 0)
	{
<<<<<<< HEAD
=======
		cout << code;
>>>>>>> Completed
		result = ed.detectBySobel(src_img, dst_img);
	}

	// Edge detection using Prewitt kernel
	else if (code == 1)
	{
<<<<<<< HEAD
		result = ed.detectByPrewitt(grayscale_image, dst_img);
=======
		cout << code;
		result = ed.detectByPrewitt(src_img, dst_img);
>>>>>>> Completed
	}

	// Edge detection using Laplace kernel
	else if (code == 2)
	{
<<<<<<< HEAD
=======
		cout << code;
>>>>>>> Completed
		result = ed.detectByLaplace(src_img, dst_img);
	}

	// Edge detection using Cany method
	else if (code == 3)
	{
<<<<<<< HEAD
		result = ed.detectByCany(src_img, dst_img);
	}

	if (result == 1)
	{
		imshow("Source Image", src_img);
		imshow("Destination Image", dst_img);

=======
		cout << code;
		result = ed.detectByCanny(src_img, dst_img);
	}

	if (result == 1)
	{
		imshow("Source Image", src_img);
		imshow("Destination Image", dst_img);

>>>>>>> Completed
		waitKey(0);
	}
	else
	{
		cout << "Cannot open image\n";
	}

	return 0;
}

