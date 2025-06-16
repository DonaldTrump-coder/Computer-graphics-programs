#include "pch.h"
#include "GMatrix.h"



GMatrix::GMatrix(void)
{
	m_ColNum = 0;
	m_RowNum = 0;
	m_Data = nullptr;
}
//���ܣ����캯������ʼ��Ԫ��Ϊ��ľ���
GMatrix::GMatrix(int nRowNum, int nColNum)
{
	if (nRowNum <= 0 || nColNum <= 0)
	{
		m_ColNum = 0;
		m_RowNum = 0;
		return;
	}
	m_Data = new double* [nRowNum];
	for (int i = 0; i < nRowNum; i++)
	{
		m_Data[i] = new double[nColNum];
	}
	for (int i = 0; i < nRowNum; i++)
		for (int j = 0; j < nColNum; j++)
		{
			m_Data[i][j] = 0.0;
		}
	m_ColNum = nColNum;
	m_RowNum = nRowNum;
	return;
}

//�������캯��
GMatrix::GMatrix(GMatrix& copy)
{
	m_ColNum = copy.m_ColNum;
	m_RowNum = copy.m_RowNum;
	m_Data = new double* [m_RowNum];
	for (int i = 0; i < m_RowNum; i++)
	{
		m_Data[i] = new double[m_ColNum];
		for (int j = 0; j < m_ColNum; j++)
		{
			m_Data[i][j] = copy.m_Data[i][j];
		}
	}
}

//��������
GMatrix::~GMatrix()
{
	if (m_Data != nullptr)
	{
		for (int i = 0; i < m_RowNum; i++)
		{
			delete[] m_Data[i];
		}
		delete[] m_Data;
		m_Data = nullptr;
	}
	m_RowNum = 0;
	m_ColNum = 0;
}

//=====================�����Ƕ�άͼ�α任����============================================
//#**************************************************************************************
//���ɱ����任����
GMatrix GMatrix::Translating(double Tx, double Ty, double Tz)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[3][0] = Tx; C.m_Data[3][1] = Ty; C.m_Data[3][2] = Tz;
	return C;
}
//������X����ת�任����
GMatrix GMatrix::RotatingX(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1;
	C.m_Data[1][1] = cos(Theta); C.m_Data[1][2] = sin(Theta);
	C.m_Data[2][1] = -sin(Theta); C.m_Data[2][2] = cos(Theta);
	C.m_Data[3][3] = 1;
	return C;
}
//������Y����ת�任����
GMatrix GMatrix::RotatingY(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = cos(Theta); C.m_Data[0][2] = -sin(Theta);
	C.m_Data[1][1] = 1;
	C.m_Data[2][0] = sin(Theta); C.m_Data[2][2] = cos(Theta);
	C.m_Data[3][3] = 1;
	return C;
}
//������Z����ת�任����
GMatrix GMatrix::RotatingZ(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = cos(Theta); C.m_Data[0][1] = sin(Theta);
	C.m_Data[1][0] = -sin(Theta); C.m_Data[1][1] = cos(Theta);
	C.m_Data[2][2] = 1;
	C.m_Data[3][3] = 1;
	return C;
}
//���ɱ����任����
GMatrix GMatrix::Scaling(double Sx, double Sy, double Sz)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = Sx; C.m_Data[1][1] = Sy; C.m_Data[2][2] = Sz; C.m_Data[3][3] = 1;
	return C;
}
//����ȫ�����任����
GMatrix GMatrix::Scaling(double s)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = s; C.m_Data[1][1] = s; C.m_Data[2][2] = s; C.m_Data[3][3] = 1;
	return C;
}
//����X��ĶԳƱ任����
GMatrix GMatrix::SymmetryInX()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = -1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//����Y��ĶԳƱ任����
GMatrix GMatrix::SymmetryInY()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = 1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//����Z��ĶԳƱ任����
GMatrix GMatrix::SymmetryInZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = -1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//����ԭ��ԳƱ任����
GMatrix GMatrix::SymmetryInO()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = -1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//����XY��ĶԳƱ任����
GMatrix GMatrix::SymmetryInXY()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//����XZ��ĶԳƱ任����
GMatrix GMatrix::SymmetryInXZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = -1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//����YZ��ĶԳƱ任����
GMatrix GMatrix::SymmetryInYZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//������x��y���б任����
GMatrix GMatrix::ShearXY(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[1][0] = d;
	return C;
}
//������x��z���б任����
GMatrix GMatrix::ShearXZ(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[2][0] = d;
	return C;
}
//������y��x���б任����
GMatrix GMatrix::ShearYX(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[0][1] = d;
	return C;
}
//������y��z���б任����
GMatrix GMatrix::ShearYZ(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[2][1] = d;
	return C;
}
//������z��x���б任����
GMatrix GMatrix::ShearZX(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[0][2] = d;
	return C;
}
//������z��y���б任����
GMatrix GMatrix::ShearZY(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[1][2] = d;
	return C;
}
//**************************************************************************************
//=====================�����Ƕ�άͼ�α任����============================================

