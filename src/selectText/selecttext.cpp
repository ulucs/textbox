#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

bool leftDown=false,leftup=false;
Mat img;
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
		Mat temp_img=img.clone();
		rectangle(temp_img,cor1,pt,Scalar(0,0,255)); //drawing a rectangle continuously
		imshow("Original",temp_img);
	}

	if(leftDown==true&&leftup==true) //when the selection is done
	{
		box.width=abs(cor1.x-cor2.x);
		box.height=abs(cor1.y-cor2.y);
		box.x=min(cor1.x,cor2.x);
		box.y=min(cor1.y,cor2.y);
		Mat crop(img,box);   //Selecting a ROI(region of interest) from the original pic
		namedWindow("Cropped Image");
		imshow("Cropped Image",crop); //showing the cropped image
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
    if (img1.empty())
    {
        cerr << "Cannot open source image!" << endl;
        return -1;
    }

   namedWindow("Original");
   imshow("Original",img);

   setMouseCallback("Original",mouse_call); //setting the mouse callback for selecting the region with mouse

   while(char(waitKey(1)!='n')) //waiting for the 'q' key to finish the execution
   {

   }
   return 0;
}