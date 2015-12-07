// useClipper.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "clipper.hpp"

using namespace std;

class xdpoint   //��������
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

typedef std::vector< xdpoint > polygon;   //�������Σ��������Ϊһ���ĳ������
typedef std::vector< polygon > polygons;   //����ö����Σ��������Ϊһ���ȫ��������ɵ�����

inline polygon& operator <<(polygon& poly, const xdpoint& p) {poly.push_back(p); return poly;}  //����polygon����xdpoint��������������ӵ�
inline polygons& operator <<(polygons& polys, const polygon& p) {polys.push_back(p); return polys;}  //����polygons����polygons���������������polygons

std::ostream& operator <<(std::ostream &s, const polygon &p) //����polygon�����������ʹ��cout�����ʾ
{
	if (p.empty()) return s;
	polygon::size_type last = p.size() -1;
	for (polygon::size_type i = 0; i < last; i++)
		s << "(" << p[i].x << "," << p[i].y << "), ";
	s << "(" << p[last].x << "," << p[last].y << ")\n";
	return s;
}
std::ostream& operator <<(std::ostream &s, const polygons &p) //����polygons�����������ʹ��cout�����ʾ
{
	for(polygons::size_type i=0;i!=p.size();++i)
		s<<p[i];
	return s;
}

static const float scale_factor=100000;  //����Ŵ���������Ϊ����clipper֮ǰҪ�Ŵ�

int _tmain(int argc, _TCHAR* argv[])
{
	//��һ������Ҫƫ�õ���������
	polygon contour1,contour2;
	polygons contours;
	contour1<<xdpoint(0,0)<<xdpoint(1,0)<<xdpoint(1,1)<<xdpoint(0,1);  //clipper�����������һ���㲻���ڵ�һ���㣬�Լ�����
	contour2<<xdpoint(2,2)<<xdpoint(3,0)<<xdpoint(4,2);
	contours<<contour1<<contour2;
	cout<<"����ƫ��֮ǰ���������ǣ�"<<contours<<endl;
	//�ڶ�����������ת��Ϊclipper����ʹ�õ�paths��
	ClipperLib::Paths input,output;  //input���ת������������ݣ�output��ƫ�ú�Ľ��
	for (int i=0;i!=contours.size();++i)
	{
		ClipperLib::Path temIn;
		for (int j=0;j!=contours[i].size();++j)
		{
			temIn<<ClipperLib::IntPoint(contours[i][j].x*scale_factor,contours[i][j].y*scale_factor);
		}
		input<<temIn;
	}
	cout<<"ת����clipper����ʹ�õ���������Ϊ��"<<input;
	//������������ƫ����
	ClipperLib::ClipperOffset Offset;
	Offset.AddPaths(input, ClipperLib::jtSquare, ClipperLib::etClosedPolygon);  
	Offset.Execute(output, -0.2*scale_factor);  //����ƫ��0.2 
	cout<<"ƫ�ú��clipper����ǣ�"<<output;
	//���Ĳ���ת�����Լ��ĵ���
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
	cout<<"ƫ�ú�ת��Ϊ�Լ����ݸ�ʽ�Ľ���ǣ�"<<myoutput;
	cin.get();  //Ϊ��ʹ����̨����ʧ
	return 0;
}

