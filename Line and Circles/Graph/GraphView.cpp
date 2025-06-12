
// GraphView.cpp: CGraphView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Graph.h"
#endif

#include "GraphDoc.h"
#include "GraphView.h"

#include <chrono>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

long long gettime()
{
	auto now=std::chrono::system_clock::now();
	auto duration=now.time_since_epoch();
	auto mili = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
	return mili.count();
}

// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CView)

BEGIN_MESSAGE_MAP(CGraphView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	//test1添加
	ON_COMMAND(ID_LINE_BRESENHAM1, &CGraphView::OnLineBresenham1)
	ON_COMMAND(ID_LINE_BRESENHAM2, &CGraphView::OnLineBresenham2)
	ON_COMMAND(ID_LINE_BRESENHAM3, &CGraphView::OnLineBresenham3)
	ON_COMMAND(ID_LINE_DDA, &CGraphView::OnLineDda)
	ON_COMMAND(ID_LINE_MID, &CGraphView::OnLineMid)
	ON_COMMAND(ID_COLOR, &CGraphView::OnColor)
	ON_COMMAND(ID_COLOR, &CGraphView::OnColor)

	//test1添加
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE() //对应OnSize函数，可通过类向导中的 “消息” 添加

	//test2添加
	ON_COMMAND(ID_CIRCLE_MID, &CGraphView::OnCircleMid)
	ON_COMMAND(ID_CIRCLE_BRESENHAM, &CGraphView::OnCircleBresenham)
	ON_COMMAND(ID_ELLIPSE_MID, &CGraphView::OnEllipseMid)


END_MESSAGE_MAP()

// CGraphView 构造/析构

CGraphView::CGraphView() noexcept
{
	// TODO: 在此处添加构造代码

}

CGraphView::~CGraphView()
{
	//test1添加
	m_bitmap.DeleteObject();
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphView 绘图

void CGraphView::OnDraw(CDC* pDC)
{
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	//OnDraw函数在系统重绘时，会自动调用该函数，有两种情况会系统会选择重绘
	//一种是当窗口位置发生移动或者大小发生改变等，系统自动重绘，自动调用该函数
	//一种是我们使用Invalidate函数使得系统强制重绘，也会调用该函数
	//所以使用成员变量m_isNeedInvalidate来判断是系统自动重绘还是我们想让系统强制重绘
	// 当系统强制重绘时，调用绘制图形的函数进行绘制，并把绘制的内容保存到位图中
	// 当系统自动重绘时，把保存的位图绘制到客户区中，以实现当窗口大小改变以及位置移动时已绘制的图形保持不变
	if (m_isNeedInvalidate == true) //如果是系统强制重绘
	{
		//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
		if (m_GraphMode >= 1 && m_GraphMode <= 8)
		{
			DrawGraphFromBmp(pDC, m_bitmap);
			if (m_isStop == false)
			{
				//绘制图形,显示绘制轨迹
				DrawGraph(pDC, m_StartPoint, m_MovePoint, m_color, m_GraphMode);
			}
			else if (m_isStop == true)
			{
				//绘制图形的最终结果
				DrawGraph(pDC, m_StartPoint, m_EndPoint, m_color, m_GraphMode);
				
				//DrawGraph(pDC, CPoint(0, 0), CPoint(10, 10), m_color, m_GraphMode);
				
				SaveGraph2Bmp(pDC, m_bitmap);
			}
		}

		// 绘制结束后将客户区内容复制到位图中
		// 以实现系统自动重绘时，将保存在的内容重新绘制出来
		//SaveGraph2Bmp(pDC, m_bitmap);

		m_isNeedInvalidate = false;
	}
	else if(m_isNeedInvalidate == false) //如果是系统自动重绘
	{
		if (m_bitmap.GetSafeHandle() != NULL)
		{
			//把保存的位图绘制到客户区上
			DrawGraphFromBmp(pDC, m_bitmap);
		}
	}
}


// CGraphView 打印

BOOL CGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGraphView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGraphView 诊断

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphDoc* CGraphView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
	return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphView 消息处理程序

//test1添加
//绘制图形方法的统一设计
void CGraphView::DrawGraph(CDC* pDC, CPoint P0, CPoint P1, COLORREF color, GRAPHMODE GraphMode)
{
	switch (GraphMode)
	{
	case(NoMode):
	{
		return;
		break;
	}
	case(Line_DDA):
	{
		m_line = LINE(P0, P1, color);
		m_line.DDA(pDC);
		break;
	}
	case(Line_Mid):
	{
		m_line = LINE(P0, P1, color);
		m_line.MidPointLine(pDC);
		break;
	}
	case(Line_Bresenham1):
	{
		m_line = LINE(P0, P1, color);
		m_line.Bresenham1(pDC);
		break;
	}
	case(Line_Bresenham2):
	{
		m_line = LINE(P0, P1, color);
		m_line.Bresenham2(pDC);
		break;
	}
	case(Line_Bresenham3):
	{
		m_line = LINE(P0, P1, color);
		m_line.Bresenham3(pDC);
		break;
	}
	case(Circle_Mid):
	{
		double r = sqrt((P1.x - P0.x) * (P1.x - P0.x) + (P1.y - P0.y) * (P1.y - P0.y));
		m_circle = CIRCLE(P0, r, color);
		m_circle.MidPointCircle(pDC);
		break;
	}
	case(Circle_Bresenham):
	{
		double r = sqrt((P1.x - P0.x) * (P1.x - P0.x) + (P1.y - P0.y) * (P1.y - P0.y));
		m_circle = CIRCLE(P0, r, color);
		m_circle.BresenhamCircle(pDC);
		break;
	}
	case(Ellipse_Mid):
	{
		int a = abs(P0.x - P1.x);
		int b = abs(P0.y - P1.y);
		m_circle = CIRCLE(P0, a, b, color);
		m_circle.MidPointEllipse(pDC);
		break;
	}
	}
}


//test1添加
//保存客户区绘制的内容到位图中
void CGraphView::SaveGraph2Bmp(CDC* pDC, CBitmap& bitmap)
{
	//如果CBitmap对象未被创建或者创建的CBitmap对象大小未更新
	if (bitmap.GetSafeHandle() == NULL) //如果CBitmap对象未被创建
	{
		//创建一个 CBitmap 对象，大小与绘图区域相同，用于保存客户区绘制的内容
		bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	}
	else
	{
		BITMAP bmpInfo = { 0 };
		bitmap.GetBitmap(&bmpInfo);
		if (bmpInfo.bmWidth != m_width || bmpInfo.bmHeight != m_height)
		{
			bitmap.DeleteObject();
			//创建一个 CBitmap 对象，大小与绘图区域相同，用于保存客户区绘制的内容
			bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
		}
	}

	//创建一个临时的内存设备上下文对象，用于保存绘制的内容
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	//保存客户区绘制的内容到位图中
	memDC.BitBlt(0, 0, m_width, m_height, pDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	// 释放内存设备上下文
	memDC.DeleteDC();
}

//test1添加
//将位图重新绘制到客户区
void CGraphView::DrawGraphFromBmp(CDC* pDC, CBitmap& bitmap)
{
	//创建一个临时的内存设备上下文对象，用于保存绘制的内容
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// 然后再将内存设备上下文对象复制到设备上下文中
	pDC->BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	// 释放内存设备上下文
	memDC.DeleteDC();
}

//test1添加
//窗口大小改变时的响应函数
void CGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	m_height = rect.Height();
	m_width = rect.Width();
}


//test1添加
//鼠标左键点击的响应函数
void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//如果没有选择绘制图形，则鼠标左键点击不进行任何操作
	if (m_GraphMode == NoMode)
	{
		return;
	}

	//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
	if (m_GraphMode >= 1 && m_GraphMode <= 8)
	{
		//获得鼠标当前位置，称为起点m_StartPoint
		m_StartPoint = point;
		// 将变量m_isStop设为false，指示绘制已经开始
		m_isStop = false;
	}

	CView::OnLButtonDown(nFlags, point);
}

//test1添加
//鼠标移动的响应函数
void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 如果绘制没有开始，则不做任何操作
	if (m_isStop == true)
	{
		return;
	}

	// 如果绘制已经开始
	//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
	if (m_GraphMode >= 1 && m_GraphMode <= 8)
	{
		if (nFlags & MK_LBUTTON) //只有鼠标移动的同时鼠标左键按下才会执行
		{
			// 获取当前鼠标的位置，称为移动点m_MovePoint
			m_MovePoint = point;
			// 当m_isNeedInvalidate设为true后,
			// 再使用Invalidate(TRUE)命令后，工作区就会被清空，然后MFC会自动调用OnDraw函数，执行强制重绘
			// 而使用Invalidate(FALSE)命令不会清空工作区，会在工作区已有基础上继续绘制新的内容
			m_isNeedInvalidate = true;
			Invalidate(TRUE); //强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
		}
	}

	CView::OnMouseMove(nFlags, point);
}


