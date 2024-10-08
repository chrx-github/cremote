//#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//                                           Basic Functions
int main()
{
    string path="../tdttask/图形1.png";
    Mat img=imread(path);
    Mat imgGray;
    Mat imgBlur;
    Mat imgCanny;
    Mat imgDia;
    Mat imgErode;

    cvtColor(img,imgGray,COLOR_BGR2GRAY);//转换颜色为灰色  其中img为原图像  imgGray为目标图像
    GaussianBlur(img,imgBlur,Size(1,9),0,0);//Size(x,y)表示X轴和Y轴的模糊程度
    Canny(imgBlur,imgCanny,1,4);//边缘检测函数    --->数越小，输出的边缘越多
    Mat kernel=getStructuringElement(MORPH_RECT,Size(1,9));//创建可使用的膨胀内核 Size( , )中数越大，膨胀程度越大 注意：要用奇数！
    dilate(imgCanny,imgDia,kernel);// 膨胀
    erode(imgCanny,imgErode,kernel);//腐蚀
    
    imshow("Image",img);
    imshow("Image Gray",imgGray);
    imshow("Image Blur",imgBlur);
    imshow("Image Canny",imgCanny);
    imshow("Image Dilation",imgDia);
    imshow("Image Erode",imgErode);
    
    waitKey(0);
}
