#include "Edge_Detector.h"
#include "Convolutioner.h"
#define PI 3.14;

<<<<<<< HEAD
int Edge_Detector::detectBySobel(Mat sourceImage, Mat destinationImage)
{
	Convolutioner conv;

	vector<float> kernel_x;
	kernel_x = { 1, 0, -1,2,0,-2,1,0,-1 };
	for (int i = 0; i < 9; i++) {
		kernel_x[i] = kernel_x[i] * 1.0 / 1;
	}
	conv.SetKernel(kernel_x, 3, 3);

	int width = sourceImage.cols, height = sourceImage.rows;
	Mat dstFx;
	dstFx.create(height, width, sourceImage.type());
	conv.DoConvolution(sourceImage, dstFx);

	vector<float> kernel_y;
	kernel_y = { -1,-2,-1,0,0,0,1,2,1 };
	for (int i = 0; i < 9; i++) {
		kernel_y[i] = kernel_y[i] * 1.0 / 1;
	}
	conv.SetKernel(kernel_y, 3, 3);

	Mat dstFy;
	dstFy.create(height, width, sourceImage.type());
	conv.DoConvolution(sourceImage, dstFy);

	int nChannels = sourceImage.channels();
	int size_row_raw = width * nChannels;
	int rem = size_row_raw % 4;
	int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

	uchar* pxData = dstFx.data;
	uchar* pyData = dstFy.data;
	uchar* pData = destinationImage.data;

	uchar* pxRow = dstFx.data;
	uchar* pyRow = dstFy.data;
	uchar* pRow = destinationImage.data;
=======
Mat rgb2gray(Mat& sourceImage, Mat& destinationImage);
Mat computeGradient(Mat& sourceImage, Mat& destinationImage);
Mat nonMaxSuppression(Mat& directSegment, Mat& destinationImage);
void doubleThreshold(Mat& M, Mat& destinationImage, float strongThreshold, float weakThreshold);
void trackingHysteresis(Mat& destinationImage);

Mat rgb2gray(Mat& sourceImage, Mat& destinationImage)
{
	int channels = sourceImage.channels();
	if (channels == 1) {
		return sourceImage;
	}

	uchar lookup[256];
	int width = sourceImage.cols, height = sourceImage.rows;
	//we don't want calculate a many similar rgb pixels many time
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i);

	destinationImage.create(height, width, CV_8UC1);

	for (int i = 0; i < height; i++) {
		uchar* pdstRow = destinationImage.ptr<uchar>(i);
		uchar* psrcRow = sourceImage.ptr<uchar>(i);
		for (int j = 0; j < width; j++, psrcRow += channels, pdstRow++) {
			//Y = 0.299⋅R+0.587⋅G+0.114⋅B
			float Y = ((float)psrcRow[2] * 2 + (float)psrcRow[1] * 5 + (float)psrcRow[0] * 1) / 8;
			pdstRow[0] = lookup[(int)Y];
		}
	}

	Mat grayImage = destinationImage;
	return grayImage;
}

