#include "pch.h"
#include "LINE.h"

//���캯��
LINE::LINE(void)
{
	m_Begin = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_color = RGB(0, 0, 0);
}

//���ع��캯��
LINE::LINE(CPoint Begin, CPoint End, COLORREF color)
{
	m_Begin = Begin;
	m_End = End;
	m_color = color;
}

//DDA���߷�
void LINE::DDA(CDC* pDC)
{
	int dx = abs(m_End.x - m_Begin.x);
	int dy = abs(m_End.y - m_Begin.y);
	int s1, s2;
	if (m_End.x >= m_Begin.x) s1 = 1; else s1 = -1;
	if (m_End.y >= m_Begin.y) s2 = 1; else s2 = -1;
	float x = (float)m_Begin.x;
	float y = (float)m_Begin.y;
	float k = (float)dy / dx;
	if (k <= 1)
	{
		for (int i = 0; i <= dx; i++)
		{
			x += s1;
			pDC->SetPixel((int)x, (int)(y + 0.5), m_color);
			y += s2 * k;
		}
	}
	else
	{
		for (int i = 0; i <= dy; i++)
		{
			y += s2;
			pDC->SetPixel((int)(x + 0.5), (int)y, m_color);
			x += s1 * 1 / k;
		}
	}
}

//Bresenham�㷨1�����л��и���������
void LINE::Bresenham1(CDC* pDC)
{
	int dx = abs(m_End.x - m_Begin.x);
	int dy = abs(m_End.y - m_Begin.y);
	int s1, s2, interchange;
	if (m_End.x >= m_Begin.x) s1 = 1; else s1 = -1;
	if (m_End.y >= m_Begin.y) s2 = 1; else s2 = -1;
	if (dy > dx)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;
	}
	else interchange = 0;
	float e = -0.5f;
	float k = (float)dy / dx;
	int x = m_Begin.x; int y = m_Begin.y;
	for (int i = 0; i <= dx; i++)
	{
		pDC->SetPixel(x, y, m_color);
		if (e < 0)
		{
			if (interchange == 1) y += s2; else x += s1;
			e = e + k;
		}
		else if (e >= 0)
		{
			x += s1;
			y += s2;
			e = e + k - 1;
		}
	}
}


//Bresenham�㷨2�����㷨��ȫȥ���˸���������
void LINE::Bresenham2(CDC* pDC)
{
	int dx = abs(m_End.x - m_Begin.x);
	int dy = abs(m_End.y - m_Begin.y);
	int s1, s2, interchange;
	if (m_End.x >= m_Begin.x) s1 = 1; else s1 = -1;
	if (m_End.y >= m_Begin.y) s2 = 1; else s2 = -1;
	if (dy > dx)//xy����
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;
	}
	else interchange = 0;
	int f = -dx;
	int x = m_Begin.x; int y = m_Begin.y;
	for (int i = 0; i <= dx; i++)
	{
		pDC->SetPixel(x, y, m_color);
		if (f < 0)
		{
			if (interchange == 1) y += s2; else x += s1;
			f = f + 2 * dy;
		}
		else if (f >= 0)
		{
			x += s1;
			y += s2;
			f = f + 2 * dy - 2 * dx;
		}
	}
}

//Bresenham��������
void LINE::Bresenham3(CDC* pDC)
{
	int dx = abs(m_End.x - m_Begin.x);
	int dy = abs(m_End.y - m_Begin.y);
	int s1, s2, interchange;
	if (m_End.x >= m_Begin.x) s1 = 1; else s1 = -1;
	if (m_End.y >= m_Begin.y) s2 = 1; else s2 = -1;
	if (dy > dx)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;
	}
	else interchange = 0;
	int f = -dx;
	int x = m_Begin.x; int y = m_Begin.y;
	for (int i = 0; i <= dx; i++)
	{
		if (i % 4 == 0)pDC->SetPixel(x, y, m_color);
		if (f < 0)
		{
			if (interchange == 1) y += s2; else x += s1;
			f = f + 2 * dy;
		}
		if (f >= 0)
		{
			if (interchange == 1) x += s1; else y += s2;
			f = f - 2 * dx;
		}
	}
}

// �е㻭�߷�
void LINE::MidPointLine(CDC* pDC)
{
	int a = abs(m_End.y - m_Begin.y);
	int b = abs(m_End.x - m_Begin.x);
	int xs = 1; int ys = 1; int interchange;
	if (m_End.x < m_Begin.x) xs = -1;
	if (m_End.y < m_Begin.y) ys = -1;
	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
		interchange = 1;
	}
	else interchange = 0;
	int x = m_Begin.x;
	int y = m_Begin.y;
	int d = -2 * a + b;
	int delt1 = -2 * a;
	int delt2 = 2 * (-a + b);
	for (int i = 0; i < b; i++)
	{
		pDC->SetPixel(x, y, m_color);
		if (d < 0)
		{
			x += xs; y += ys;
			d += delt2;
		}
		else
		{
			if (interchange == 0) x += xs; else y += ys;
			d += delt1;
		}
	}
}