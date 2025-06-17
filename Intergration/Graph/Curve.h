#pragma once
#include<vector>
#include "Polygon.h"

class CURVE
{
public:
	vector<CPoint> m_CtrlPt;//���Ƶ����� (����Ϊ������)
	COLORREF m_color; //�������ߵ���ɫ
	//int nCtrlPt;//���Ƶ���
public:
	CURVE(void); //���캯��
	CURVE(vector<CPoint>& CtrlPt); //���ع��캯��
	CURVE(POLYGON& polygon); //���ع��캯��
public:
	void DrawBzier(CDC* pDC); //���ݿ��Ƶ㣬�������ϵ�nBezPts����
	void Bezier(CDC* pDC, int nBezPts);
private:
	int binomialCoeffs(int k, int n); //�������ϵ��Cn^k
	void computeBezierPt(float t, CPoint& bezierPt); //����Bezier�����ϵ������
};