//test1添加
//鼠标左键松开的响应函数
void CGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 如果绘制没有开始，则不做任何操作
	if (m_isStop == true)
	{
		return;
	}

	long long dura;
	// 如果绘制已经开始
	//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
	if (m_GraphMode >= 1 && m_GraphMode <= 8)
	{
		// 获取鼠标当前位置，称为终点m_EndPoint
		m_EndPoint = point;
		// 将m_isStop设为true，指示绘制已经停止
		m_isStop = true;
		// 强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容，False表示不擦除之前绘制的内容
		m_isNeedInvalidate = true;

		long long time1 = gettime();
		Invalidate(TRUE);
		long long time2 = gettime();
		dura = time2 - time1;
	}

	CView::OnLButtonUp(nFlags, point);
}

//test1添加
// 菜单栏“DDA算法”选项对应的响应函数
void CGraphView::OnLineDda()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行DDA算法直线绘制
	m_GraphMode = Line_DDA;
}

//test1添加
// 菜单栏“中点算法”选项对应的响应函数
void CGraphView::OnLineMid()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行中点算法直线绘制
	m_GraphMode = Line_Mid;
}

//test1添加
// 菜单栏“Bresenham算法1”选项对应的响应函数
void CGraphView::OnLineBresenham1()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行Line_Bresenham1算法直线绘制
	m_GraphMode = Line_Bresenham1;
}

