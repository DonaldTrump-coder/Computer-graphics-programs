#include "pch.h"
#include "CIRCLE.h"

//构造函数
CIRCLE::CIRCLE()
{
	m_CentralPoint = CPoint(0, 0);
	m_Radius = 0;
	m_Ellipse_a = 0;
	m_Ellipse_b = 0;
	m_color = RGB(0, 0, 0);
}

//重载构造函数
CIRCLE::CIRCLE(CPoint CentralPoint, int Radius, COLORREF color)
{
	m_CentralPoint = CentralPoint;
	m_Radius = Radius;
	m_color = color;
}

//重载构造函数
CIRCLE::CIRCLE(CPoint CentralPoint, int Ellipse_a, int Ellipse_b, COLORREF color)
{
	m_CentralPoint = CentralPoint;
	m_Ellipse_a = Ellipse_a;
	m_Ellipse_b = Ellipse_b;
	m_color = color;
}

//描圆曲线的点
void CIRCLE::CirclePoints(int x, int y, CDC* pDC)
{
	pDC->SetPixel(m_CentralPoint.x + x, m_CentralPoint.y + y, m_color);
	pDC->SetPixel(m_CentralPoint.x - x, m_CentralPoint.y + y, m_color);
	pDC->SetPixel(m_CentralPoint.x + x, m_CentralPoint.y - y, m_color);
	pDC->SetPixel(m_CentralPoint.x - x, m_CentralPoint.y - y, m_color);
	pDC->SetPixel(m_CentralPoint.x + y, m_CentralPoint.y + x, m_color);
	pDC->SetPixel(m_CentralPoint.x - y, m_CentralPoint.y + x, m_color);
	pDC->SetPixel(m_CentralPoint.x + y, m_CentralPoint.y - x, m_color);
	pDC->SetPixel(m_CentralPoint.x - y, m_CentralPoint.y - x, m_color);
}

//描椭圆曲线的点
void CIRCLE::EllipsePoints(int x, int y, CDC* pDC)
{
	pDC->SetPixel(m_CentralPoint.x + x, m_CentralPoint.y + y, m_color);
	pDC->SetPixel(m_CentralPoint.x - x, m_CentralPoint.y + y, m_color);
	pDC->SetPixel(m_CentralPoint.x - x, m_CentralPoint.y - y, m_color);
	pDC->SetPixel(m_CentralPoint.x + x, m_CentralPoint.y - y, m_color);
}

//中点法画圆
void CIRCLE::MidPointCircle(CDC* pDC)
{
	float d;
	int x = 0; int y = m_Radius;
	d = 1.25f - m_Radius;
	CirclePoints(x, y, pDC);
	while (x <= y)
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		CirclePoints(x, y, pDC);
	}
}

//Bresenham方法画圆
void CIRCLE::BresenhamCircle(CDC* pDC)
{
	int x, y, d;
	x = 0; y = m_Radius;
	d = 3 - 2 * m_Radius;
	while (x < y)
	{
		CirclePoints(x, y, pDC);
		if (d < 0)
			d = d + 4 * x + 6;
		else
		{
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	if (x == y)
		CirclePoints(x, y, pDC);
}

void CIRCLE::MidPointEllipse(CDC* pDC)
{
	int x, y; float d1, d2;
	x = 0; y = m_Ellipse_b;

	//上部分
	d1 = m_Ellipse_b * m_Ellipse_b + m_Ellipse_a * m_Ellipse_a * (-m_Ellipse_b + 0.25);
	EllipsePoints(x, y, pDC);
	while (m_Ellipse_b * m_Ellipse_b * (x + 1) < m_Ellipse_a * m_Ellipse_a * (y - 0.5))
	{
		if (d1 < 0)
		{
			d1 += m_Ellipse_b * m_Ellipse_b * (2 * x + 3); x++;
		}
		else 
		{
			d1 += (m_Ellipse_b * m_Ellipse_b * (2 * x + 3) + m_Ellipse_a * m_Ellipse_a * (-2 * y + 2));
			x++;
			y--;
		}
		EllipsePoints(x, y, pDC);
	}

	//下部分
	d2 = sqrt(m_Ellipse_b * (x + 0.5)) + sqrt(m_Ellipse_a * (y - 1)) - sqrt(m_Ellipse_a * m_Ellipse_b);
	while (y > 0)
	{
		if (d2 < 0) 
		{
			d2 += m_Ellipse_b * m_Ellipse_b * (2 * x + 2) + m_Ellipse_a * m_Ellipse_a * (-2 * y + 3);
			x++;
			y--;
		}
		else 
		{ 
			d2 += m_Ellipse_a * m_Ellipse_a * (-2 * y + 3); y--; 
		}
		EllipsePoints(x, y, pDC);
	}
}


////中点法画椭圆,整型运算，非浮点运算
//void CIRCLE::MidPointEllipse2(CDC* pDC)
//{
//	int x, y, d, xp, yp, squarea, squareb;
//	squarea = m_Ellipse_a * m_Ellipse_a;
//	squareb = m_Ellipse_b * m_Ellipse_b;
//	xp = (int)(0.5 + (float)squarea / sqrt((float)(squarea + squareb)));
//	yp = (int)(0.5 + (float)squareb / sqrt((float)(squarea + squareb)));
//	x = 0;
//	y = m_Ellipse_b;
//	d = 4 * (squareb - squarea * m_Ellipse_b) + squarea;
//	EllipsePoints(x, y, pDC);
//	while (x <= xp)
//	{
//		if (d <= 0)
//			d += 4 * squareb * (2 * x + 3);
//		else
//		{
//			d += 4 * squareb * (2 * x + 3) - 8 * squarea * (y - 1);
//			y--;
//		}
//		x++;
//		EllipsePoints(x, y, pDC);
//	}
//	x = m_Ellipse_a;
//	y = 0;
//	d = 4 * (squarea - squareb * m_Ellipse_a) + squareb;
//	EllipsePoints(x, y, pDC);
//	while (y < yp)
//	{
//		if (d <= 0)
//			d += 4 * squarea * (2 * y + 3);
//		else
//		{
//			d += 4 * squarea * (2 * y + 3) - 8 * squareb * (x - 1);
//			x--;
//		}
//		y++;
//		EllipsePoints(x, y, pDC);
//	}
//}
