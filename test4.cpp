#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//                                     Draw Shapes and Write Text1:10:36
int main()
{
    Mat img(512,512,CV_8UC3,Scalar(125,25,50));//512,512图片大小;8为2^8 U为unsigned 合起来表示0～255;(255,0,0)表示B255/G0/R0
    circle(img,Point(256,256),100,Scalar(0,125,0),10);//img图形放入的图片,Point为圆心,100半径,10为边缘厚度,可改为FILLED表示填充
    rectangle(img,Point(130,226),Point(382,326),Scalar(212,212,252),10);//Point表示矩形的对顶点
    line(img,Point(130,260),Point(450,260),Scalar(256,256,256),2);

    putText(img,"CHRX EMPIRE",Point(0,510),FONT_HERSHEY_DUPLEX,1.05,Scalar(0,69,225),1.5);//FONT_HERSHEY_DUPLEX字体,1.05为字体大小,1.5为厚度
    imshow("img",img);
    waitKey(1000);
}