//test1添加
// 菜单栏“Bresenham算法2”选项对应的响应函数
void CGraphView::OnLineBresenham2()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行Line_Bresenham2算法直线绘制
	m_GraphMode = Line_Bresenham2;
}

//test1添加
// 菜单栏“Bresenham算法3”选项对应的响应函数
void CGraphView::OnLineBresenham3()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行Line_Bresenham3算法直线绘制
	m_GraphMode = Line_Bresenham3;
}


// 菜单栏“颜色”选项对应的响应函数
void CGraphView::OnColor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dig;	//构造一个颜色选择对话框

	dig.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT; // CC_RGBINIT可以让上次选择的颜色作为初始颜色显示出来
	dig.m_cc.rgbResult = m_color;	//保存前一个选择的色素

	if (IDOK == dig.DoModal())	//判断是否选中了色素(按下OK按钮)
	{
		m_color = dig.m_cc.rgbResult;	//获取当前在颜色对话框中选择的色素
	}
}


//test2添加
// 菜单栏“中点法画圆”选项对应的响应函数
void CGraphView::OnCircleMid()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行中点法画圆
	m_GraphMode = Circle_Mid;
}

//test2添加
// 菜单栏“Bresenham法画圆”选项对应的响应函数
void CGraphView::OnCircleBresenham()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行Bresenham法画圆
	m_GraphMode = Circle_Bresenham;
}

//test2添加
// 菜单栏“中点法画椭圆”选项对应的响应函数
void CGraphView::OnEllipseMid()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行中点法画椭圆
	m_GraphMode = Ellipse_Mid;
}