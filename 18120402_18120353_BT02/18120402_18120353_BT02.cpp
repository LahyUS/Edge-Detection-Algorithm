#include "opencv2/opencv.hpp "
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include "Edge_Detector.h"
#include "Convolutioner.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat src_img = imread("C:\\Users\\pc\\Desktop\\Temparory\\Lenna.png", IMREAD_COLOR); // (2)
	Mat dst_img;
	dst_img.create(src_img.rows, src_img.cols, src_img.type());

	Edge_Detector ed;
	int result = ed.detectByLaplace(src_img, dst_img);

	// Show destination image
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
