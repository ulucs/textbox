#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <tesseract/baseapi.h>

using namespace cv;
using namespace std;

bool leftDown=false,leftup=false;
Mat img;
Mat viewimg;
Mat crop;
double shrinkBy;
Point cor1,cor2;
Rect box;

void mouse_call(int event,int x,int y,int,void*)
{
	if(event==EVENT_LBUTTONDOWN)
	{
		leftDown=true;
		cor1.x=x;
		cor1.y=y;
		clog << "Corner 1: " << cor1 << endl;
	}

	if(event==EVENT_LBUTTONUP)
	{
		if(abs(x-cor1.x)+(y-cor1.y)>40) //checking whether the region is too small
		{
			leftup=true;
			cor2.x=x;
			cor2.y=y;
			clog << "Corner 2: " << cor2 << endl;
		}
		else
		{
			clog << "Mouse click ignored" << endl;
		}
	}

	if(leftDown==true&&leftup==false) //when the left button is down
	{
		Point pt;
		pt.x=x;
		pt.y=y;
		Mat temp_img=viewimg.clone();
		rectangle(temp_img,cor1,pt,Scalar(0,0,255)); //drawing a rectangle continuously
		imshow("Original",temp_img);
	}

	if(leftDown==true&&leftup==true) //when the selection is done
	{
		box.width=abs(cor1.x-cor2.x) / shrinkBy;
		box.height=abs(cor1.y-cor2.y) / shrinkBy;
		box.x=min(cor1.x,cor2.x) / shrinkBy;
		box.y=min(cor1.y,cor2.y) / shrinkBy;
		crop = img(box);   //Selecting a ROI(region of interest) from the original pic

		Mat cropShow;
		int viewHeight1 = 700;
		if (crop.rows > viewHeight1)
		{
			double csh = (double) viewHeight1 / crop.rows;
			resize(crop, cropShow, Size(), csh, csh);
		}
		else
			cropShow = crop.clone();

		namedWindow("Cropped Image");
		imshow("Cropped Image",cropShow); //showing the cropped image
		leftDown=false;
		leftup=false;
	}
}

int main(int argc,char** argv)
{
   if (argc != 2)
	{
		cerr << "Please specify the input image!" << endl;
		return -1;
	}

	// Load image
	img = imread(argv[1]);
	if (img.empty())
	{
		cerr << "Cannot open source image!" << endl;
		return -1;
	}

	int viewHeight = 760;
	shrinkBy = (double) viewHeight / img.rows;

	resize(img, viewimg, Size(), shrinkBy, shrinkBy);

	namedWindow("Original", 0);
	imshow("Original",viewimg);

	setMouseCallback("Original",mouse_call); //setting the mouse callback for selecting the region with mouse

	waitKey(0);

	destroyAllWindows();

	if (crop.empty())
	{
		cout << "Cropped part is empty" << endl;
		return 0;
	}

	fastNlMeansDenoising(crop,crop,17);
	cvtColor( crop, crop, CV_BGR2GRAY );
	// adaptiveThreshold(crop,crop,255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 25, 0);

	// imshow("Modded", crop);
	// while(char(waitKey(1)!='n')) //waiting for the 'q' key to finish the execution
	// {

	// }

	// pass the image to tesseract
	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "tst", tesseract::OEM_DEFAULT);
	tess.SetImage((uchar*)crop.data, crop.cols, crop.rows, 1, crop.cols);

	// print the characters of the image
	char* out = tess.GetUTF8Text();
	cout << out << endl;

   return 0;
}
