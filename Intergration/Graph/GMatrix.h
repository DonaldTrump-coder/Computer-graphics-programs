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
	GMatrix(void); //构造函数
	GMatrix(int nRowNum, int nColNum); //重载构造函数
	GMatrix(GMatrix& copy); //拷贝构造函数
	~GMatrix(); //析构函数

	GMatrix Translating(double Tx, double Ty, double Tz); //生成比例变换矩阵
	GMatrix RotatingX(double Theta); //生成绕X轴旋转变换矩阵
	GMatrix RotatingY(double Theta); //生成绕Y轴旋转变换矩阵
	GMatrix RotatingZ(double Theta); //生成绕Z轴旋转变换矩阵
	GMatrix Scaling(double Sx, double Sy, double Sz); //生成比例变换矩阵
	GMatrix Scaling(double s); //生成全比例变换矩阵
	GMatrix SymmetryInX(); //生成X轴的对称变换矩阵
	GMatrix SymmetryInY(); //生成Y轴的对称变换矩阵
	GMatrix SymmetryInZ(); //生成Z轴的对称变换矩阵
	GMatrix SymmetryInO(); //生成原点对称变换矩阵
	GMatrix SymmetryInXY(); //生成XY面的对称变换矩阵
	GMatrix SymmetryInXZ(); //生成XZ面的对称变换矩阵
	GMatrix SymmetryInYZ(); //生成YZ面的对称变换矩阵
	GMatrix ShearXY(double d); //生成沿x含y错切变换矩阵
	GMatrix ShearXZ(double d); //生成沿x含z错切变换矩阵
	GMatrix ShearYX(double d); //生成沿y含x错切变换矩阵
	GMatrix ShearYZ(double d); //生成沿y含z错切变换矩阵
	GMatrix ShearZX(double d); //生成沿z含x错切变换矩阵
	GMatrix ShearZY(double d); //生成沿z含y错切变换矩阵

	GMatrix& operator = (const GMatrix& A); //重载操作符=

	//功能：两个矩阵相乘核心函数
	//其中，A是大小为m*s的矩阵，B是s*n的矩阵，C是相乘得到的m*n的矩阵
	GMatrix Mult(const GMatrix& A, const GMatrix& B);

	//矩阵变换，M为变换矩阵，Points_in为输入的点，Points_out为输出的变换后的矩阵
	void PointsConvert(const GMatrix& M, vector<CPoint>& Points_in, vector<CPoint>& Points_out);

	//多边形的矩阵变换
	void PolygonConvert(const GMatrix& M, POLYGON& polygon_in, POLYGON& polygon_out);


	
		

};

