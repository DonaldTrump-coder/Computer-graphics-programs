#pragma once
#include <stack>

#include "Line.h"

using namespace std;

struct Edge
{
	double Xi = 0;
	double dX = 0;// ����ɨ���ߵ�����
	int Ymax = 0;
	Edge* Next = nullptr;
};


class POLYGON
{
public:
	CPoint* m_Nodes; //���� (����Ϊ������)
	int m_NodeNum = 0; //������
	COLORREF m_old_color; //���ǰ����ε���ɫ
	COLORREF m_fill_color; //�����ɫ
public:
	POLYGON(); //���캯��
	POLYGON(POLYGON& copy); //�������캯��
	~POLYGON(); //��������
	void AddNode(CPoint& p);
	void ScanLineFill(CDC* pDC);//ɨ���߶��������㷨
	void ScanLineSeedFill(CDC* pDC, CPoint seed);// < ɨ������������㷨 >
	void SeedFill4(CDC* pDC, CPoint initial_seed); //��������㷨��������̫�󣬷������������
	void SeedFill4withStack(CDC* pDC, CPoint initial_seed);
	void DrawPolygon(CDC* pDC, COLORREF color); //���ƶ����
private:
	int MinInY(); //�ҵ������е�y����Сֵ
	int MaxInY(); //�ҵ������е�y�����ֵ
	void insertEdge(Edge* edge, Edge* ActiveHead);
	void makeEdgeTable(CPoint Lower, CPoint Upper, Edge** ET);
	void CreatEdgeTable(Edge** ET);
	void fillScan(CDC* pDC, int y, Edge* active);//���һ�Խ���
	void HorizonEdgeFill(CDC* pDC);//ˮƽ��ֱ�ӻ������
	void RefreshActiveList(int scan, Edge** ET, Edge* active);//�����󣬸��»�߱�
	int FillLine(CDC* pDC, int Direct, CPoint seedPoint);
	void SearchLineNewSeed(CDC* pDC, stack<CPoint>& stk, int xLeft, int xRight, int y);
};

