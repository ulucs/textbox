#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <fstream>
#include <cstring>

cv::Mat layout;
cv::Rect box;
bool leftDown = false, leftup = false;
bool cropdone = false;
cv::Point cor1, cor2;
double shrinkBy;

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

void mouse_call(int event,int x,int y,int,void*)
{
	

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		leftDown = true;
		cor1.x = x;
		cor1.y = y;
		std::clog << "Corner 1: " << cor1 << std::endl;
	}

	if (event == cv::EVENT_LBUTTONUP)
	{
		if (abs(x - cor1.x)+(y - cor1.y) > 40) //checking whether the region is too small
		{
			leftup = true;
			cor2.x = x;
			cor2.y = y;
			std::clog << "Corner 2: " << cor2 << std::endl;
		}
		else
		{
			std::clog << "Mouse click ignored" << std::endl;
			leftup = false;
		}
	}

	if (leftDown == true && leftup == false) //when the left button is down
	{
		cv::Point pt;
		pt.x = x;
		pt.y = y;
		cv::Mat temp_img;

		int viewHeight = 700;
		shrinkBy = (double) viewHeight / layout.rows;
		cv::resize(layout, temp_img, cv::Size(), shrinkBy, shrinkBy);

		cv::rectangle(temp_img,cor1,pt,cv::Scalar(0,0,255)); //drawing a rectangle continuously
		cv::imshow("layout",temp_img);
	}

	if (leftDown == true && leftup == true) //when the selection is done
	{
		std::clog << "Target box specified!" << std::endl;

		cor1.x = cor1.x / shrinkBy; 
		cor2.x = cor2.x / shrinkBy; 
		cor1.y = cor1.y / shrinkBy; 
		cor2.y = cor2.y / shrinkBy; 
		
		cropdone = true;
		leftDown = false;
		leftup = false;
	}
}

int main(int argc,char** argv)
{
	// Usage: tesscv image.png
	if (argc != 2)
	{
		std::cout << "Please specify the input image!" << std::endl;
		return -1;
	}

	// Load image
	// cv::Mat img1 = cv::imread(argv[1]);
	// if (img1.empty())
	// {
	// 	std::cout << "Cannot open source image!" << std::endl;
	// 	return -1;
	// }

	// Load layout
	layout = cv::imread(argv[1]);

	if (layout.empty())
	{
		std::cout << "Cannot open source image!" << std::endl;
		return -1;
	}

	cv::Mat toBeCropped;
	// preprocessing with thresh and denoise
	cv::cvtColor(layout,toBeCropped, CV_BGR2GRAY );
	cv::threshold(toBeCropped, toBeCropped, 0, 255, CV_THRESH_OTSU);
	// cv::Sobel(toBeCropped, toBeCropped, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	// cv::fastNlMeansDenoising(toBeCropped,toBeCropped,100);
	// shrinkShow(toBeCropped);

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = toBeCropped.begin<uchar>();
	cv::Mat_<uchar>::iterator end = toBeCropped.end<uchar>();
	for (; it != end; ++it)
		if (*it)
			points.push_back(it.pos());

	cv::Rect box = cv::boundingRect(cv::Mat(points));

	cv::Mat cropped = layout(box);
	cv::namedWindow("layout");
	// shrinkShow("layout", cropped);
	// replacement

	cv::Mat viewimg;
	int viewHeight = 700;
	double shrinkBy = (double) viewHeight / layout.rows;

	if (shrinkBy < 1)
		cv::resize(layout, viewimg, cv::Size(), shrinkBy, shrinkBy);
	else
		viewimg = layout;

	cv::imshow("layout", viewimg);

	cv::setMouseCallback("layout",mouse_call); //setting the mouse callback for selecting the region with mouse
	cv::waitKey(0);

	if (cropdone)
		std::cout << cropped.cols << " " << cropped.rows << " " << cor1.x << " " << cor1.y << " " << cor2.x << " " << cor2.y << std::endl;
	else
		std::cout << "Nothing cropped!" << std::endl;

	return 0;
}