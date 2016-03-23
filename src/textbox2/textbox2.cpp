#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <fstream>
#include <cstring>

void shrinkShow(cv::Mat img)
{
    cv::Mat viewimg;
    int viewHeight = 760;
    double shrinkBy = (double) viewHeight / img.rows;

    if (shrinkBy < 1)
        cv::resize(img, viewimg, cv::Size(), shrinkBy, shrinkBy);
    else
        viewimg = img;

    cv::imshow("Original",viewimg);
    cv::waitKey(0);
    return;
}

std::vector<cv::Rect> detectLetters(cv::Mat img)
{
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    // put in already grayed image
    // cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::Sobel(img, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    // we should calibrate the size numbers according to our scan size
    // this supposedly works with 300 ppi
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(180, 5) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1); 
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        if (contours[i].size()>100)
        { 
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height) 
                boundRect.push_back(appRect);
        }
    return boundRect;
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
    cv::Mat img1 = cv::imread(argv[1]);
    if (img1.empty())
    {
        std::cout << "Cannot open source image!" << std::endl;
        return -1;
    }

    // all grayscaling here
    // asserts should have been compile time errors
    cv::cvtColor( img1, img1, CV_BGR2GRAY );

    //Detect
    std::vector<cv::Rect> letterBBoxes1=detectLetters(img1);

    // for size comparison
    int p;
    double cmax = 0;
    for(int i=0; i< letterBBoxes1.size(); i++)
    {
        // record the ratio of width and length of the bounding box
        // with at least 20 height (so we include characters)
        if (letterBBoxes1[i].y > cmax && letterBBoxes1[i].height*letterBBoxes1[i].width > 200)
        {
            cmax = letterBBoxes1[i].y;
            p = i;
        }
    }
    // mark the widest box with red
    // comment this out for production
    // cv::rectangle(img1,letterBBoxes1[p],cv::Scalar(0,0,255),3,8,0);

    // check if widest box is larger than half of the page
    // exit if it is not
    if (letterBBoxes1[p].width < img1.cols/4)
    {
        std::cout << "The page does not contain answers" << std::endl;
        return 0;
    }

    // or crop the widest box
    cv::Mat imgbox = img1(cv::Rect(letterBBoxes1[p].x-40,letterBBoxes1[p].y,letterBBoxes1[p].width+80,letterBBoxes1[p].height));

    // // crop to delete sides
    // cv::Rect roi1 = cv::Rect(10,0,letterBBoxes1[p].width-20,letterBBoxes1[p].height);
    // cv::Mat imgbox2 = imgbox(roi1);


    // this is the easy way out code

    // crop to optimistically remove the border
    // it actually worked...
    // we should deskew according to the border, and then identify and remove it
    // current data shows it will be much more accurate
    // cv::Rect roi = cv::Rect(30,43,letterBBoxes1[p].width-60,letterBBoxes1[p].height-80);
    // cv::Mat imgbox2 = imgbox(roi);

    // show what we've got
    //cv::namedWindow("TextBox", cv::WINDOW_AUTOSIZE);
    //cv::imshow("TextBox",imgbox2);
    //cv::waitKey(0);

    // perspective correction??

    // identify the bounding rectangle
    // start typing here
    cv::Mat gray;
    cv::threshold(imgbox, imgbox, 0, 255, CV_THRESH_OTSU);

    // goodbye noise (works for 300 ppi)
    cv::fastNlMeansDenoising(imgbox,gray,17);
    // shrinkShow(gray);

    // pass the image to tesseract
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "tst", tesseract::OEM_DEFAULT);
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // print the characters of the image
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;

    // std::ofstream outfile;

    // strcat(argv[1],".txt");
    // outfile.open(argv[1], std::ios_base::app);
    // outfile << out;

    return 0;
}