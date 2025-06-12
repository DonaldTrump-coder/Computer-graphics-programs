#pragma once
class CIRCLE
{
private:
	CPoint m_CentralPoint; //圆或椭圆的中心点
	int m_Radius; //圆的半径
	int m_Ellipse_a; //椭圆的长半轴
	int m_Ellipse_b; //椭圆的短半轴
	COLORREF m_color; //画圆颜色

public:
	CIRCLE(); //构造函数
	CIRCLE(CPoint CentralPoint, int Radius, COLORREF color); //重载构造函数
	CIRCLE(CPoint CentralPoint, int Ellipse_a, int Ellipse_b, COLORREF color); //重载构造函数
private:
	void CirclePoints(int x, int y, CDC* pDC); //描圆曲线的点
	void EllipsePoints(int x, int y, CDC* pDC); //描椭圆曲线的点
public:
	void MidPointCircle(CDC* pDC); //中点法画圆
	void BresenhamCircle(CDC* pDC); //Bresenham法画圆
	void MidPointEllipse(CDC* pDC); //中点法画椭圆
	//void MidPointEllipse2(CDC* pDC); //中点法画椭圆,整型运算，非浮点运算
};

