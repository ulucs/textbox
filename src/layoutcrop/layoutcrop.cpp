#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

void shrinkShow(const cv::string& winname, cv::Mat img)
{
	cv::Mat viewimg;
	int viewHeight = 700;
	double shrinkBy = (double) viewHeight / img.rows;

	if (shrinkBy < 1)
		cv::resize(img, viewimg, cv::Size(), shrinkBy, shrinkBy);
	else
		viewimg = img;

	cv::imshow(winname, viewimg);
	cv::waitKey(0);
	return;
}

int main(int argc,char** argv)
{
	// Usage: tesscv image.png
	if (argc < 2)
	{
		std::cout << "Please specify the input image!" << std::endl;
		return -1;
	}

	cv::Mat img;
	// Load layout
	img = cv::imread(argv[1]);

	if (img.empty())
	{
		std::cout << "Cannot open source image!" << std::endl;
		return -1;
	}

	cv::Size fitBox = cv::Size(atoi(argv[2]),atoi(argv[3]));

	cv::Point cor1, cor2;
	cor1 = cv::Point(atoi(argv[4]),atoi(argv[5]));
	cor2 = cv::Point(atoi(argv[6]),atoi(argv[7]));

	// preprocessing with thresh and denoise
	cv::Mat greyimg;
	cv::cvtColor(img, greyimg, CV_BGR2GRAY );
	cv::threshold(greyimg, greyimg, 0, 255, CV_THRESH_OTSU);
	// cv::Sobel(greyimg, greyimg, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	// cv::fastNlMeansDenoising(greyimg,greyimg,100);
	// shrinkShow(greyimg);

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = greyimg.begin<uchar>();
	cv::Mat_<uchar>::iterator end = greyimg.end<uchar>();
	for (; it != end; ++it)
		if (*it)
			points.push_back(it.pos());

	cv::Rect box = cv::boundingRect(cv::Mat(points));

	cv::Mat cropped = img(box);
	cv::resize(cropped,cropped,fitBox);

	cv::Mat ans = cropped(cv::Rect(cor1,cor2));
	cv::fastNlMeansDenoising(ans,ans,17);
	cv::cvtColor(ans,ans, CV_BGR2GRAY );

	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "tst", tesseract::OEM_DEFAULT);
	tess.SetImage((uchar*)ans.data, ans.cols, ans.rows, 1, ans.cols);

	// print the characters of the image
	char* out = tess.GetUTF8Text();
	std::cout << out << std::endl;

	return 0;
}