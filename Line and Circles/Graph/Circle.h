#pragma once
class CIRCLE
{
private:
	CPoint m_CentralPoint; //Բ����Բ�����ĵ�
	int m_Radius; //Բ�İ뾶
	int m_Ellipse_a; //��Բ�ĳ�����
	int m_Ellipse_b; //��Բ�Ķ̰���
	COLORREF m_color; //��Բ��ɫ

public:
	CIRCLE(); //���캯��
	CIRCLE(CPoint CentralPoint, int Radius, COLORREF color); //���ع��캯��
	CIRCLE(CPoint CentralPoint, int Ellipse_a, int Ellipse_b, COLORREF color); //���ع��캯��
private:
	void CirclePoints(int x, int y, CDC* pDC); //��Բ���ߵĵ�
	void EllipsePoints(int x, int y, CDC* pDC); //����Բ���ߵĵ�
public:
	void MidPointCircle(CDC* pDC); //�е㷨��Բ
	void BresenhamCircle(CDC* pDC); //Bresenham����Բ
	void MidPointEllipse(CDC* pDC); //�е㷨����Բ
	//void MidPointEllipse2(CDC* pDC); //�е㷨����Բ,�������㣬�Ǹ�������
};

