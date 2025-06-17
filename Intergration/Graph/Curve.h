#pragma once
#include<vector>
#include "Polygon.h"

class CURVE
{
public:
	vector<CPoint> m_CtrlPt;//控制点数组 (坐标为正整数)
	COLORREF m_color; //绘制曲线的颜色
	//int nCtrlPt;//控制点数
public:
	CURVE(void); //构造函数
	CURVE(vector<CPoint>& CtrlPt); //重载构造函数
	CURVE(POLYGON& polygon); //重载构造函数
public:
	void DrawBzier(CDC* pDC); //根据控制点，求曲线上的nBezPts个点
	void Bezier(CDC* pDC, int nBezPts);
private:
	int binomialCoeffs(int k, int n); //计算组合系数Cn^k
	void computeBezierPt(float t, CPoint& bezierPt); //计算Bezier曲线上点的坐标
};

