// useClipper.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "clipper.hpp"

using namespace std;

class xdpoint   //基本点类
{
public:
	float x;
	float y;
	xdpoint():x(0),y(0){}
	xdpoint(float a,float b):x(a),y(b){}  
	friend inline bool operator== (const xdpoint& a, const xdpoint& b)
	{
		return a.x == b.x && a.y == b.y;
	}
	friend inline bool operator!= (const xdpoint& a, const xdpoint& b)
	{
		return a.x != b.x  || a.y != b.y;
	}

};

typedef std::vector< xdpoint > polygon;   //定义多边形，可以理解为一层的某个轮廓
typedef std::vector< polygon > polygons;   //定义好多多边形，可以理解为一层的全部轮廓组成的数据

inline polygon& operator <<(polygon& poly, const xdpoint& p) {poly.push_back(p); return poly;}  //重载polygon加入xdpoint的输入流，方便加点
inline polygons& operator <<(polygons& polys, const polygon& p) {polys.push_back(p); return polys;}  //重载polygons加入polygons的输入流，方便加polygons

std::ostream& operator <<(std::ostream &s, const polygon &p) //重载polygon输出流，方便使用cout输出显示
{
	if (p.empty()) return s;
	polygon::size_type last = p.size() -1;
	for (polygon::size_type i = 0; i < last; i++)
		s << "(" << p[i].x << "," << p[i].y << "), ";
	s << "(" << p[last].x << "," << p[last].y << ")\n";
	return s;
}
std::ostream& operator <<(std::ostream &s, const polygons &p) //重载polygons输出流，方便使用cout输出显示
{
	for(polygons::size_type i=0;i!=p.size();++i)
		s<<p[i];
	return s;
}

static const float scale_factor=100000;  //定义放大因数，因为进入clipper之前要放大

int _tmain(int argc, _TCHAR* argv[])
{
	//第一步：需要偏置的轮廓数据
	polygon contour1,contour2;
	polygons contours;
	contour1<<xdpoint(0,0)<<xdpoint(1,0)<<xdpoint(1,1)<<xdpoint(0,1);  //clipper可以输入最后一个点不等于第一个点，自己会封闭
	contour2<<xdpoint(2,2)<<xdpoint(3,0)<<xdpoint(4,2);
	contours<<contour1<<contour2;
	cout<<"进行偏置之前的轮廓点是："<<contours<<endl;
	//第二步：将数据转换为clipper可以使用的paths类
	ClipperLib::Paths input,output;  //input存放转换后的轮廓数据，output存偏置后的结果
	for (int i=0;i!=contours.size();++i)
	{
		ClipperLib::Path temIn;
		for (int j=0;j!=contours[i].size();++j)
		{
			temIn<<ClipperLib::IntPoint(contours[i][j].x*scale_factor,contours[i][j].y*scale_factor);
		}
		input<<temIn;
	}
	cout<<"转换成clipper可以使用的轮廓数据为："<<input;
	//第三步：构造偏置类
	ClipperLib::ClipperOffset Offset;
	Offset.AddPaths(input, ClipperLib::jtSquare, ClipperLib::etClosedPolygon);  
	Offset.Execute(output, -0.2*scale_factor);  //向内偏置0.2 
	cout<<"偏置后的clipper结果是："<<output;
	//第四步：转换成自己的点类
	polygons myoutput;
	for (int i=0;i!=output.size();++i)
	{
		polygon temIn;
		for (int j=0;j!=output[i].size();++j)
		{
			temIn<<xdpoint(output[i][j].X/scale_factor,output[i][j].Y/scale_factor);
		}
		myoutput<<temIn;
	}
	cout<<"偏置后转换为自己数据格式的结果是："<<myoutput;
	cin.get();  //为了使控制台不消失
	return 0;
}

