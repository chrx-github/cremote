//#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//                                      importing images
int main()
{
    string path="opencvbasic/图形1.png";
    Mat img=imread(path);//Mat是opencv引入的矩阵数据类型
    imshow("Image",img);
    waitKey(0);//0为无穷大的延迟
}


//                                     Video
// int main()
// {
//     string path="opencvbasic/gold_video_1.avi";
//     VideoCapture cap(path);
//     Mat img;
//     while(true)
//     {
//         cap.read(img);
//         imshow("Image",img);
//         waitKey(1);//0为无穷大的延迟
//     }
    
// }


//                                     webcam
// int main()
// {
//     VideoCapture cap(0);//---->括号里填入相机ID
//     Mat img;
//     while(true)
//     {
//         cap.read(img);
//         imshow("Image",img);
//         waitKey(1);//0为无穷大的延迟
//     }
// }