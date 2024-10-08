#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat imgGray;
Mat imgBlur;
Mat imgCanny;
Mat imgDia;
Mat imgErode;

string objectType;

void getContours(Mat imgDia,Mat img)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(imgDia,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    //drawContours(img,contours,-1,Scalar(255,0,255),2);
    
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    for(int i=0;i<contours.size();i++)
    {
        int area=contourArea(contours[i]);//conPoly 将会被初始化为拥有与 contours 同样多的元素
        cout<<area<<endl;

        if(area>100)
        {
            float peri=arcLength(contours[i],true);
            //计算 contours 向量中第 i 个轮廓的周长，并假设这个轮廓是闭合的，然后将结果存储在 peri 变量中。
            approxPolyDP(contours[i],conPoly[i],0.02*peri,true);
            /*
            将 contours[i] 轮廓简化为一个近似的多边形，并将结果存储在 conPoly[i] 中。具体来说：
            contours[i] 是原始的轮廓。
            conPoly[i] 是经过多边形逼近后的轮廓。
            0.02 * peri 控制了多边形逼近的精度，值越小，多边形越接近原始轮廓。
            true 表示输出的多边形是闭合的。
            */
            cout<<conPoly[i].size()<<endl;
            boundRect[i]=boundingRect(conPoly[i]);//计算 conPoly[i] 轮廓的最小外接矩形，并返回一个 Rect 对象。
            
            int objCor=(int)conPoly[i].size();
            
            if(objCor==3){objectType="Triangle";}
            else if(objCor==4)
            {
                float aspRatio=(float)boundRect[i].width/(float)boundRect[i].height;//判断正方形
                if(aspRatio<1.1 && aspRatio>0.9)
                    objectType="Square";
                else
                    objectType="Rectangle";
            }
            else if(objCor>4){objectType="Circle";}
            
            drawContours(img,conPoly,i,Scalar(255,0,255),2);
            rectangle(img,boundRect[i].tl(),boundRect[i].br(),Scalar(0,255,0),5);//绘制矩形，tl为topleft,br为bottomright
            putText(img,objectType,{boundRect[i].x,boundRect[i].y-5},FONT_HERSHEY_DUPLEX,1.05,Scalar(0,69,225),1.5);

            
        }
    }
}
//                                           
int main()
{
    string path="opencvbasic/图形1.png";
    Mat img=imread(path);
    

    cvtColor(img,imgGray,COLOR_BGR2GRAY);
    GaussianBlur(img,imgBlur,Size(3,3),0,0);
    Canny(imgBlur,imgCanny,25,75);
    Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3));
    dilate(imgCanny,imgDia,kernel);

    getContours(imgDia,img);

    imshow("Image",img);
    // imshow("Image Gray",imgGray);
    // imshow("Image Blur",imgBlur);
    // imshow("Image Canny",imgCanny);
    // imshow("Image Dilation",imgDia);
    
    waitKey(0);
}