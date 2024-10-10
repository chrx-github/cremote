#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgGray,imgBlur,imgCanny,imgDil,imgErode,imgHSV,mask,img,imgCrop;

void getContours(Mat imgDil,Mat img)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++)
    {
        int area=contourArea(contours[i]);
        if(area>4500)
        {
            boundRect[i]=boundingRect(contours[i]);
            rectangle(img,boundRect[i].tl(),boundRect[i].br(),Scalar(0,255,0),1.5);
            // drawContours(img,contours,i,Scalar(255,0,255),2);
            Rect roi(boundRect[i].tl(),boundRect[i].br());
            imgCrop=img(roi);
        }
    }
}

int main()
{
    //attempt to use CascadeClassifier without XML file, but failed

    // CascadeClassifier detec;
    // detec.load("resources/jinkuang.png");
    // if(detec.empty()){ cout<<"jinkuang.png not loaded"<< endl;
    // VideoCapture cap("resources/gold_video_1.avi");
    // Mat img;
    // vector<Rect> gmine;
    // while(true)
    // {
    //     cap.read(img);
    //     // detec.detectMultiScale(img,gmine, 1.1, 10);//clear?
    //     // for(int i=0;i<gmine.size();i++)
    //     // {
    //     //     rectangle(img,gmine[i].tl(),gmine[i].br(),Scalar(255,255,255),3);
    //     // }
    //     imshow("Goldmine Recognition",img);
    //     waitKey(50);
    // }

    //Color Detection
    VideoCapture cap("resources/gold_video_1.avi");

    while(true)
    {
        cap.read(img);
        if(img.empty())
            break;
        cvtColor(img,imgHSV,COLOR_BGR2HSV);
        Scalar lower(20,128,128);
        Scalar upper(60,255,255);
        inRange(imgHSV,lower,upper,mask);
        GaussianBlur(mask,imgBlur,Size(1,1),0,0);
        Canny(imgBlur,imgCanny,100,150);
        Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3));
        dilate(imgCanny,imgDil,kernel);

        getContours(imgDil,img);
        imshow("Goldmine Recognition",img);
        waitKey(6);
        imshow("Mask",mask);
        waitKey(6);
        imshow("Crop Goldmine Picture",imgCrop);
        waitKey(6);
    }
    cap.release();
    destroyAllWindows();
}