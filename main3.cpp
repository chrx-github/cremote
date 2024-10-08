#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <iostream>
#include<filesystem>

using namespace std;
using namespace cv;

Mat imgcan,img;

class Shape
{
public:
    //析构函数
    virtual ~Shape() {}
    // 拷贝构造函数
    Shape(const Shape& other) : name(other.name) {}
    // 显式构造函数
    explicit Shape(const string& name) : name(name) {}
    // 纯虚函数
    virtual double area() const = 0;
    const string& getname() const { return name; }
protected:
    string name; // 形状的名字
};

class Circle : public Shape
{
public:
    ~Circle(){}
    Circle(const Circle& other) : Shape(other), radius(other.radius){}
    // explicit Circle(double radius, const string& name = "Circle"){}
    explicit Circle(double radius, const string& name = "Circle") : Shape(name), radius(radius) {}
    double area() const override
    {
        return 3.14*radius*radius;
    }
    double getRadius() const { return radius; }
private:
    double radius;
};

class Rectangle : public Shape
{
public:
    ~Rectangle(){}
    Rectangle(const Rectangle& other) : Shape(other),width(other.width),height(other.height){}
    explicit Rectangle(double width,double height,const string& name="Rectangle"):Shape(name),width(width),height(height) {}
    double area() const override
    {
        return width*height;
    }
private:
    double width,height;
};

class Triangle : public Shape
{
public:
    ~Triangle(){}
    Triangle(const Triangle& other) : Shape(other),width(other.width),height(other.height){}
    explicit Triangle(double width,double height,const string& name="Triangle"):Shape(name),width(width),height(height){}
    double area() const override
    {
        return width*height/2;
    }
private:
    double width,height;
};

void getContours(Mat img,Mat imgcan)
{
    vector<Shape*> graph;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgcan,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());



    // int sum{0};

    for(int i=0;i<contours.size();i++)
    {
        approxPolyDP(contours[i], conPoly[i], 10, true);
        boundRect[i]=boundingRect(conPoly[i]);//计算 conPoly[i] 轮廓的最小外接矩形，并返回一个 Rect 对象。
        int objCor=(int)conPoly[i].size();

        // if(objCor==3)
        // {
        //     graph[i]=new Triangle(boundRect[i].width,boundRect[i].height);
        //     putText(imgcan,graph[i].name+" base="+to_string(boundRect[i].width)+" height="+to_string(boundRect[i].height)+" S="+to_string(graph[i].area()),{boundRect[i].x,boundRect[i].y-5},FONT_HERSHEY_DUPLEX,1.05,Scalar(0,69,225),1.5);
        //     // sum++;
        // }
        // else if(objCor==4)
        // {
        //     graph[i]=new Rectangle(boundRect[i].width,boundRect[i].height);
        //     putText(imgcan,graph[i].name+" width="+to_string(boundRect[i].width)+" height="+to_string(boundRect[i].height)+" S="+to_string(graph[i].area())+" aspRatio="+to_string(boundRect[i].width/boundRect[i].height),{boundRect[i].x,boundRect[i].y-5},FONT_HERSHEY_DUPLEX,1.05,Scalar(0,69,225),1.5);
        // }
        if (objCor == 3)
        {
            graph.push_back(new Triangle(boundRect[i].width, boundRect[i].height));
            putText(img, graph.back()->getname() + " base=" + to_string(boundRect[i].width) + " height=" + to_string(boundRect[i].height) + " S=" + to_string(graph.back()->area()), {boundRect[i].x-75, boundRect[i].y - 5}, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 69, 225), 1.5);
        }
        else if (objCor == 4)
        {
            graph.push_back(new Rectangle(boundRect[i].width, boundRect[i].height));
            float a=boundRect[i].width,b=boundRect[i].height,aspRatio=a>b?a/b:b/a;
            putText(img, graph.back()->getname()+ " width=" + to_string(boundRect[i].width) + " height=" + to_string(boundRect[i].height) + " S=" + to_string(graph.back()->area()) + " aspRatio=" + to_string(aspRatio), {boundRect[i].x-75, boundRect[i].y - 5}, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 69, 225), 1.5);
        }
        else if(objCor>5)
        {
            // cv::HoughCircles(edges, circles, cv::HOUGH_GRADIENT, 1, img.rows / 8, 100, 30, 1, 30);
            // HoughCircles(contours[i], circles, cv::HOUGH_GRADIENT, 1, img.rows / 8, 100, 30, 1, 30);
            // graph[i]=new Circle(boundRect[i].radius);
            graph.push_back(new Circle(boundRect[i].width/2));
            Circle* circle = dynamic_cast<Circle*>(graph.back());
            putText(img,circle->getname()+" radius="+to_string(circle->getRadius())+" S="+to_string(circle->area()),{boundRect[i].x-75,boundRect[i].y-5},FONT_HERSHEY_DUPLEX,0.5,Scalar(0,69,225),1.5);
        }
    }
    // vector<Vec3f> circles;
    // HoughCircles(imgcan, circles, HOUGH_GRADIENT, 1, img.rows / 8, 100, 30, 1, 300);
    // for(int i=0;i<circles.size();i++)
    // {
    //     graph[sum+i]=new Circle(circles[i][2]);
    //     putText(imgcan,graph[sum+i].name+" radius="+to_string(graph[sum+i].radius)+" S="+to_string(graph[sum+i].area()));
    // }


    // delete[] graph;//针对数组
    for (auto& shape:graph)//针对一个一个new的对象
    {
        delete shape;
    }
    imshow("Detailed Graph",img);
    waitKey(0);
}

int main()
{
    img=imread("../resources/图形1.png");
    Canny(img,imgcan,2,10);
    getContours(img,imgcan);
    
    img=imread("../resources/图形2.png");
    Canny(img,imgcan,2,10);
    getContours(img,imgcan);
    
    img=imread("../resources/图形3.png");
    Canny(img,imgcan,2,10);
    getContours(img,imgcan);
    
}