//���ز�����=
GMatrix& GMatrix::operator=(const GMatrix& A)
{
	if (m_RowNum != A.m_RowNum || m_ColNum != A.m_ColNum)
	{
		if (m_Data != nullptr)
		{
			for (int i = 0; i < m_RowNum; i++)
			{
				delete[] m_Data[i];
			}
			delete[] m_Data;
			m_Data = nullptr;
		}
	}

	m_ColNum = A.m_ColNum;
	m_RowNum = A.m_RowNum;
	m_Data = new double* [m_RowNum];
	for (int i = 0; i < m_RowNum; i++)
	{
		m_Data[i] = new double[m_ColNum];
		for (int j = 0; j < m_ColNum; j++)
		{
			m_Data[i][j] = A.m_Data[i][j];
		}
	}

	return *this;
}

//���ܣ�����������˺��ĺ���
//���У�A�Ǵ�СΪm*s�ľ���B��s*n�ľ���C����˵õ���m*n�ľ���
GMatrix GMatrix::Mult(const GMatrix& A, const GMatrix& B)
{
	GMatrix C = GMatrix(A.m_RowNum, B.m_ColNum);
	
	for (int i = 0; i < A.m_RowNum; i++)
	{
		for (int j = 0; j < B.m_ColNum; j++)
		{
			C.m_Data[i][j] = 0.0;
			for (int k = 0; k < A.m_ColNum; k++)
				C.m_Data[i][j] += A.m_Data[i][k] * B.m_Data[k][j];
		}
	}
	return C;
}


//����任��MΪ�任����Points_inΪ����ĵ㣬Points_outΪ����ı任��ľ���
void GMatrix::PointsConvert(const GMatrix& M, vector<CPoint>& Points_in, vector<CPoint>& Points_out)
{
	if (Points_out.size() != Points_in.size())
	{
		Points_out.resize(Points_in.size());
	}

	for (int i = 0; i < Points_in.size(); i++)
	{
		GMatrix v(1, 4);
		v.m_Data[0][0] = Points_in[i].x; v.m_Data[0][1] = Points_in[i].y; v.m_Data[0][2] = 0; v.m_Data[0][3] = 1;
		GMatrix v1 = Mult(v, M);
		Points_out[i].x = (int)v1.m_Data[0][0];
		Points_out[i].y = (int)v1.m_Data[0][1];
	}
	return;
}


//����εľ���任
void GMatrix::PolygonConvert(const GMatrix& M, POLYGON& polygon_in, POLYGON& polygon_out)
{
	polygon_out = POLYGON();
	polygon_out.m_fill_color = polygon_in.m_fill_color;
	polygon_out.m_old_color = polygon_in.m_old_color;
	polygon_out.m_NodeNum = polygon_in.m_NodeNum;
	polygon_out.m_Nodes = new CPoint[polygon_in.m_NodeNum];

	for (int i = 0; i < polygon_in.m_NodeNum; i++)
	{
		GMatrix v(1, 4);
		v.m_Data[0][0] = polygon_in.m_Nodes[i].x; v.m_Data[0][1] = polygon_in.m_Nodes[i].y; v.m_Data[0][2] = 0; v.m_Data[0][3] = 1;
		GMatrix v1 = Mult(v, M);
		polygon_out.m_Nodes[i].x = (int)v1.m_Data[0][0];
		polygon_out.m_Nodes[i].y = (int)v1.m_Data[0][1];
	}
	return;
}