#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>
#include<filesystem>

using namespace std;
using namespace cv;

int main()
{
    // Mat RusFlag;
    // string path="../resources/FLAG1.png";//用相对路径
    // RusFlag=imread(path);
   
    // //找路径
    // // auto currentPath = filesystem::current_path();
    // // cout << "Current working directory: " << currentPath << endl;
    
    // // if (!filesystem::exists(path)) 
    // // {
    // //     cerr << "File does not exist: " << path << endl;
    // //     return -1;
    // // }   

    // Mat copflag,img;
    // //Mat copflag=Mat::zeros(900,600,CV_8UC3);//初始化
    // RusFlag.copyTo(copflag);
    // resize(copflag,img,Size(900,600));
    // imshow("Russian Flag",img);
    // waitKey(0);

    // Mat mask=Mat::zeros(900,600,CV_8UC1);
    // im.setTo(Scalar(255,255,255));
    
    // rectangle(mask,Point(0,201),Point(900,400),Scalar(255),-1);//-1=FILLED
    // Mat blue,red=Mat::zeros(900,200,CV_8UC3);
    // blue=red;
    // blue.setTo(Scalar(1, 33, 105));
    // red.setTo(Scalar(200, 16, 46));
    
    // blue.copyTo(im,mask(Rect(0,200,900,200)));
    // rectangle(mask,Point(0,201),Point(900,400),Scalar(0),-1);
    // rectangle(mask,Point(0,401),Point(900,600),Scalar(255),-1);
    // red.copyTo(im,mask(Rect(0,400,900,200)));




    Mat im=Mat::zeros(600,900,CV_8UC3);//Y->600 X->900
    rectangle(im,Point(0,0),Point(900,200),Scalar(255,255,255),FILLED);
    rectangle(im,Point(0,201),Point(900,400),Scalar(186,81,0),FILLED);//RGB->BGR
    rectangle(im,Point(0,401),Point(900,600),Scalar(5,30,216),FILLED);//
    imshow("im",im);
    waitKey(0);

//England
    Mat img=imread("../resources/FLAG2.png");
    Mat hsvimg;
    cvtColor(img,hsvimg,COLOR_BGR2HSV);
    Scalar lred(160,30,0);
    Scalar ured(180,255,200);
    Mat mask1;
    inRange(hsvimg,lred,ured,mask1);

    // imshow("mask1",mask1);

    Mat mask2;
    Scalar lblu(100,30,0);
    Scalar ublu(130,255,200);
    inRange(hsvimg,lblu,ublu,mask2);

    // imshow("mask2",mask2);

    img.setTo(Scalar(46, 16,200),mask1);
    img.setTo(Scalar(105,33,1),mask2);

    Mat oimg;
    resize(img,oimg,Size(1000,500));
    imshow("English Flag",oimg);
    waitKey(0);

    //crop

    Mat croimg;
    Rect roi(0,0,500,250);
    croimg=img(roi);
    imshow("Crop Image",croimg);
    waitKey(0);
}