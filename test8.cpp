// Chapter 8
// facial recognition
// leave it learnt later
// 2:16:00

// Virtual Paint
// 2:22:26

#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/objdetect.hpp>//对应头文件
#include <iostream>

using namespace std;
using namespace cv;

//                                  Images
int main()
{
    string path = "Resources/test.png";
    Mat img = imread(path);
    CascadeClassifier faceCascade;
    faceCascade.load("Resources/haarcascade_frontalface_default.xm1");
    if(faceCascade.empty()){ cout<<"XML file not loaded" << endl;}
    vector<Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10);
    for(int i=0;i<faces.size();i++)
    {
        rectangle(img,faces[i].tl(),faces[i].br(),Scalar(255,255,255),3);
    }
    imshow("Facial Recognition",img);
}