#pragma once
#include <stack>

#include "Line.h"

using namespace std;

struct Edge
{
	double Xi = 0;
	double dX = 0;// 所在扫描线的增量
	int Ymax = 0;
	Edge* Next = nullptr;
};


class POLYGON
{
public:
	CPoint* m_Nodes; //顶点 (坐标为正整数)
	int m_NodeNum = 0; //顶点数
	COLORREF m_old_color; //填充前多边形的颜色
	COLORREF m_fill_color; //填充颜色
public:
	POLYGON(); //构造函数
	POLYGON(POLYGON& copy); //拷贝构造函数
	~POLYGON(); //析构函数
	void AddNode(CPoint& p);
	void ScanLineFill(CDC* pDC);//扫描线多边形填充算法
	void ScanLineSeedFill(CDC* pDC, CPoint seed);// < 扫描线种子填充算法 >
	void SeedFill4(CDC* pDC, CPoint initial_seed); //种子填充算法（区域不能太大，否则会出现溢出）
	void SeedFill4withStack(CDC* pDC, CPoint initial_seed);
	void DrawPolygon(CDC* pDC, COLORREF color); //绘制多边形
private:
	int MinInY(); //找到顶点中的y的最小值
	int MaxInY(); //找到顶点中的y的最大值
	void insertEdge(Edge* edge, Edge* ActiveHead);
	void makeEdgeTable(CPoint Lower, CPoint Upper, Edge** ET);
	void CreatEdgeTable(Edge** ET);
	void fillScan(CDC* pDC, int y, Edge* active);//填充一对交点
	void HorizonEdgeFill(CDC* pDC);//水平边直接画线填充
	void RefreshActiveList(int scan, Edge** ET, Edge* active);//填充完后，更新活动边表
	int FillLine(CDC* pDC, int Direct, CPoint seedPoint);
	void SearchLineNewSeed(CDC* pDC, stack<CPoint>& stk, int xLeft, int xRight, int y);
};

