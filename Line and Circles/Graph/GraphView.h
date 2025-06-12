
// GraphView.h: CGraphView 类的接口
//

#pragma once
#include "Line.h"
#include "Circle.h"

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
private:
	void SaveGraph2Bmp(CDC* pDC, CBitmap& bitmap); //保存客户区绘制的内容到位图中
	void DrawGraphFromBmp(CDC* pDC, CBitmap& bitmap); //将位图重新绘制到客户区
	void DrawGraph(CDC* pDC, CPoint P0, CPoint P1, COLORREF color, GRAPHMODE GraphMode); //绘制图形方法的统一设计
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
};

#ifndef _DEBUG  // GraphView.cpp 中的调试版本
inline CGraphDoc* CGraphView::GetDocument() const
   { return reinterpret_cast<CGraphDoc*>(m_pDocument); }
#endif

