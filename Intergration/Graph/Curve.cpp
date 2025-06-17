#include "pch.h"
#include "CURVE.h"


//构造函数
CURVE::CURVE(void)
{
}

//重载构造函数
CURVE::CURVE(vector<CPoint>& CtrlPt)
{
	m_CtrlPt = CtrlPt;
}

//重载构造函数
CURVE::CURVE(POLYGON& polygon)
{
	m_CtrlPt.resize(polygon.m_NodeNum);
	for (int i = 0; i < polygon.m_NodeNum; i++)
	{
		m_CtrlPt[i] = polygon.m_Nodes[i];
	}
}

//计算组合系数Cn^k
int  CURVE::binomialCoeffs(int k, int n)
{
	int i, c = 1;
	if (k == 0)
		return c;
	else
	{
		for (i = n - k + 1; i <= n; i++)
			c *= i;
		for (i = 1; i <= k; i++)
			c /= i;
		return c;
	}
}

//计算Bezier曲线上点的坐标
void CURVE::computeBezierPt(float t, CPoint& bezierPt)
{
	//int k=0;
	float bernstein;
	//bezierPt = gcnew GPoint();
	bezierPt.x = 0;
	bezierPt.y = 0;
	for (int i = 0; i < m_CtrlPt.size(); i++)
	{
		int nOrder = m_CtrlPt.size() - 1;// 阶数
		bernstein = binomialCoeffs(i, nOrder) * pow(t, i) * pow(1 - t, nOrder - i);
		bezierPt.x += int(m_CtrlPt[i].x * bernstein);
		bezierPt.y += int(m_CtrlPt[i].y * bernstein);
	}
}

//根据控制点，求曲线上的nBezPts个点
void CURVE::Bezier(CDC* pDC, int nBezPts)
{
	CPoint bezierPt;
	float u;
	int k = nBezPts;
	for (k = 0; k <= nBezPts; k++)
	{
		u = float(k) / float(nBezPts);
		computeBezierPt(u, bezierPt);
		pDC->SetPixel(bezierPt.x, bezierPt.y, m_color);
	}
}

//根据控制点，求曲线上的nBezPts个点(递归算法）
void CURVE::DrawBzier(CDC* pDC)
{
	if (m_CtrlPt.size() <= 0)
		return;
	int Xn = m_CtrlPt[m_CtrlPt.size() - 1].x;
	int X0 = m_CtrlPt[0].x;
	int Yn = m_CtrlPt[m_CtrlPt.size() - 1].y;
	int Y0 = m_CtrlPt[0].y;

	if ((Xn < X0 + 1) && (Xn > X0 - 1) && (Yn < Y0 + 1) && (Yn > Y0 - 1))
	{
		pDC->SetPixel(m_CtrlPt[0].x, m_CtrlPt[0].y, m_color);
		return;
	}
	vector<CPoint> p1(m_CtrlPt.size());
	p1[0] = m_CtrlPt[0];
	for (int i = 0; i < m_CtrlPt.size(); i++)
	{
		for (int j = 0; j < m_CtrlPt.size() - i; j++)
		{
			m_CtrlPt[j].x = (m_CtrlPt[j].x + m_CtrlPt[j].x) / 2;
			m_CtrlPt[j].y = (m_CtrlPt[j].y + m_CtrlPt[j].y) / 2;
		}
		p1[i] = m_CtrlPt[0];
	}
	CURVE CP1(p1);
	DrawBzier(pDC);
	CP1.DrawBzier(pDC);
}