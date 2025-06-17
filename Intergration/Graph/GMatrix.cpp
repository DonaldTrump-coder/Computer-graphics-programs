#include "pch.h"
#include "GMatrix.h"



GMatrix::GMatrix(void)
{
	m_ColNum = 0;
	m_RowNum = 0;
	m_Data = nullptr;
}
//功能：构造函数，初始化元素为零的矩阵
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

//拷贝构造函数
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

//析构函数
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

//=====================下面是二维图形变换矩阵============================================
//#**************************************************************************************
//生成比例变换矩阵
GMatrix GMatrix::Translating(double Tx, double Ty, double Tz)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[3][0] = Tx; C.m_Data[3][1] = Ty; C.m_Data[3][2] = Tz;
	return C;
}
//生成绕X轴旋转变换矩阵
GMatrix GMatrix::RotatingX(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1;
	C.m_Data[1][1] = cos(Theta); C.m_Data[1][2] = sin(Theta);
	C.m_Data[2][1] = -sin(Theta); C.m_Data[2][2] = cos(Theta);
	C.m_Data[3][3] = 1;
	return C;
}
//生成绕Y轴旋转变换矩阵
GMatrix GMatrix::RotatingY(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = cos(Theta); C.m_Data[0][2] = -sin(Theta);
	C.m_Data[1][1] = 1;
	C.m_Data[2][0] = sin(Theta); C.m_Data[2][2] = cos(Theta);
	C.m_Data[3][3] = 1;
	return C;
}
//生成绕Z轴旋转变换矩阵
GMatrix GMatrix::RotatingZ(double Theta)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = cos(Theta); C.m_Data[0][1] = sin(Theta);
	C.m_Data[1][0] = -sin(Theta); C.m_Data[1][1] = cos(Theta);
	C.m_Data[2][2] = 1;
	C.m_Data[3][3] = 1;
	return C;
}
//生成比例变换矩阵
GMatrix GMatrix::Scaling(double Sx, double Sy, double Sz)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = Sx; C.m_Data[1][1] = Sy; C.m_Data[2][2] = Sz; C.m_Data[3][3] = 1;
	return C;
}
//生成全比例变换矩阵
GMatrix GMatrix::Scaling(double s)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = s; C.m_Data[1][1] = s; C.m_Data[2][2] = s; C.m_Data[3][3] = 1;
	return C;
}
//生成X轴的对称变换矩阵
GMatrix GMatrix::SymmetryInX()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = -1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//生成Y轴的对称变换矩阵
GMatrix GMatrix::SymmetryInY()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = 1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//生成Z轴的对称变换矩阵
GMatrix GMatrix::SymmetryInZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = -1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//生成原点对称变换矩阵
GMatrix GMatrix::SymmetryInO()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = -1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//生成XY面的对称变换矩阵
GMatrix GMatrix::SymmetryInXY()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = -1; C.m_Data[3][3] = 1;
	return C;
}
//生成XZ面的对称变换矩阵
GMatrix GMatrix::SymmetryInXZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = -1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//生成YZ面的对称变换矩阵
GMatrix GMatrix::SymmetryInYZ()
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = -1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	return C;
}
//生成沿x含y错切变换矩阵
GMatrix GMatrix::ShearXY(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[1][0] = d;
	return C;
}
//生成沿x含z错切变换矩阵
GMatrix GMatrix::ShearXZ(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[2][0] = d;
	return C;
}
//生成沿y含x错切变换矩阵
GMatrix GMatrix::ShearYX(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[0][1] = d;
	return C;
}
//生成沿y含z错切变换矩阵
GMatrix GMatrix::ShearYZ(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[2][1] = d;
	return C;
}
//生成沿z含x错切变换矩阵
GMatrix GMatrix::ShearZX(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[0][2] = d;
	return C;
}
//生成沿z含y错切变换矩阵
GMatrix GMatrix::ShearZY(double d)
{
	GMatrix C = GMatrix(4, 4);
	C.m_Data[0][0] = 1; C.m_Data[1][1] = 1; C.m_Data[2][2] = 1; C.m_Data[3][3] = 1;
	C.m_Data[1][2] = d;
	return C;
}
//**************************************************************************************
//=====================上面是二维图形变换矩阵============================================

//重载操作符=
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

//功能：两个矩阵相乘核心函数
//其中，A是大小为m*s的矩阵，B是s*n的矩阵，C是相乘得到的m*n的矩阵
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


//矩阵变换，M为变换矩阵，Points_in为输入的点，Points_out为输出的变换后的矩阵
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


//多边形的矩阵变换
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