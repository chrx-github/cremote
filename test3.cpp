#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//                                     Resize and Crop
int main()
{
    string path="opencvbasic/图形1.png";
    Mat img=imread(path);
    Mat imgResize,imgCrop;

    //cout<<img.size()<<endl;//输出图片的像素
    resize(img,imgResize,Size(1600,1200));//调整大小  注意Size函数要大写  法一：缩放程度由像素数决定
    resize(img,imgResize,Size(),0.5,0.5);//                           法二：缩放程度由倍率决定
    Rect roi(100,100,300,250);//X:100->100+300像素  Y:->100->100+250像素  Rect->矩形
    imgCrop=img(roi);//imgCrop取img的矩形部分
    imshow("Image",img);
    imshow("Image Resize",imgResize);
    imshow("Image Crop",imgCrop);
    waitKey(10000);
}