/*detect edge with grandient and calculate the direction of each edges
  out: a matrix store the delegates that represent gradient intensity for all actual edge detection
	1. -
	2. /
	3. |
	4. \
*/
Mat computeGradient(Mat& sourceImage, Mat& destinationImage)
{
	//init variables
	Convolutioner conv;
	vector<float> verticalDirection, horizontalDirection;
	Mat verticalDirectImage, horizontalDirectImage, directSegment;
	uchar* pVer = NULL, * pHor = NULL, * pDes = NULL, * pSeg = NULL;

	const int sizeOfMatrix = 3;
	int channels = sourceImage.channels();
	directSegment.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	//sobel fiter 3x3 matrix
	//horizontalDirection = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	//verticalDirection = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	//prewitt filter 3x3 matrix
	horizontalDirection = { -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f };
	verticalDirection = { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	//create 2 direction images with current axis 2 direct matrices
	conv.setKernel(verticalDirection, sizeOfMatrix, sizeOfMatrix);
	conv.operateConvolution(sourceImage, verticalDirectImage);
	imshow("ver", verticalDirectImage);

	conv.setKernel(horizontalDirection, sizeOfMatrix, sizeOfMatrix);
	conv.operateConvolution(sourceImage, horizontalDirectImage);
	imshow("hor", horizontalDirectImage);

	/*	combination vertical and horizontal prewitt image into a single destination image.
		all the channels in all the pixels in destination image have calculated by this fomular: G = sqrt(Gx^2 + Gy^2)
		then compute the edge direction by this fomular: arctan-1(Gy, Gx)
	*/
	int horizontalConvolve, verticalConvolve;
	for (int i = 0; i < sourceImage.rows; i++) {

		//pointers to each rows of the matrices.
		pVer = verticalDirectImage.ptr<uchar>(i);
		pHor = horizontalDirectImage.ptr<uchar>(i);
		pSeg = directSegment.ptr<uchar>(i);

		//destination image store the combination of 2 axis prewitt.
		pDes = destinationImage.ptr<uchar>(i);
		for (int j = 0; j < sourceImage.cols; j++, pVer++, pHor++, pDes++, pSeg++) {

			verticalConvolve = pVer[0] * pVer[0];
			horizontalConvolve = pHor[0] * pHor[0];

			//G = sqrt(Gx^2 + Gy^2)
			pDes[0] = saturate_cast<uchar>(sqrt(horizontalConvolve + verticalConvolve));

			double theta = std::atan2(verticalConvolve, horizontalConvolve);
			theta = theta * 180 * PI;  // translate gradian to degrees
			int segment = 0;

			//convert actual edge direction to approximate value 
			if (((theta < 22.5) && (theta > -22.5)) || (theta > 157.5) && (theta < -157.5))
				segment = 1; // -
			else if (((theta > 22.5) && (theta < 67.5)) || ((theta < -112.5) && (theta > -157.5)))
				segment = 2; // /
			else if (((theta > 67.5) && (theta < 112.5)) || ((theta < -67.5) && (theta > -112.5)))
				segment = 3;// |
			else if (((theta > 112.5) && (theta < 157.5)) || ((theta < -22.5) && (theta > -67.5)))
				segment = 4;// \ 
			pSeg[0] = segment;
		}
	}
	return directSegment;
}

/*	 the algorithm goes through all the points on the gradient intensity matrix (direct segment)
	 and finds the pixels with the maximum value in the edge directions.
	 out: a matrix maxSup intensities the pixel with the maximum value of edge directions near by.
*/
Mat nonMaxSuppression(Mat& directSegment, Mat& destinationImage) {

	uchar* pSeg = NULL, * pM = NULL, * pDst = NULL, * pDstPlus = NULL, * pDstMinus = NULL;
	Mat maxSup;
	maxSup.create(directSegment.rows, directSegment.cols, directSegment.type());

	for (int i = 1; i < destinationImage.rows - 1; i++) {
		//point to the gradient intensity matrix
		pSeg = directSegment.ptr<uchar>(i);
		//point to the dest image
		pDst = destinationImage.ptr<uchar>(i);
		//upper the of current pixel
		pDstPlus = destinationImage.ptr<uchar>(i + 1);
		//lower of the current pixel
		pDstMinus = destinationImage.ptr<uchar>(i - 1);
		//point to the output matrix
		pM = maxSup.ptr<uchar>(i);

		int p = 255, r = 255;
		for (int j = 1; j < destinationImage.cols - 1; j++, pM++, pSeg++, pDst++, pDstPlus++, pDstMinus++) {
			switch (pSeg[0]) {
			case 1: // -
				p = (pDst + 1)[0];
				r = (pDst - 1)[0];
				break;

			case 2: // /
				r = (pDstMinus + 1)[0];
				p = (pDstPlus - 1)[0];
				break;
			case 3: // |
				r = (pDstMinus)[0];
				p = (pDstPlus)[0];
				break;
			case 4: // \ 
				r = (pDstMinus - 1)[0];
				p = (pDstPlus + 1)[0];
				break;
			}
			//intensity the maxium pixels in directions.
			if (pDst[0] >= p && pDst[0] >= r) {
				pM[0] = pDst[0];
			}
			else {
				pM[0] = 0;
			}
		}

	}
	return maxSup;
}

//intensity strong and weak edge by 2 threshold values.
//parameter: maxSup intensities the pixel with the maximum value of edge directions near by.
void doubleThreshold(Mat& maxSup, Mat& destinationImage, float strongThreshold, float weakThreshold) {
	uchar* pDst, * pM;

	for (int i = 0; i < destinationImage.rows; i++) {
		pDst = destinationImage.ptr<uchar>(i);
		pM = maxSup.ptr<uchar>(i);
		for (int j = 0; j < destinationImage.cols; j++, pDst++, pM++) {
			if (pM[0] > strongThreshold) {
				pDst[0] = 255;
			}
			else if (pM[0] > weakThreshold) {
				pDst[0] = 100;
			}
			else {
				pDst[0] = 0;
			}
		}
	}
}

//tracking the edges of weak edge intensities. Update weak intensity pixel to strong intensity pixel if one of the direct neighbors is strong intensity pixel.
void trackingHysteresis(Mat& destinationImage) {
	for (int i = 1; i < destinationImage.rows - 1; i++) {
		uchar* pDst = destinationImage.ptr<uchar>(i);
		//upper row of current row
		uchar* pDstPlus = destinationImage.ptr<uchar>(i + 1);
		//lower row of current row
		uchar* pDstMinus = destinationImage.ptr<uchar>(i - 1);

		for (int j = 1; j < destinationImage.cols - 1; j++, pDst++, pDstPlus++, pDstMinus++) {
			//if one of the neighbors of a weak intensity pixel (100) is a strong intensity (255), when want update that pixel to strong intensity (255). 
			if (pDst[0] == 100) {
				if ((pDst - 1)[0] == 255 || (pDst + 1)[0] == 255 ||
					(pDstPlus - 1)[0] == 255 || (pDstPlus)[0] == 255 || (pDstPlus + 1)[0] == 255 ||
					(pDstMinus - 1)[0] == 255 || (pDstMinus)[0] == 255 || (pDstMinus + 1)[0] == 255)
				{
					pDst[0] = 255;
				}
				else {
					pDst[0] = 0;
				}
			}
		}
	}
}

int Edge_Detector::detectBySobel(Mat sourceImage, Mat destinationImage)
{
	// Initialize parameters
	Convolutioner conv;
	vector<float> kernel_x = { 1, 0, -1,2,0,-2,1,0,-1 };
	vector<float> kernel_y = { -1,-2,-1,0,0,0,1,2,1 };
	int width = sourceImage.cols, height = sourceImage.rows;
	Mat verticalDirectImage, horizontalDirectImage;
	uchar* pVer = NULL, * pHor = NULL, * pDes = NULL;
	int channels = sourceImage.channels();
>>>>>>> Completed

	// Operate convolution on X axis
	conv.setKernel(kernel_x, 3, 3);
	verticalDirectImage.create(height, width, sourceImage.type());
	conv.operateConvolution(sourceImage, verticalDirectImage);
	imshow("Vertical Sobel", verticalDirectImage);

	// Operate convolution on Y axis
	conv.setKernel(kernel_y, 3, 3);
	horizontalDirectImage.create(height, width, sourceImage.type());
	conv.operateConvolution(sourceImage, horizontalDirectImage);
	imshow("Horizontal", horizontalDirectImage);

	/*	combination vertical and horizontal prewitt image into a single destination image.
		all the channels in all the pixels in destination image have calculated by this fomular: G = sqrt(Gx^2 + Gy^2)
	*/
	for (int i = 0; i < sourceImage.rows; i++) 
	{
		//pointers point to the direct prewitt images.
		pVer = verticalDirectImage.ptr<uchar>(i);
		pHor = horizontalDirectImage.ptr<uchar>(i);

		//destination image store the combination of 2 axis prewitt.
		pDes = destinationImage.ptr<uchar>(i);
		for (int j = 0; j < sourceImage.cols; j++, pVer += channels, pHor += channels, pDes += channels) {

			//each channels in a pixel.
			for (int k = 0; k < channels; k++) {
				//G = sqrt(Gx^2 + Gy^2)
				pDes[k] = saturate_cast<uchar>(sqrt(pVer[k] * pVer[k] + pHor[k] * pHor[k]));
			}
		}
	}

	return 1;
}

int Edge_Detector::detectByPrewitt(Mat sourceImage, Mat destinationImage)
{
	//init variables
	Convolutioner conv;
	vector<float> verticalDirection, horizontalDirection;
	Mat verticalDirectImage, horizontalDirectImage;
	uchar* pVer = NULL, * pHor = NULL, * pDes = NULL;

	const int sizeOfMatrix = 3;
	int channels = sourceImage.channels();
	destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	//prewitt filter 3x3 matrix
	verticalDirection = { -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f };
	horizontalDirection = { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	//create 2 direction images with current axis 2 direct matrices
	conv.setKernel(verticalDirection, sizeOfMatrix, sizeOfMatrix);
	conv.operateConvolution(sourceImage, verticalDirectImage);
	imshow("Vertical Prewitt", verticalDirectImage);

	conv.setKernel(horizontalDirection, sizeOfMatrix, sizeOfMatrix);
	conv.operateConvolution(sourceImage, horizontalDirectImage);
	imshow("Horizontal Prewitt", horizontalDirectImage);

	/*	combination vertical and horizontal prewitt image into a single destination image.
		all the channels in all the pixels in destination image have calculated by this fomular: G = sqrt(Gx^2 + Gy^2)
	*/
	for (int i = 0; i < sourceImage.rows; i++) {
		//pointers point to the direct prewitt images.
		pVer = verticalDirectImage.ptr<uchar>(i);
		pHor = horizontalDirectImage.ptr<uchar>(i);

		//destination image store the combination of 2 axis prewitt.
		pDes = destinationImage.ptr<uchar>(i);
		for (int j = 0; j < sourceImage.cols; j++, pVer += channels, pHor += channels, pDes += channels) {

			//each channels in a pixel.
			for (int k = 0; k < channels; k++) {
				//G = sqrt(Gx^2 + Gy^2)
				pDes[k] = saturate_cast<uchar>(sqrt(pVer[k] * pVer[k] + pHor[k] * pHor[k]));
			}
		}
	}

	return 1;
}

int Edge_Detector::detectByLaplace(Mat sourceImage, Mat destinationImage)
{
<<<<<<< HEAD
	Convolutioner conv;
	vector<float> kernel_x;
	kernel_x = { 0,-1,0,-1,4,-1,0,-1,0 };
	conv.SetKernel(kernel_x, 3, 3);
	for (int i = 0; i < 9; i++) {
		kernel_x[i] = kernel_x[i] * 1.0 / 4;
	}

	int width = sourceImage.cols, height = sourceImage.rows;
	Mat dstFx;
	dstFx.create(height, width, sourceImage.type());
	conv.DoConvolution(sourceImage, dstFx);

	vector<float> kernel_y;
	kernel_y = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	conv.SetKernel(kernel_y, 3, 3);
	for (int i = 0; i < 9; i++) {
		kernel_y[i] = kernel_y[i] * 1.0 / 4;
	}
	Mat dstFy;
	dstFy.create(height, width, sourceImage.type());
	conv.DoConvolution(sourceImage, dstFy);

	int nChannels = sourceImage.channels();
	int size_row_raw = width * nChannels;
	int rem = size_row_raw % 4;
	int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

	uchar* pxData = dstFx.data;
	uchar* pyData = dstFy.data;
	uchar* pData = destinationImage.data;

	uchar* pxRow = dstFx.data;
	uchar* pyRow = dstFy.data;
	uchar* pRow = destinationImage.data;

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
=======
	// Initialize parameters
	Convolutioner conv;
	int width = sourceImage.cols, height = sourceImage.rows;
	vector<float> kernel_x, kernel_y;
	Mat verticalDirectImage, horizontalDirectImage;
	uchar* pVer = NULL, * pHor = NULL, * pDes = NULL;
	int channels = sourceImage.channels();

	// Operate convolution on X axis
	kernel_x = { 0,-1,0,-1,4,-1,0,-1,0 };
	conv.setKernel(kernel_x, 3, 3);
	verticalDirectImage.create(height, width, sourceImage.type());
	conv.operateConvolution(sourceImage, verticalDirectImage);
	imshow("Vertical Laplace", verticalDirectImage);

	// Operate convolution on Y axis
	kernel_y = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	conv.setKernel(kernel_y, 3, 3);
	horizontalDirectImage.create(height, width, sourceImage.type());
	conv.operateConvolution(sourceImage, horizontalDirectImage);
	imshow("Horizontal Laplace", horizontalDirectImage);

	/*	combination vertical and horizontal prewitt image into a single destination image.
	all the channels in all the pixels in destination image have calculated by this fomular: G = sqrt(Gx^2 + Gy^2)
*/
	for (int i = 0; i < sourceImage.rows; i++)
	{
		//pointers point to the direct prewitt images.
		pVer = verticalDirectImage.ptr<uchar>(i);
		pHor = horizontalDirectImage.ptr<uchar>(i);

		//destination image store the combination of 2 axis prewitt.
		pDes = destinationImage.ptr<uchar>(i);
		for (int j = 0; j < sourceImage.cols; j++, pVer += channels, pHor += channels, pDes += channels) 
		{

			//each channels in a pixel.
			for (int k = 0; k < channels; k++)
			{
				//G = sqrt(Gx^2 + Gy^2)
				pDes[k] = saturate_cast<uchar>(sqrt(pVer[k] * pVer[k] + pHor[k] * pHor[k]));
			}
		}
	}

>>>>>>> Completed
	return 1;
}

int Edge_Detector::detectByCanny(Mat sourceImage, Mat& destinationImage)
{
	//initial variables
	vector<float> gaussianMatrix, verticalDirection, horizontalDirection;
	const int sizeOfMatrix = 3;
	Convolutioner conv;
	Mat verticalDirectImage, horizontalDirectImage;

	//value for thresholding
	const float lowThreshold = 15.0f;
	const float highTreshold = 30.0f;

	//intial des image
	destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	//gaussian filter matrix
	gaussianMatrix = { 1.0f / 16 , 2.0f / 16, 1.0f / 16, 2.0f / 16, 4.0f / 16, 2.0f / 16, 1.0f / 16, 2.0f / 16, 1.0f / 16 };

	//Step 0: translate rgb image to grayscale image.
	Mat grayImage = destinationImage;
	rgb2gray(sourceImage, grayImage);

	//Step 1: smooth the image with gaussian filter.
	Mat smoothImage = destinationImage;

	conv.setKernel(gaussianMatrix, sizeOfMatrix, sizeOfMatrix);
	conv.operateConvolution(grayImage, smoothImage);
	destinationImage = smoothImage;

	//Step 2: compute edge gradient strength and direction 
	Mat directSegment = computeGradient(smoothImage, destinationImage);

	//Step 3: local maxima: non maxima suppression
	Mat maxSup = nonMaxSuppression(directSegment, destinationImage);

	//Step 4: double threshold
	doubleThreshold(maxSup, destinationImage, highTreshold, lowThreshold);

	//Step 5: edges with hysteresis
	trackingHysteresis(destinationImage);

	return 1;
}

