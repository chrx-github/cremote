#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>
#include<opencv2/dnn.hpp>
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

void currentFPS()
{
    float fps;
    currTime=high_resolution_clock::now();
    lastTime=currTime-prevTime;
    fps=1.0/lastTime.count();
    prevTime=currTime;
    ss<<std::fixed<<std::setprecision(2)<<fps;
}

Mat img,imgGray,blob,imgHSV,imgCrop,mask,canny;
int main()
{
    // int choice=0;
    // int times=1;
    VideoCapture cap("../video/Infantry_red.avi");
    while(true)
    {
// cout<<endl<<"loop for "<<times<<" times"<<endl;times++;
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
        vector<RotatedRect> rect;

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
// circle(img,rect.back().center,10,Scalar(255,0,255),-1);
                for(int i=0;i<4;i++)
                    line(img,points[i],points[(i+1)%4],Scalar(0,255,0),2,8,0);
// cout<<rect.size()<<" "<<rect.back().center<<" angle:"<<rect.back().angle<<" width="<<rect.back().size.width<<" height="<<rect.back().size.height;
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
// cout<<"  dx="<<dx<<" armorRect.width="<<unit1+unit2;
                    // cout<<unit1<<" "<<unit2<<"     "<<dx<<"          ";
                    // cout<<rect[i].size.width;
                    // rect[i].size.width<rect[i].size.height?cout<<"<":cout<<">";
                    // cout<<rect[i].size.height<<" "<<rect[i].angle<<" "<<rect[i].center<<"   "<<rect[j].size.width;
                    // rect[j].size.width<rect[j].size.height?cout<<"<":cout<<">";
                    // cout<<rect[j].size.height<<" "<<rect[j].angle<<" "<<rect[j].center<<endl;
                
                if(rect[i].angle-rect[j].angle<-1||rect[i].angle-rect[j].angle>1
                ||rect[i].size.height-rect[j].size.height<-10||rect[i].size.height-rect[j].size.height>10
                ||rect[i].size.width-rect[j].size.width>6||rect[i].size.width-rect[j].size.width<-6
                )
                    continue;
                else 
                {
                    
                    // if(rect[i].angle<45.0)
                    //     unit1=rect[i].size.height;
                    // else if(rect[i].angle>45.0)
                    //     unit1=rect[i].size.width;
                    // if(rect[j].angle<45.0)
                    //     unit2=rect[j].size.height;
                    // else if(rect[i].angle>45.0)
                    //     unit2=rect[j].size.width;
                    if(dx<=(unit1+unit2)*1.5&&dx>=(unit1+unit2))
                    {

                        circle(img,(rect[i].center+rect[j].center)*0.5,5,Scalar(0,0,255),-1);
                        RotatedRect armorRect((rect[i].center+rect[j].center)*0.5,Size2f(unit1+unit2,unit1+unit2),rect[i].angle);
                        Point2f Points[4];
                        armorRect.points(Points);     
                        for(int t=0;t<4;t++)
                            line(img,Points[t],Points[(t+1)%4],Scalar(0,255,0),2,8,0);
// cout<<"Success recognition!!!Success recognition!!!Success recognition!!!Success recognition!!!Success recognition!!!Success recognition!!!"<<endl;
                    }
                }
            }
        }
        imshow("img",img);
        waitKey(100);
    }
}