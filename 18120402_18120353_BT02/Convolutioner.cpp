#include "Convolutioner.h"

void Convolutioner::setKernel(vector<float> kernel, int kWidth, int kHeight)
{
	_kernel = kernel;
	_kernelWidth = kWidth;
	_kernelHeight = kHeight;
};

int Convolutioner::operateConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	// Check if source image is loaded or not
	if (sourceImage.empty())
		return 0;

	// Initialize parameter
	int width = sourceImage.cols; // number of collumn
	int height = sourceImage.rows; // number of row
	int nChannels = sourceImage.channels(); // number of channel
	int widthStep = sourceImage.step[0]; // width of image

	// Create destination image
	destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	uchar* pData = (uchar*)destinationImage.data; // Get the data pointer of destination image
	cv::Mat temp_data = sourceImage.clone(); // clone data from source image
	uchar* psData = (uchar*)temp_data.data;  // Get the data pointer of cloned image

	// Create offsets table
	int kHalfWidth = _kernelWidth >> 1; // = 3
	int kHalfHeight = _kernelHeight >> 1; // = 3 
	vector<int> offsets;

	int OffsetTableSize;
	for (int y = -kHalfHeight; y <= kHalfHeight; y++) {
		for (int x = -kHalfWidth; x <= kHalfWidth; x++) {
			offsets.push_back(y * widthStep * nChannels + x * nChannels);
		}
	}
	OffsetTableSize = offsets.size();

	double sum;
	// source image is a grayscale image
	if (nChannels == 1)
	{
		for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep) // browse on row, step by widthStep
		{
			uchar* pRow = pData; 
			uchar* psRow = psData;
			for (int j = 0; j < width; j++, psRow += nChannels, pRow += nChannels) // browse on collumn, step by nchannels
			{
				sum = 0;
				// Do convolution
				for (int k = 0; k < OffsetTableSize; k++) // browse on offset table
				{
					int index = offsets[k];

					if (i * width + j + index > 0 && i * widthStep + j + index < width * height * nChannels) // Check for the limit of image
					{
						uchar* p_temp = psRow; // Get temporary data pointer from psRow
						p_temp += index; // move the pointer with index unit

						if (i * width + j + index > 0)
						{
							uchar val = *(p_temp);
							sum += val * _kernel[OffsetTableSize - 1 - k];
						}
					}
				}

				// accurate conversion from one primitive type to another
				pRow[0] = saturate_cast<uchar>(sum);
			}
		}
	}

	// source image is a color image
	else
	{
		double sum0, sum1, sum2;
		for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep) // browse on row, step by widthStep
		{
			uchar* pRow  = pData;
			uchar* psRow = psData;
			for (int j = 0; j < width; j++, psRow += nChannels, pRow += nChannels) // browse on collumn, step by nchannels
			{
				sum0 = 0;
				sum1 = 0;
				sum2 = 0;
				// Do convolution
				for (int k = 0; k < OffsetTableSize; k++) // browse on offset table
				{
					int index = offsets[k];

					if (i * width + j + index >= 0 && i * widthStep + j + index < width * height * nChannels) // Check for the limit of image
					{
						// Blue channel
						uchar* p_temp = psRow; // Get temporary data pointer from psRow
						p_temp += index; // move the pointer with index unit
						uchar val0 = *(p_temp); // Get value from poiner
						sum0 += val0 * _kernel[OffsetTableSize - 1 - k];

					}
					if (i * width + j + index + 1 > 0 && i * widthStep + j + index + 1 < width * height * nChannels) // Check for the limit of image
					{
						// Green channel
						uchar* p_temp1 = psRow + 1; // Get temporary data pointer from psRow
						p_temp1 += index; // move the pointer with index unit
						uchar val1 = *(p_temp1); // Get value from poiner
						sum1 += val1 * _kernel[OffsetTableSize - 1 - k];

					}
					if (i * width + j + index + 2 > 0 && i * widthStep + j + index + 2 < width * height * nChannels) // Check for the limit of image
					{
						// Red channel
						uchar* p_temp2 = psRow + 2; // Get temporary data pointer from psRow
						p_temp2 += index; // move the pointer with index unit
						uchar val2 = *(p_temp2); // Get value from poiner
						sum2 += val2 * _kernel[OffsetTableSize - 1 - k];
					}
				}

				// accurate conversion from one primitive type to another
				pRow[0] = saturate_cast<uchar>(sum0);
				pRow[1] = saturate_cast<uchar>(sum1);
				pRow[2] = saturate_cast<uchar>(sum2);
			}
		}
	}

	return 1;
};