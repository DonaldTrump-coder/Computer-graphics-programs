#pragma once
#include "Line.h"
#include "Polygon.h"


//以下是直线裁剪相关算法
class LINE_CLIP : public LINE
{
public:
	LINE_CLIP(); //构造函数
	LINE_CLIP(CPoint Begin, CPoint End, COLORREF color); //重载构造函数

private:
	//Sothland算法相关函数
	int Encode(CPoint p, int XL, int XR, int YB, int YT);
	//中点算法相关函数
	CPoint MidClipPoint(int XL, int XR, int YB, int YT);
	bool NotIntersect(int XL, int XR, int YB, int YT);
	bool IsInArea(CPoint P, int XL, int XR, int YB, int YT);
	//梁有栋算法相关函数
	bool ClipT(int p, int q, float& u1, float& u2);
public:
	//直线裁剪算法，若直线完全在窗口内或者明显在窗口外，则返回false，否则返回true
	//Sothland算法相关函数
	bool LineClip(int XL, int XR, int YB, int YT, LINE& line);
	//中点算法相关函数
	bool LineMidClip(int XL, int XR, int YB, int YT, LINE& L);
	//梁有栋算法相关函数
	bool LBClip(int XL, int XR, int YB, int YT, LINE& line);
};


//以下是多边形裁剪算法
class POLYGON_CLIP : public POLYGON
{
public:
	POLYGON_CLIP(); //构造函数
	POLYGON_CLIP(POLYGON_CLIP& copy); //拷贝构造函数
	~POLYGON_CLIP(); //析构函数
private:
	bool isVisible(CPoint& P, LINE& ClipEdge);
	CPoint Intersect(LINE& PolygonEdge, LINE& ClipEdge);
	void SingleEdgeClip(LINE& ClipEdge, POLYGON_CLIP& NewPolygon);
	void SHCheck(int XL, int XR, int YB, int YT);
	void SHDrawPolygon(CDC* pDC, int XL, int XR, int YB, int YT);
	void SortInX();
	void SortInY();
	bool IsPolygonNeedClip(int XL, int XR, int YB, int YT); //判断是否需要裁剪
public:
	void operator = (POLYGON_CLIP& A); //重载运算符
	void SetPolygonClip(POLYGON& polygon); //根据多变形的值设置多边形裁剪的值
	bool Surtherland_Hondgman(int XL, int XR, int YB, int YT, POLYGON_CLIP& polygon_out);
};
