
// GraphView.h: CGraphView 类的接口
//

#pragma once
#include "Line.h"
#include "Circle.h"
#include "Polygon.h"
#include "GMatrix.h"

//图形绘制模式标识的枚举
enum GRAPHMODE
{
	NoMode, //不进行任何图形的绘制 0
	Line_DDA, //DDA法画直线 1
	Line_Mid, //中点法画直线 2
	Line_Bresenham1, //Bresenham算法1画直线 3
	Line_Bresenham2, //Bresenham算法2画直线 4
	Line_Bresenham3, //Bresenham算法画虚线 5
	Circle_Mid, //中点法画圆 6
	Circle_Bresenham, //Bresenham法画圆 7
	Ellipse_Mid, //中点法画椭圆 8
	Draw_Polygon, //绘制多边形 9
	PolygonFill_Scan, //扫描线填充多边形 10
	PolygonFill_Seed, //种子填充多边形 11
	PolygonFill_ScanSeed, //扫描线种子填充多边形 12
	PolygonFill_CleanFill, //清除填充 13
	ImgTrans_Translate, //图形平移 14
	ImgTrans_Symmetry_X, //图形关于X轴对称 15
	ImgTrans_Symmetry_Y, //图形关于Y轴对称 16
	ImgTrans_Symmetry_O, //图形关于原点对称 17
	ImgTrans_Scale_Larger, //图形放大 18
	ImgTrans_Scale_Smaller, //图形缩小 19
	ImgTrans_Rotate_X, //图形绕X轴旋转 20
	ImgTrans_Rotate_Y, //图形绕Y轴旋转 21
	ImgTrans_Rotate_Z, //图形绕Z轴旋转 22
	ImgTrans_CleanTrans, //清除变换 23
};


class CGraphView : public CView
{
protected: // 仅从序列化创建
	CGraphView() noexcept;
	DECLARE_DYNCREATE(CGraphView)

// 特性
public:
	CGraphDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	//test1添加
	//bool m_isAutomaticRedrawing = true;
	bool m_isNeedInvalidate = false; //是否需要强制重绘
	GRAPHMODE m_GraphMode = NoMode; //图形绘制模式标识
	//客户区大小
	double m_width;
	double m_height;
	CBitmap m_bitmap; //保存客户区绘制内容的位图，用窗口大小改变时的重绘

	COLORREF m_color = RGB(0, 0, 0); //图形颜色,默认黑色
	bool m_isStop = true; //判断绘制是否停止
	CPoint m_StartPoint = CPoint(0, 0); //起点
	CPoint m_MovePoint = CPoint(0, 0); //移动的点
	CPoint m_EndPoint = CPoint(0, 0);; //终点

	LINE m_line; //直线

	//test2添加
	CIRCLE m_circle; //圆

	//test3添加
	bool m_IsDrawPolygonDone = false; //判断多边形是否绘制完成
	bool m_IsFillDone = false; //判断是否填充完成
	CPoint m_Polygon_Node; //上一个多边形顶点
	POLYGON m_polygon; //多边形
	COLORREF m_Polygon_color; //绘制多边形的颜色
	CPoint m_SeedPoint; //种子点

	//test4添加
	GRAPHMODE m_OldGraphMode1 = NoMode; //图形绘制模式标识
	GRAPHMODE m_OldGraphMode2 = NoMode; //图形绘制模式标识
	vector<CPoint> m_GraphPoints; //图形点坐标
	vector<COLORREF> m_GraphPointsColors; //图形点颜色
	CBitmap m_save_bitmap;
	CButton m_startButton, m_endButton; //开始和暂停的按钮
	double m_theta = 0; //旋转角度
	bool m_isStartRotate = false; //是否开始旋转
	int m_flush_num = 0; //为解决图形旋转带来的一个未知bug而设置的变量

private:
	void SaveGraph2Bmp(CDC* pDC, CBitmap& bitmap); //保存客户区绘制的内容到位图中
	void DrawGraphFromBmp(CDC* pDC, CBitmap& bitmap); //将位图重新绘制到客户区
	void DrawGraph(CDC* pDC, CPoint P0, CPoint P1, COLORREF color, GRAPHMODE GraphMode); //绘制图形方法的统一设计

	//test3添加
	void DrawOldPolygon(CDC* pDC, GRAPHMODE GraphMode, COLORREF color); //把之前已经绘制过的多边形的边再重新绘制一遍
	void CleanOldGraph(CDC* pDC); //清除之前绘制的图形

	//test4添加
	void GetGraphPointsFromBmp(CBitmap& bitmap); //从位图中获得图形的点坐标，默认背景色像素值是RGB(255,255,255)
	void DrawGraphPoints(CDC* pDC, vector<CPoint>& GraphPoints, vector<COLORREF>& GraphPointsColors); //根据点的坐标及颜色来绘制图形
	void ShowAutoCloseMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, UINT uTimeout); //生成自动关闭的弹窗
	void DrawXOY(CDC* pDC, COLORREF color); //绘制XY轴
	void Draw_XOY_And_Button_2_Bmp(CButton& button, int x, int y, CBitmap& bitmap); //绘制坐标轴和按钮到位图上，其中x，y是按钮的左上角坐标

public:
	//test1添加
	//菜单选项的响应函数
	afx_msg void OnSize(UINT nType, int cx, int cy); //窗口大小改变时的响应函数

	afx_msg void OnLineDda();
	afx_msg void OnLineMid();
	afx_msg void OnLineBresenham1();
	afx_msg void OnLineBresenham2();
	afx_msg void OnLineBresenham3();
	afx_msg void OnColor();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //鼠标左键点击的响应函数
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  //鼠标左键松开的响应函数
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);  //鼠标移动的响应函数

	//test2添加
	afx_msg void OnCircleMid();
	afx_msg void OnCircleBresenham();
	afx_msg void OnEllipseMid();

	//test3添加
	afx_msg void OnPfDrawPolygon();
	afx_msg void OnPfScan();
	afx_msg void OnPfSeed();
	afx_msg void OnPfScanSeed();
	afx_msg void OnPfCleanfill();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	//test4添加
	afx_msg void OnImgtransTranslate();
	afx_msg void OnImgtransSymmetryX();
	afx_msg void OnImgtransSymmetryY();
	afx_msg void OnImgtransSymmetryO();
	afx_msg void OnImgtransScaleLarger();
	afx_msg void OnImgtransScaleSmaller();
	afx_msg void OnImgtransRotateX();
	afx_msg void OnImgtransRotateY();
	afx_msg void OnImgtransRotateZ();
	afx_msg void OnImgtransCleantrans();
	afx_msg void OnButtonStart_Click();
	afx_msg void OnButtonEnd_Click();
	afx_msg void OnTimer(UINT_PTR nIDEvent); //计时器

};

#ifndef _DEBUG  // GraphView.cpp 中的调试版本
inline CGraphDoc* CGraphView::GetDocument() const
   { return reinterpret_cast<CGraphDoc*>(m_pDocument); }
#endif

