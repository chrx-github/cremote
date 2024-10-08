#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

float w=250,h=300;
Mat matrix,imgWarp;
//                            Warp Images

int main()
{
    string path="opencvbasic/图形1.png";
    Mat img=imread(path);
    
    Point2f src[4]={{116,156},{681,105},{103,779},{644,899}};//扭曲图形的四个点
    Point2f dst[4]={{0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h}};//w(idth) h(eight)
    
    matrix=getPerspectiveTransform(src,dst);
    warpPerspective(img,imgWarp,matrix,Point(w,h));

    for(int i=0;i<4;i++)
        circle(img,src[i],10,Scalar(0,0,255),FILLED);

    imshow("Image",img);
    imshow("Image Warp",imgWarp);
    waitKey(0);//K大写
}