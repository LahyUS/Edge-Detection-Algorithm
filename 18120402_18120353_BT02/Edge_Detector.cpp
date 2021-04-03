#include "Edge_Detector.h"
#include "Convolutioner.h"

int Edge_Detector::detectBySobel(Mat src, Mat dst)
{
	Convolutioner convX, convY;	// Declare a convolutioner

	vector<float> kernel_x;
	kernel_x = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	for (int i = 0; i < 9; i++) {
		kernel_x[i] = kernel_x[i] * 1.0 / 9;
	}
	convX.SetKernel(kernel_x, 3, 3);

	int width = src.cols, height = src.rows;
	Mat dstFx;
	dstFx.create(height, width, src.type());
	convX.DoConvolution(src, dstFx);

	vector<float> kernel_y;
	kernel_y = { -1,-2,-1,0,0,0,1,2,1 };
	for (int i = 0; i < 9; i++) {
		kernel_y[i] = kernel_y[i] * 1.0 / 9;
	}
	convY.SetKernel(kernel_y, 3, 3);

	Mat dstFy;
	dstFy.create(height, width, src.type());
	convY.DoConvolution(src, dstFy);

	int nChannels = src.channels();
	int size_row_raw = width * nChannels;
	int rem = size_row_raw % 9;
	int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

	uchar* pxData = dstFx.data;
	uchar* pyData = dstFy.data;
	uchar* pData = dst.data;

	uchar* pxRow = dstFx.data;
	uchar* pyRow = dstFy.data;
	uchar* pRow = dst.data;

	float sum;
	for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep)
	{
		pRow = pData;
		pxRow = pxData;
		pyRow = pyData;
		for (int j = 0; j < width * nChannels; j++, pxRow += 1, pyRow += 1, pRow += 1) {
			sum = sqrt(pxRow[0] * pxRow[0] * 1.0f + pyRow[0] * pyRow[0] * 1.0f);
			pRow[0] = saturate_cast<uchar>(sum);
		}
	}
	return 1;
}

int Edge_Detector::detectByPrewitt(Mat src, Mat dst)
{
	return 1;
}

int Edge_Detector::detectByLaplace(Mat src, Mat des)
{
	return 1;
}

int Edge_Detector::detectByCany(Mat sourceImage, Mat destinationImage)
{
	return 1;
}