#pragma once
class LINE
{
public:
	CPoint m_Begin = CPoint(0, 0); //起点
	CPoint m_End = CPoint(0, 0); //终点
	COLORREF m_color = RGB(0, 0, 0); //画线颜色
public:
	LINE(void); //构造函数
	LINE(CPoint Begin, CPoint End, COLORREF color); //重载构造函数

	void DDA(CDC* pDC); //DDA画线法
	void Bresenham1(CDC* pDC); //Bresenham算法1，其中还有浮点算运算
	void Bresenham2(CDC* pDC); //Bresenham算法2，该算法完全去掉了浮点数运算
	void Bresenham3(CDC* pDC); //Bresenham法画虚线
	void MidPointLine(CDC* pDC); // 中点画线法
};

