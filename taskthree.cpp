#include<opencv2/opencv.hpp>
#include<iostream>
#include<chrono>
#include<sstream>
#include<iomanip>
using namespace cv::dnn;
using namespace std;
using namespace cv;
using namespace std::chrono;

high_resolution_clock::time_point prevTime{},currTime{};
duration<double> lastTime;
stringstream ss;
Mat img,imgGray,blob,imgHSV,imgCrop,mask,canny;

class armor
{
public:
    Mat rvec,tvec;
    explicit armor(Mat rvec,Mat tvec):rvec(rvec),tvec(tvec){}
    float distance(){return cv::norm(tvec);};
};
vector<armor> armorBoard;

void currentFPS()
{
    float fps;
    currTime=high_resolution_clock::now();
    lastTime=currTime-prevTime;
    fps=1.0/lastTime.count();
    prevTime=currTime;
    ss<<std::fixed<<std::setprecision(2)<<fps;
}

void getLocation(RotatedRect m,RotatedRect n)
{
    vector<Point2f> Points_m(4);//在调用 RotatedRect 的 points 方法时，接受返回值的变量大小必须预先确定。这是因为 points 方法会填充一个固定大小的数组，通常是一个包含 4 个顶点的数组。如果你不预先分配足够的空间，可能会导致未定义行为或内存访问错误。
    // 或者Point2f Points_m[4];m.points(Points_m);for(int i=0;i<4;i++)也可以
    vector<Point2f> Points_n(4);
    m.points(Points_m.data());
    n.points(Points_n.data());

    vector<Point3f> obPoints={{0,0,0},{0,55,0},{0,27.5,0},{135,0,0},{135,55,0},{135,27.5,0}};
    vector<Point2f> imgPoints={(Points_m[0]+Points_m[1])*0.5,(Points_m[2]+Points_m[3])*0.5,m.center,(Points_n[0]+Points_n[1])*0.5,(Points_n[2]+Points_n[3])*0.5,n.center};
    //证明点的顺序：从tl开始，顺时针标记
    // for(int i=0;i<Points_m.size();i++)
    // {
    //     circle(img,Points_m[i],5,Scalar(255,0,255),-1);
    //     putText(img,to_string(i),{Points_m[i].x-5,Points_m[i].y-5},FONT_HERSHEY_DUPLEX,1.05,Scalar(255,255,225),2);
    // }
    // for(int i=0;i<Points_n.size();i++)
    // {
    //     circle(img,Points_n[i],5,Scalar(255,0,255),-1);
    //     putText(img,to_string(i),{Points_n[i].x-5,Points_n[i].y-5},FONT_HERSHEY_DUPLEX,1.05,Scalar(255,255,225),2);
    // }
    // 相机内参矩阵
    Mat cameraMatrix = (Mat_<double>(3, 3) << 
        1.7774091341308808e+03, 0., 7.1075979428865026e+02,
        0., 1.7754170626354828e+03, 5.3472407285624729e+02,
        0., 0., 1.
    );

    // 畸变系数
    Mat distCoeffs = (Mat_<double>(5, 1) << 
        -5.6313426428564950e-01, 1.8301501710641366e-01,
        1.9661478907901904e-03, 9.6259122849674621e-04,
        5.6883803390679100e-01
    );

    Mat rvec,tvec;
    bool found=cv::solvePnP(obPoints,imgPoints,cameraMatrix,distCoeffs,rvec,tvec);
    armorBoard.push_back(armor(rvec,tvec));
    cout<<armorBoard.back().rvec<<"  "<<armorBoard.back().tvec<<"  "<<armorBoard.back().distance()<<endl;
    if(found)
    {
        Mat R;//Mat 是一个多维数组类，可以用来表示各种数据结构，包括向量和矩阵
        Rodrigues(rvec,R);//向旋转向量转变为旋转矩阵

        Mat T=(Mat_<double>(4,4)<< 
            R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), tvec.at<double>(0),
            R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2), tvec.at<double>(1),
            R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2), tvec.at<double>(2),
            0, 0, 0, 1
        );
        //反投影2D点到3D点
        // for(const auto& point:imgPoints)//表示point是imagePoints中每个元素的常量引用。这种写法在C++中称为（Range-based for loop），用于遍历容器中所有元素。
        // {
        //     Mat uv1=(Mat_<double>(3,1)<<point.x, point.y,1);//<<初始化矩阵,其他用法:位左移(移动二进制表示的位次)，流插入(cout<<将内容插入到标准输出流中)
        //     Mat P = cameraMatrix.inv() * uv1;
        //     Mat X = T.inv() * P;
        //     X /= X.at<double>(3); // 归一化齐次坐标
        //     cout<<"Reprojected 3D Point:"<<X.t()<<endl;
        // }
    }
}
int main()
{
    // int choice=0;
    // int times=1;
    VideoCapture cap("../video/Infantry_red.avi");
    while(true)
    {
        cap.read(img);
        currentFPS();
        putText(img,"FPS:"+ss.str(),{1300,30},FONT_HERSHEY_DUPLEX,0.8,Scalar(255,255,255),1.5);
        ss={};
        int num{};
        if (img.empty())
        {
            cout << "End of video or error reading frame" << endl;
            break;
        }
        cvtColor(img,imgHSV,COLOR_BGR2HSV);
        // if(choice==1)
        // {
            // int hmin=77,hmax=114,smin=0,smax=255,vmin=210,vmax=255;//  blue
            // Scalar lower(hmin,smin,vmin);
            // Scalar upper(hmax,smax,vmax);
            // inRange(imgHSV,lower,upper,mask);
        // }
        // else
        // {
            int hmin=24,hmax=164,smin=63,smax=255,vmin=178,vmax=255;//  red
            Scalar lower1(0,smin,vmin);
            Scalar upper1(hmin,smax,vmax);
            Scalar lower2(hmax,smin,vmin);
            Scalar upper2(180,smax,vmax);
            Mat mask1,mask2;
            inRange(imgHSV,lower1,upper1,mask1);
            inRange(imgHSV,lower2,upper2,mask2);
            mask = mask1 | mask2 ;
        // }
        GaussianBlur(mask, mask, Size(5, 5), 0); 
        Canny(mask,canny,80,160,3,false);
        Mat kernel=getStructuringElement(0,Size(3,3));
        dilate(canny,canny,kernel);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(canny,contours,hierarchy,0,2,Point());
        vector<Rect> boundRect(contours.size());
        vector<RotatedRect> rect;   //灯条

        for(int n=0;n<contours.size();n++)
        {
            if(contourArea(contours[n])>100)
            {
                boundRect[n]=boundingRect(contours[n]);
                float a=boundRect[n].width;
                float b=boundRect[n].height;
                float aspRatio=b/a;
                if(aspRatio<2)
                    continue;
                Point2f points[4];
                
                rect.push_back(minAreaRect(contours[n]));
                rect.back().points(points);
                for(int i=0;i<4;i++)
                    line(img,points[i],points[(i+1)%4],Scalar(0,255,0),2,8,0);
            }
        }

        for(int i=0;i<rect.size()-1;i++)
        {
            for(int j=i+1;j<rect.size();j++)
            {
                float unit1,unit2;
                if(rect[i].angle<45.0)
                    unit1=rect[i].size.height;
                else if(rect[i].angle>45.0)
                    unit1=rect[i].size.width;
                if(rect[j].angle<45.0)
                    unit2=rect[j].size.height;
                else if(rect[i].angle>45.0)
                    unit2=rect[j].size.width;
                float dx=rect[j].center.x>rect[i].center.x ? rect[j].center.x-rect[i].center.x : rect[i].center.x-rect[j].center.x;
                if(rect[i].angle-rect[j].angle<-1||rect[i].angle-rect[j].angle>1
                ||rect[i].size.height-rect[j].size.height<-10||rect[i].size.height-rect[j].size.height>10
                ||rect[i].size.width-rect[j].size.width>6||rect[i].size.width-rect[j].size.width<-6
                )
                    continue;
                else 
                {
                    if(dx<=(unit1+unit2)*1.5&&dx>=(unit1+unit2))
                    {

                        circle(img,(rect[i].center+rect[j].center)*0.5,5,Scalar(0,0,255),-1);
                        RotatedRect armorRect((rect[i].center+rect[j].center)*0.5,Size2f(unit1+unit2,unit1+unit2),rect[i].angle);
                        Point2f Points[4];
                        armorRect.points(Points);     
                        for(int t=0;t<4;t++)
                            line(img,Points[t],Points[(t+1)%4],Scalar(0,255,0),2,8,0);
                        //getLocation
                        getLocation(rect[i],rect[j]);
                    }
                }
            }
        }
        armorBoard.clear();
        imshow("img",img);
        waitKey(0);
    }
}