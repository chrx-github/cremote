#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>
#include <chrono>
#include <iomanip>  // 包含 setprecision
#include <sstream>
#include<filesystem>

using namespace std;
using namespace cv;
using namespace std::chrono;

float fps;
high_resolution_clock::time_point prevTime{},currTime{};
duration<double> lastTime;
stringstream ss;

// void ensureDirectoryExists(const string& path) {
//     if (!filesystem::exists(path)) {
//         filesystem::create_directories(path);
//     }
// }

void fpscounter()
{
    currTime=high_resolution_clock::now();
    lastTime=currTime-prevTime;
    fps=1.0/lastTime.count();
    ss<<std::fixed<<std::setprecision(2)<<fps;
}

void interruption()
{
    string path="gold_video_1.avi";
    VideoCapture cap(path);
    Mat img;
    duration<double> interruptionTime{};
    // if(!cap.isOpened())
    // {
    //     cerr<<"Error!!!"<<endl;
    //     return -1;
    // }
    while(true)
    {
        cap.read(img);
        if(img.empty())//attention：empty对象为图片
            break;
        fpscounter();
        interruptionTime+=lastTime;
        if(interruptionTime.count()>5.0)
            break;
        putText(img,"FPS:"+ss.str(),{420,30},FONT_HERSHEY_DUPLEX,0.8,Scalar(255,255,255),1.5);//
        ss={};//
        imshow("Image",img);
        prevTime=currTime;
        waitKey(2);//50
    }
}

int main()
{
    // ensureDirectoryExists("pictures");
    string path="Infantry_red.avi";
    VideoCapture cap(path);
    Mat img;
    duration<double> sumlastTime{};
    bool flag{};
    int n{1};

    if(!cap.isOpened())
    {
        cerr<<"Error!!!"<<endl;
        return -1;
    }
    prevTime=high_resolution_clock::now();
    while(true)
    {
        cap.read(img);
        if(img.empty())//attention：empty对象为图片
            break;
        fpscounter();
        if(!flag)
        {
            sumlastTime+=lastTime;
            if(sumlastTime.count()>2.0)
            {
                interruption();
                flag=true;
            } 
        }
        putText(img,"FPS:"+ss.str(),{1280,30},FONT_HERSHEY_DUPLEX,0.8,Scalar(255,255,255),1.5);//
        ss={};//
        imshow("Image",img);
        prevTime=currTime;
        if(waitKey(2)=='q')  // 按 'q' 键退出
            break;
        if(waitKey(2)=='s')
        {
            string filename="taskone/pictures/Screenshots_"+to_string(n)+".png";
            imwrite(filename,img);
            n++;
        }
    }
    cap.release();

    //webcam
    VideoCapture capt(0);
    prevTime = high_resolution_clock::now();
    while(true)
    {
        capt.read(img);
        fpscounter();
        putText(img,"FPS:"+ss.str(),{800,30},FONT_HERSHEY_DUPLEX,0.8,Scalar(255,255,255),1.5);//
        ss={};//
        imshow("Image",img);
        if (waitKey(1) == 'q')  // 按 'q' 键退出
            break;
    }
    capt.release();
    destroyAllWindows();
}