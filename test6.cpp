#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgHSV,mask;
int hmin=0,smin=110,vmin=153;
int hmax=19,smax=240,vmax=255;

//           Color Detection        用HSV更好 :  H代表色调（Hue），S代表饱和度（Saturation），V代表亮度（Value）

// int main()
// {
//     string path="opencvbasic/图形1.png";
//     Mat img=imread(path);

//     cvtColor(img,imgHSV,COLOR_BGR2HSV);//转色

//     Scalar lower(hmin,smin,vmin);//定义上下限
//     Scalar upper(hmax,smax,vmax);
//     inRange(imgHSV,lower,upper,mask);
//     //inRange函数会检查输入数组中的每个元素是否位于给定的范围（由lower和upper定义）内。
//     //对于那些满足条件的元素，输出数组mask中对应的元素将被设置为255（白色），否则设置为0（黑色）。
//     //mask实际上是一个二值图像，它标识了原图像中哪些部分的颜色落在指定的HSV范围内

//     imshow("Image",img);
//     imshow("Image HSV",imgHSV);
//     imshow("Image Mask",mask);
//     waitKey(0);
// }


int main()
{
    string path="opencvbasic/图形1.png";
    Mat img=imread(path);

    cvtColor(img,imgHSV,COLOR_BGR2HSV);

    namedWindow("Trackbars",(640,200));//用于创建一个窗口以显示图像。这个函数允许你指定窗口的名称和一些可选的属性。
    createTrackbar("Hue Min","Trackbars",&hmin,179);//创建跟踪栏
    createTrackbar("Hue Max","Trackbars",&hmax,179);
    createTrackbar("Sat Min","Trackbars",&smin,255);
    createTrackbar("Sat Max","Trackbars",&smax,255);
    createTrackbar("Val Min","Trackbars",&vmin,255);
    createTrackbar("Val Max","Trackbars",&vmax,255);
    /*
    H（色调）：通常表示为0到360度，但是当应用于计算机视觉和数字图像处理时，H的值通常会被缩放到0到179（或者0到255，取决于具体的应用和库）。
    这是因为计算机图像处理中为了节省内存，通常使用8位整数来表示每个通道，所以最大值为255。
    然而，在OpenCV中，默认情况下H的范围是0到179，这样可以提供更精细的控制同时保持8位精度。
    
    S（饱和度）：表示色彩的纯度或强度，范围是0到255。0表示完全无色（灰色），255表示完全饱和（最纯的色彩）。
    
    V（明度/亮度）：表示色彩的明亮程度，范围同样是0到255。0表示完全黑暗（黑色），255表示最亮。
    */
    while(true)
    {
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax,smax,vmax);
        inRange(imgHSV,lower,upper,mask);

        imshow("Image",img);
        imshow("Image HSV",imgHSV);
        imshow("Image Mask",mask);
        waitKey(1);
    }

    
}