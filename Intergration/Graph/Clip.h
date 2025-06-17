#pragma once
#include "Line.h"
#include "Polygon.h"


//������ֱ�߲ü�����㷨
class LINE_CLIP : public LINE
{
public:
	LINE_CLIP(); //���캯��
	LINE_CLIP(CPoint Begin, CPoint End, COLORREF color); //���ع��캯��

private:
	//Sothland�㷨��غ���
	int Encode(CPoint p, int XL, int XR, int YB, int YT);
	//�е��㷨��غ���
	CPoint MidClipPoint(int XL, int XR, int YB, int YT);
	bool NotIntersect(int XL, int XR, int YB, int YT);
	bool IsInArea(CPoint P, int XL, int XR, int YB, int YT);
	//���ж��㷨��غ���
	bool ClipT(int p, int q, float& u1, float& u2);
public:
	//ֱ�߲ü��㷨����ֱ����ȫ�ڴ����ڻ��������ڴ����⣬�򷵻�false�����򷵻�true
	//Sothland�㷨��غ���
	bool LineClip(int XL, int XR, int YB, int YT, LINE& line);
	//�е��㷨��غ���
	bool LineMidClip(int XL, int XR, int YB, int YT, LINE& L);
	//���ж��㷨��غ���
	bool LBClip(int XL, int XR, int YB, int YT, LINE& line);
};


//�����Ƕ���βü��㷨
class POLYGON_CLIP : public POLYGON
{
public:
	POLYGON_CLIP(); //���캯��
	POLYGON_CLIP(POLYGON_CLIP& copy); //�������캯��
	~POLYGON_CLIP(); //��������
private:
	bool isVisible(CPoint& P, LINE& ClipEdge);
	CPoint Intersect(LINE& PolygonEdge, LINE& ClipEdge);
	void SingleEdgeClip(LINE& ClipEdge, POLYGON_CLIP& NewPolygon);
	void SHCheck(int XL, int XR, int YB, int YT);
	void SHDrawPolygon(CDC* pDC, int XL, int XR, int YB, int YT);
	void SortInX();
	void SortInY();
	bool IsPolygonNeedClip(int XL, int XR, int YB, int YT); //�ж��Ƿ���Ҫ�ü�
public:
	void operator = (POLYGON_CLIP& A); //���������
	void SetPolygonClip(POLYGON& polygon); //���ݶ���ε�ֵ���ö���βü���ֵ
	bool Surtherland_Hondgman(int XL, int XR, int YB, int YT, POLYGON_CLIP& polygon_out);
};
