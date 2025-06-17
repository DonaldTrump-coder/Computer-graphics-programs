#pragma once
class LINE
{
public:
	CPoint m_Begin = CPoint(0, 0); //���
	CPoint m_End = CPoint(0, 0); //�յ�
	COLORREF m_color = RGB(0, 0, 0); //������ɫ
public:
	LINE(void); //���캯��
	LINE(CPoint Begin, CPoint End, COLORREF color); //���ع��캯��

	void DDA(CDC* pDC); //DDA���߷�
	void Bresenham1(CDC* pDC); //Bresenham�㷨1�����л��и���������
	void Bresenham2(CDC* pDC); //Bresenham�㷨2�����㷨��ȫȥ���˸���������
	void Bresenham3(CDC* pDC); //Bresenham��������
	void MidPointLine(CDC* pDC); // �е㻭�߷�
};

