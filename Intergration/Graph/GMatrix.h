#pragma once
#include "math.h"
#include <vector>
#include "Polygon.h"

using namespace std;


class GMatrix
{
public:
	int m_ColNum;
	int m_RowNum;
	double** m_Data;

public:
	GMatrix(void); //���캯��
	GMatrix(int nRowNum, int nColNum); //���ع��캯��
	GMatrix(GMatrix& copy); //�������캯��
	~GMatrix(); //��������

	GMatrix Translating(double Tx, double Ty, double Tz); //���ɱ����任����
	GMatrix RotatingX(double Theta); //������X����ת�任����
	GMatrix RotatingY(double Theta); //������Y����ת�任����
	GMatrix RotatingZ(double Theta); //������Z����ת�任����
	GMatrix Scaling(double Sx, double Sy, double Sz); //���ɱ����任����
	GMatrix Scaling(double s); //����ȫ�����任����
	GMatrix SymmetryInX(); //����X��ĶԳƱ任����
	GMatrix SymmetryInY(); //����Y��ĶԳƱ任����
	GMatrix SymmetryInZ(); //����Z��ĶԳƱ任����
	GMatrix SymmetryInO(); //����ԭ��ԳƱ任����
	GMatrix SymmetryInXY(); //����XY��ĶԳƱ任����
	GMatrix SymmetryInXZ(); //����XZ��ĶԳƱ任����
	GMatrix SymmetryInYZ(); //����YZ��ĶԳƱ任����
	GMatrix ShearXY(double d); //������x��y���б任����
	GMatrix ShearXZ(double d); //������x��z���б任����
	GMatrix ShearYX(double d); //������y��x���б任����
	GMatrix ShearYZ(double d); //������y��z���б任����
	GMatrix ShearZX(double d); //������z��x���б任����
	GMatrix ShearZY(double d); //������z��y���б任����

	GMatrix& operator = (const GMatrix& A); //���ز�����=

	//���ܣ�����������˺��ĺ���
	//���У�A�Ǵ�СΪm*s�ľ���B��s*n�ľ���C����˵õ���m*n�ľ���
	GMatrix Mult(const GMatrix& A, const GMatrix& B);

	//����任��MΪ�任����Points_inΪ����ĵ㣬Points_outΪ����ı任��ľ���
	void PointsConvert(const GMatrix& M, vector<CPoint>& Points_in, vector<CPoint>& Points_out);

	//����εľ���任
	void PolygonConvert(const GMatrix& M, POLYGON& polygon_in, POLYGON& polygon_out);


	
		

};

