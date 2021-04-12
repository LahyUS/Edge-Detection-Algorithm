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

	// Load color image
	Mat src_img = imread(argv[1], IMREAD_COLOR);
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
		cout << code;
		result = ed.detectBySobel(src_img, dst_img);
	}

	// Edge detection using Prewitt kernel
	else if (code == 1)
	{
		cout << code;
		result = ed.detectByPrewitt(src_img, dst_img);
	}

	// Edge detection using Laplace kernel
	else if (code == 2)
	{
		cout << code;
		result = ed.detectByLaplace(src_img, dst_img);
	}

	// Edge detection using Cany method
	else if (code == 3)
	{
		cout << code;
		result = ed.detectByCanny(src_img, dst_img);
	}

	if (result == 1)
	{
		imshow("Source Image", src_img);
		imshow("Destination Image", dst_img);

		waitKey(0);
	}
	else
	{
		cout << "Cannot open image\n";
	}

	return 0;
}

