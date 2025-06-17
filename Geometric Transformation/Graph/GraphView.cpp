
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

	//test3添加
	ON_COMMAND(ID_PF_DRAW_POLYGON, &CGraphView::OnPfDrawPolygon)
	ON_COMMAND(ID_PF_SCAN, &CGraphView::OnPfScan)
	ON_COMMAND(ID_PF_SEED, &CGraphView::OnPfSeed)
	ON_COMMAND(ID_PF_SCAN_SEED, &CGraphView::OnPfScanSeed)
	ON_COMMAND(ID_PF_CLEANFILL, &CGraphView::OnPfCleanfill)
	ON_WM_LBUTTONDBLCLK()

	//test4添加
	ON_COMMAND(ID_IMGTRANS_TRANSLATE, &CGraphView::OnImgtransTranslate)
	ON_COMMAND(ID_IMGTRANS_SYMMETRY_X, &CGraphView::OnImgtransSymmetryX)
	ON_COMMAND(ID_IMGTRANS_SYMMETRY_Y, &CGraphView::OnImgtransSymmetryY)
	ON_COMMAND(ID_IMGTRANS_SYMMETRY_O, &CGraphView::OnImgtransSymmetryO)
	ON_COMMAND(ID_IMGTRANS_SCALE_LARGER, &CGraphView::OnImgtransScaleLarger)
	ON_COMMAND(ID_IMGTRANS_SCALE_SMALLER, &CGraphView::OnImgtransScaleSmaller)
	ON_COMMAND(ID_IMGTRANS_ROTATE_X, &CGraphView::OnImgtransRotateX)
	ON_COMMAND(ID_IMGTRANS_ROTATE_Y, &CGraphView::OnImgtransRotateY)
	ON_COMMAND(ID_IMGTRANS_ROTATE_Z, &CGraphView::OnImgtransRotateZ)
	ON_COMMAND(ID_IMGTRANS_CLEANTRANS, &CGraphView::OnImgtransCleantrans)
	ON_COMMAND(IDB_BUTTON_START, &CGraphView::OnButtonStart_Click)
	ON_COMMAND(IDB_BUTTON_END, &CGraphView::OnButtonEnd_Click)
	ON_WM_TIMER() //对应计时器函数OnTimer

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

	//test4添加
	m_startButton.DestroyWindow();
	m_endButton.DestroyWindow();

	m_save_bitmap.DeleteObject();
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

	//test3添加
	if (m_IsDrawPolygonDone == true)
	{
		if (m_GraphMode < 9 || m_GraphMode > 12)
		{
			m_IsDrawPolygonDone = false;
		}

		//test4添加
		if (m_GraphMode >= 20 && m_GraphMode <= 22)
		{
			m_IsDrawPolygonDone = true;
		}
	}

	//test4添加，指示前一个绘制模式
	if (m_OldGraphMode1 != m_GraphMode)
	{
		m_OldGraphMode2 = m_OldGraphMode1;
	}
	m_OldGraphMode1 = m_GraphMode;

	//test4添加
	//图形旋转后将按钮隐藏
	if (m_OldGraphMode2 >= 20 && m_OldGraphMode2 <= 22)
	{
		if (m_GraphMode < 20 || m_GraphMode>22)
		{
			m_startButton.ShowWindow(SW_HIDE);
			m_endButton.ShowWindow(SW_HIDE);
		}
	}

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
			if (m_isStop == false)
			{
				//绘制图形,显示绘制轨迹
				DrawGraph(pDC, m_StartPoint, m_MovePoint, m_color, m_GraphMode);
			}
			else if (m_isStop == true)
			{
				//绘制图形的最终结果
				DrawGraph(pDC, m_StartPoint, m_EndPoint, m_color, m_GraphMode);
			}
		}
		else if (m_GraphMode == Draw_Polygon) //如果是绘制多边形
		{
			m_Polygon_color = m_color;
			if (m_isStop == false)
			{
				//绘制图形,显示绘制轨迹
				DrawGraph(pDC, m_Polygon_Node, m_MovePoint, m_Polygon_color, m_GraphMode);
			}
			else if (m_isStop == true)
			{
				//绘制图形的最终结果
				DrawGraph(pDC, m_Polygon_Node, m_EndPoint, m_Polygon_color, m_GraphMode);

				m_IsDrawPolygonDone = true;
				m_IsFillDone = false;
			}
		}
		else if (m_GraphMode == PolygonFill_Scan) //如果是扫描线填充
		{
			//绘制多边形
			m_polygon.DrawPolygon(pDC, m_Polygon_color);

			m_polygon.m_fill_color = m_color;
			m_polygon.ScanLineFill(pDC);
			m_IsFillDone = true;
			MessageBox(_T("填充完成！"), _T("提示"), MB_OK);
		}
		else if (m_GraphMode == PolygonFill_Seed) //如果是种子填充
		{
			//绘制多边形
			m_polygon.DrawPolygon(pDC, m_Polygon_color);

			//获得种子颜色
			COLORREF initial_seed_color = pDC->GetPixel(m_SeedPoint);
			m_polygon.m_fill_color = m_color;
			m_polygon.m_old_color = initial_seed_color;
			m_polygon.SeedFill4withStack(pDC, m_SeedPoint);
			m_IsFillDone = true;
			MessageBox(_T("填充完成！"), _T("提示"), MB_OK);
		}
		else if (m_GraphMode == PolygonFill_ScanSeed) //如果是扫描线种子填充
		{
			//绘制多边形
			m_polygon.DrawPolygon(pDC, m_Polygon_color);

			COLORREF initial_seed_color = pDC->GetPixel(m_SeedPoint);
			m_polygon.m_fill_color = m_color;
			m_polygon.m_old_color = initial_seed_color;
			m_polygon.ScanLineSeedFill(pDC, m_SeedPoint);
			m_IsFillDone = true;
			MessageBox(_T("填充完成！"), _T("提示"), MB_OK);
		}
		else if (m_GraphMode == PolygonFill_CleanFill) //如果是清除填充
		{
			//绘制多边形
			m_polygon.DrawPolygon(pDC, m_Polygon_color);

			m_IsDrawPolygonDone = true;
			m_IsFillDone = false;
		}
		else if (m_GraphMode >= 14 && m_GraphMode <= 19) //如果是图形变换(不包括图形旋转和清除变换)
		{
			if (m_OldGraphMode2 >= 20 && m_OldGraphMode2 <= 22) //如果上一个绘制模式是图形旋转
			{
				m_flush_num++;
				if (m_flush_num > 1)
				{
					CleanOldGraph(pDC);
					SaveGraph2Bmp(pDC, m_bitmap);
					MessageBox(_T("请先绘制直线、圆、椭圆或多边形！"), _T("提示"), MB_OK);
				}
				return;
			}

			//从位图中得到图形点
			GetGraphPointsFromBmp(m_bitmap);

			if (m_GraphPoints.empty())
			{
				MessageBox(_T("请先绘制直线、圆、椭圆或多边形！"), _T("提示"), MB_OK);
				return;
			}

			CString message;
			vector<CPoint> TransedPoints;
			GMatrix temp = GMatrix();
			GMatrix M; //变换矩阵

			if (m_GraphMode == ImgTrans_Translate) //如果是图形平移
			{
				//图形平移变换
				//M = temp.Translating(m_width / 5, 0, 0);
				M = temp.Translating(20, 0, 0);
				int a = M.m_Data[0][0];
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);
				//message = _T("向右平移了窗口的1/5！");
				message = _T("向右平移了20个像素！");
			}
			else if (m_GraphMode == ImgTrans_Symmetry_X) //如果是关于X轴对称
			{
				//绘制坐标轴
				DrawXOY(pDC, m_color);

				//关于X轴对称
				M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.SymmetryInX()),
					temp.Translating(m_width / 2, m_height / 2, 0));
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);
				message = _T("关于X轴对称！");
			}
			else if (m_GraphMode == ImgTrans_Symmetry_Y) //如果是关于Y轴对称
			{
				//绘制坐标轴
				DrawXOY(pDC, m_color);

				//关于Y轴对称
				M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.SymmetryInY()),
					temp.Translating(m_width / 2, m_height / 2, 0));
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);

				message = _T("关于Y轴对称！");
			}
			else if (m_GraphMode == ImgTrans_Symmetry_O) //如果是关于原点对称
			{
				//绘制坐标轴
				DrawXOY(pDC, m_color);

				//关于原点对称
				M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.SymmetryInO()),
					temp.Translating(m_width / 2, m_height / 2, 0));
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);

				message = _T("关于原点对称！");
			}
			else if (m_GraphMode == ImgTrans_Scale_Larger) //如果是图形放大
			{
				//图形放大
				M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.Scaling(2)),
					temp.Translating(m_width / 2, m_height / 2, 0));
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);

				message = _T("放大1倍！");
			}
			else if (m_GraphMode == ImgTrans_Scale_Smaller) //如果是图形缩小
			{
				//图形缩小
				M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.Scaling(0.5)),
					temp.Translating(m_width / 2, m_height / 2, 0));
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);

				message = _T("缩小一半！");
			}

			//绘制旧的图形和变换后的图形
			//DrawGraphPoints(pDC, m_GraphPoints, m_GraphPointsColors);
			DrawGraphPoints(pDC, TransedPoints, m_GraphPointsColors);
			// 显示1秒后自动关闭的消息框
			ShowAutoCloseMessageBox(message, _T("提示"), MB_OK, 1000);
		}
		else if (m_GraphMode >= 20 && m_GraphMode <= 22) //如果是图形旋转
		{
			m_flush_num = 0;

			//直接对客户区进行GetPixel和SetPixel，考虑到屏幕刷新，速度是很慢的
			//可通过双缓冲技术进行速度提升：
			//创建临时的内存DC
			//然后把客户区的图形保存到内存DC中
			//这样可以直接在利用内存DC进行GetPixel和SetPixel操作
			//操作结束后，把内存DC复制到客户区上
			//
			//而且，为了进一步加快绘制效率，采取不进行背景重绘的方法
			//即强制重绘时使用Invalidate(FALSE)
			//直接将内存DC绘制到客户区
			CBitmap bitmap;
			bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);

			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

			//把位图m_save_bitmap复制到bitmap
			if (m_save_bitmap.GetSafeHandle() != NULL)
			{
				CDC dcSrc;
				dcSrc.CreateCompatibleDC(pDC);
				CBitmap* pOldSrcBitmap2 = dcSrc.SelectObject(&m_save_bitmap);
				memDC.BitBlt(0, 0, m_width, m_height, &dcSrc, 0, 0, SRCCOPY);
				dcSrc.SelectObject(pOldSrcBitmap2);
				dcSrc.DeleteDC();
			}

			//绘制多边形
			if (m_polygon.m_Nodes != nullptr)
			{
				if (m_IsDrawPolygonDone == false)
				{
					m_Polygon_color = m_color;
					if (m_isStop == false)
					{
						//绘制图形,显示绘制轨迹
						DrawGraph(&memDC, m_Polygon_Node, m_MovePoint, m_Polygon_color, Draw_Polygon);
					}
					else if (m_isStop == true)
					{
						if (m_polygon.m_Nodes != nullptr)
						{
							//绘制图形的最终结果
							DrawGraph(&memDC, m_Polygon_Node, m_EndPoint, m_Polygon_color, Draw_Polygon);

							m_IsDrawPolygonDone = true;
							m_IsFillDone = false;
						}
					}
				}
			}


			//如果多边形绘制结束
			if (m_IsDrawPolygonDone == true)
			{
				//绘制多边形
				m_polygon.DrawPolygon(&memDC, m_color);

				if (m_isStartRotate == true)
				{
					double pi = 3.14159;

					int split_num = 50; //这个值越高，旋转得越慢，但帧率越高
					m_theta += pi / split_num;

					GMatrix temp = GMatrix();
					GMatrix M;
					if (m_GraphMode == ImgTrans_Rotate_X)
					{
						//绕X轴逆时针旋转
						M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.RotatingX(m_theta)),
							temp.Translating(m_width / 2, m_height / 2, 0));
					}
					else if (m_GraphMode == ImgTrans_Rotate_Y)
					{
						//绕Y轴逆时针旋转
						M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.RotatingY(m_theta)),
							temp.Translating(m_width / 2, m_height / 2, 0));
					}
					else if (m_GraphMode == ImgTrans_Rotate_Z)
					{
						//绕Y轴逆时针旋转
						M = temp.Mult(temp.Mult(temp.Translating(-m_width / 2, -m_height / 2, 0), temp.RotatingZ(m_theta)),
							temp.Translating(m_width / 2, m_height / 2, 0));
					}

					POLYGON new_polygon;
					temp.PolygonConvert(M, m_polygon, new_polygon);

					if (m_GraphMode == ImgTrans_Rotate_Z)
					{
						if (m_theta >= 0 && m_theta < pi / 2) //如果是绕Z轴旋转
						{
							new_polygon.m_fill_color = RGB(255, 0, 0);
							new_polygon.ScanLineFill(&memDC);
						}
						else if (m_theta >= pi / 2 && m_theta < pi)
						{
							new_polygon.m_fill_color = RGB(0, 255, 0);
							new_polygon.ScanLineFill(&memDC);
						}
						else if (m_theta >= pi && m_theta < pi * 3 / 2)
						{
							new_polygon.m_fill_color = RGB(0, 0, 255);
							new_polygon.ScanLineFill(&memDC);
						}
						else if (m_theta >= pi * 3 / 2 && m_theta < 2 * pi)
						{
							new_polygon.m_fill_color = RGB(255, 0, 255);
							new_polygon.ScanLineFill(&memDC);
						}
					}
					else //如果是绕X轴或者Y轴旋转
					{
						if (m_theta <= pi / 2 || m_theta >= pi * 3 / 2)
						{
							new_polygon.m_fill_color = RGB(255, 0, 0);
							new_polygon.ScanLineFill(&memDC);
						}
						else
						{
							new_polygon.m_fill_color = RGB(0, 0, 255);
							new_polygon.ScanLineFill(&memDC);
						}
					}


					if (m_theta >= 2 * pi)
					{
						m_theta = 0;
					}


					m_isNeedInvalidate = true;
					Invalidate(FALSE);
				}
			}

			pDC->BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			bitmap.DeleteObject();
		}
		else if (m_GraphMode == ImgTrans_CleanTrans) //如果是清除变换
		{
			if (m_OldGraphMode2 >= 20 && m_OldGraphMode2 <= 22) //如果上一个绘制模式是图形旋转
			{
				m_flush_num++;
				if (m_flush_num > 1)
				{
					CleanOldGraph(pDC);
					SaveGraph2Bmp(pDC, m_bitmap);
					MessageBox(_T("请先绘制直线、圆、椭圆或多边形！"), _T("提示"), MB_OK);
				}
				return;
			}

			if (m_GraphPoints.empty())
			{
				MessageBox(_T("请先绘制直线、圆、椭圆或多边形！"), _T("提示"), MB_OK);
				return;
			}

			//绘制旧的图形和变换后的图形
			DrawGraphPoints(pDC, m_GraphPoints, m_GraphPointsColors);
		}

		// 绘制结束后将客户区内容复制到位图中
		// 以实现系统自动重绘时，将保存在的内容重新绘制出来
		SaveGraph2Bmp(pDC, m_bitmap);

		//test4修改
		if (m_GraphMode >= 20 && m_GraphMode <= 22) //如果是图形旋转
		{
			m_isNeedInvalidate = true;
		}
		else
		{
			m_isNeedInvalidate = false;
		}
	}
	else if (m_isNeedInvalidate == false) //如果是系统自动重绘
	{
		//test4修改
		if (m_GraphMode >= 20 && m_GraphMode <= 22) //如果是图形旋转
		{
			return;
		}
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

//test1添加，test2修改，test3修改
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
	case(Draw_Polygon):
	{
		DrawOldPolygon(pDC, Line_Bresenham2, color); //把之前已经绘制过的多边形的边再重新绘制一遍
		DrawGraph(pDC, P0, P1, color, Line_Bresenham2); //绘制最新的多边形的边
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


//test1添加，test2修改，test3修改，test4修改
//鼠标左键点击的响应函数
void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//如果没有选择绘制图形，则鼠标左键点击不进行任何操作
	if (m_GraphMode == NoMode)
	{
		return;
	}

	// 如果绘制已经开始
	//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
	if (m_GraphMode >= 1 && m_GraphMode <= 8)
	{
		//获得鼠标当前位置，称为起点m_StartPoint
		m_StartPoint = point;
		m_isStop = false;
	}
	//test4修改
	else if (m_GraphMode == Draw_Polygon || (m_GraphMode >= 20 && m_GraphMode <= 22)) //如果是绘制多边形
	{
		if (m_isStop == true && m_IsDrawPolygonDone == false)
		{
			//获得起点
			m_StartPoint = point;
			m_Polygon_Node = point;
			m_polygon.AddNode(point);
			m_isStop = false;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

//test1添加，test2修改，test3修改，test4修改
//鼠标移动的响应函数
void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	//test4修改
	else if (m_GraphMode == Draw_Polygon || (m_GraphMode >= 20 && m_GraphMode <= 22)) //如果是绘制多边形
	{
		if (nFlags & MK_LBUTTON) //只有鼠标移动的同时鼠标左键按下才会执行
		{
			m_MovePoint = point;
			m_isNeedInvalidate = true;
			if (m_GraphMode == Draw_Polygon)
			{
				Invalidate(TRUE);
			}
			else
			{
				Invalidate(FALSE);
			}
		}
	}

	CView::OnMouseMove(nFlags, point);
}


//test1添加，test2修改，test3修改，test4修改
//鼠标左键松开的响应函数
void CGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isStop == true)
	{
		return;
	}

	// 如果绘制已经开始
	//test2修改，因为绘制直线和绘制圆或椭圆的原理相近，所以将m_GraphMode范围由[1,5]扩展至[1,8]即可
	if (m_GraphMode >= 1 && m_GraphMode <= 8)
	{
		// 获取鼠标当前位置，称为终点m_EndPoint
		m_EndPoint = point;
		// 将m_isStop设为true，指示绘制已经停止
		m_isStop = true;
		// 当m_isNeedInvalidate设为true后,
		// 强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
		m_isNeedInvalidate = true;
		Invalidate(TRUE); //强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
	}
	//test4修改
	else if (m_GraphMode == Draw_Polygon || (m_GraphMode >= 20 && m_GraphMode <= 22)) //如果是绘制多边形
	{
		//如果上一个多边形顶点与多边形起点接近，则认为多边形闭合，并绘制上一个多边形顶点到起点的直线
		if (sqrt((point.x - m_StartPoint.x) * (point.x - m_StartPoint.x) + (point.y - m_StartPoint.y) * (point.y - m_StartPoint.y)) <= 15)
		{
			m_EndPoint = m_StartPoint;
			m_isStop = true;//闭合多边形，停止绘制多边形
			m_IsDrawPolygonDone = true;
			m_isNeedInvalidate = true;
			if (m_GraphMode == Draw_Polygon)
			{
				Invalidate(TRUE);
			}
			else
			{
				Invalidate(FALSE);
			}
		}
		else
		{
			m_polygon.AddNode(point);
			m_Polygon_Node = point; //更新上一个多边形顶点
		}
	}
	

	CView::OnLButtonUp(nFlags, point);
}

//test3添加
//清除绘制的图形
void CGraphView::CleanOldGraph(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	//用背景色绘制矩形，覆盖之前绘制的内容
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
}

//test3添加
//把之前已经绘制过的多边形的边再重新绘制一遍
void CGraphView::DrawOldPolygon(CDC* pDC, GRAPHMODE GraphMode, COLORREF color)
{
	for (int i = 0; i < m_polygon.m_NodeNum - 1; i++)
	{
		DrawGraph(pDC, m_polygon.m_Nodes[i], m_polygon.m_Nodes[i + 1], color, GraphMode);
	}
}

//test4添加
//从位图中获得图形的点坐标，默认背景色像素值是RGB(255,255,255)
void CGraphView::GetGraphPointsFromBmp(CBitmap& bitmap)
{
	//清除数据
	m_GraphPoints.clear();
	m_GraphPointsColors.clear();

	//如果CBitmap对象未被创建
	if (bitmap.GetSafeHandle() == NULL)
	{
		return;
	}

	// 获取位图信息
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);

	// 计算位图字节数
	DWORD bitmapSize = bmpInfo.bmHeight * bmpInfo.bmWidthBytes;

	// 分配位图内存
	BYTE* pBitmapData = new BYTE[bitmapSize];

	// 获取位图数据
	if (bitmap.GetBitmapBits(bitmapSize, pBitmapData) != bitmapSize) {
		// 处理获取失败的情况
		AfxMessageBox(_T("Failed to get bitmap data"));
		delete[] pBitmapData;
		return;
	}

	//pBitmapData 存储的是位图的像素。在Windows中，位图数据按行存储，每一行的像素数据是连续存储的。
	//对于 24 位的位图（每个像素有 RGB 三个分量），每个像素的数据占用 3 个字节（一个字节存储一个分量），顺序是 BGR（蓝色、绿色、红色）。
	//对于 32 位的位图（每个像素有 ARGB 四个分量），每个像素的数据占用 4 个字节，顺序是 ABGR（透明度、蓝色、绿色、红色）。
	// 遍历每个像素并显示其值
	for (int i = 0; i < bmpInfo.bmHeight; i++) {
		for (int j = 0; j < bmpInfo.bmWidth; j++) {
			// 获取像素索引
			int pixelIndex = i * bmpInfo.bmWidthBytes + j * 4;


			// 获取像素颜色
			BYTE blue = pBitmapData[pixelIndex];
			BYTE green = pBitmapData[pixelIndex + 1];
			BYTE red = pBitmapData[pixelIndex + 2];

			//如果不是背景色，则说明是图形坐标
			if (blue != 255 || green != 255 || red != 255)
			{
				m_GraphPoints.push_back(CPoint(j, i));
				m_GraphPointsColors.push_back(RGB(red, green, blue));
			}
		}
	}

	// 释放位图内存
	delete[] pBitmapData;
}

//根据点的坐标及颜色来绘制图形
void CGraphView::DrawGraphPoints(CDC* pDC, vector<CPoint>& GraphPoints, vector<COLORREF>& GraphPointsColors)
{
	for (int i = 0; i < GraphPoints.size(); i++)
	{
		pDC->SetPixel(GraphPoints[i], GraphPointsColors[i]);
	}
}

//生成自动关闭的弹窗
//其中lpText和lpCaption分别为消息框的内容和标题，uTimeout为自动关闭的超时时间（单位：毫秒）。如果uTimeout小于等于0，则表示不自动关闭。
void CGraphView::ShowAutoCloseMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, UINT uTimeout)
{
	SetTimer(1, uTimeout, NULL);
	MessageBox(lpText, lpCaption, uType | MB_TOPMOST);
	return;
}


//test4添加
//绘制坐标轴
void CGraphView::DrawXOY(CDC* pDC, COLORREF color)
{
	//绘制X轴
	CPoint P1(10, m_height / 2);
	CPoint P2(m_width - 10, m_height / 2);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	P1 = CPoint(m_width - 25, m_height / 2 - 7);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	P1 = CPoint(m_width - 25, m_height / 2 + 7);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);

	// 创建字体对象
	CFont font;
	font.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	// 选入字体对象
	CFont* pOldFont = pDC->SelectObject(&font);
	// 设置字体颜色
	pDC->SetTextColor(color);
	// 绘制文字
	CString str = _T("X");
	pDC->TextOut(m_width - 23, m_height / 2 + 10, str);


	P1 = CPoint(m_width / 2, 10);
	P2 = CPoint(m_width / 2, m_height - 10);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	P1 = CPoint(m_width / 2 + 7, m_height - 25);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	P1 = CPoint(m_width / 2 - 7, m_height - 25);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	// 绘制文字
	str = _T("Y");
	pDC->TextOut(m_width / 2 + 13, m_height - 25, str);

	// 恢复原来的字体和 CDC 对象
	pDC->SelectObject(pOldFont);
}

//绘制坐标轴和按钮到位图上，其中x，y是按钮的左上角坐标
void CGraphView::Draw_XOY_And_Button_2_Bmp(CButton& button, int x, int y, CBitmap& bitmap)
{
	CDC* pDC = GetDC();

	//保存坐标轴以及按钮到bitmap中
	if (bitmap.GetSafeHandle() == NULL)
	{
		bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	}
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	CRect rect_client;
	GetClientRect(&rect_client);
	//用白色色绘制矩形，覆盖之前绘制的内容
	memDC.FillSolidRect(&rect_client, RGB(255, 255, 255));

	//绘制坐标轴
	DrawXOY(&memDC, RGB(0, 0, 0));

	// 设置视口原点的位置为 (x, y)
	memDC.SetViewportOrg(x, y);
	// 发送 WM_PRINTCLIENT 消息，让按钮将自己绘制到内存设备上下文中
	button.SendMessage(WM_PRINTCLIENT, (WPARAM)memDC.m_hDC, PRF_CLIENT);

	//释放内存
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	ReleaseDC(pDC);
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

//test3添加
// 菜单栏“绘制多边形”选项对应的响应函数
void CGraphView::OnPfDrawPolygon()
{
	// TODO: 在此添加命令处理程序代码
	// 更改m_GraphMode变量，指示当前正在执行绘制多边形
	m_GraphMode = Draw_Polygon;

	//清空顶点数组
	if (m_polygon.m_Nodes != nullptr)
	{
		delete[] m_polygon.m_Nodes;
		m_polygon.m_Nodes = nullptr;
	}
	m_polygon.m_NodeNum = 0;

	//清除之前绘制的图形
	CDC* pDC = GetDC();
	CleanOldGraph(pDC);
	ReleaseDC(pDC);

	m_IsDrawPolygonDone = false;

	m_isStop = true;
}

//test3添加
// 菜单栏“扫描线填充”选项对应的响应函数
void CGraphView::OnPfScan()
{
	// TODO: 在此添加命令处理程序代码
	if (m_IsDrawPolygonDone == false)
	{
		MessageBox(_T("请先绘制多边形！"), _T("提示"), MB_OK);
		return;
	}
	if (m_IsFillDone == true)
	{
		MessageBox(_T("已填充完成，请重新绘制多边形或者清除填充！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = PolygonFill_Scan;

	m_isNeedInvalidate = true;
	Invalidate(TRUE); //强制重绘
}

//test3添加
// 菜单栏“种子填充”选项对应的响应函数
void CGraphView::OnPfSeed()
{
	// TODO: 在此添加命令处理程序代码
	if (m_IsDrawPolygonDone == false)
	{
		MessageBox(_T("请先绘制多边形！"), _T("提示"), MB_OK);
		return;
	}
	if (m_IsFillDone == true)
	{
		MessageBox(_T("已填充完成，请重新绘制多边形或者清除填充！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = PolygonFill_Seed;
}

//test3添加
// 菜单栏“扫描线种子填充”选项对应的响应函数
void CGraphView::OnPfScanSeed()
{
	// TODO: 在此添加命令处理程序代码
	if (m_IsDrawPolygonDone == false)
	{
		MessageBox(_T("请先绘制多边形！"), _T("提示"), MB_OK);
		return;
	}
	if (m_IsFillDone == true)
	{
		MessageBox(_T("已填充完成，请重新绘制多边形或者清除填充！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = PolygonFill_ScanSeed;
}

// 菜单栏“清除填充”选项对应的响应函数
void CGraphView::OnPfCleanfill()
{
	// TODO: 在此添加命令处理程序代码


	if (m_IsDrawPolygonDone == false)
	{
		MessageBox(_T("请先绘制多边形！"), _T("提示"), MB_OK);
		return;
	}
	if (m_IsFillDone == true)
	{
		m_GraphMode = PolygonFill_CleanFill;
		m_isNeedInvalidate = true;
		Invalidate(TRUE);
	}
	else
	{
		MessageBox(_T("请先填充多边形！"), _T("提示"), MB_OK);
		return;
	}
}

//test3添加
// 鼠标双击事件的响应函数
void CGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_GraphMode == PolygonFill_Seed || m_GraphMode == PolygonFill_ScanSeed)
	{
		if (m_IsFillDone == true)
		{
			MessageBox(_T("已填充完成，请重新绘制多边形或者清除填充！"), _T("提示"), MB_OK);
			return;
		}
		m_SeedPoint = point;
		m_isNeedInvalidate = true;
		Invalidate(TRUE);
	}

	CView::OnLButtonDblClk(nFlags, point);
}

//test4添加
// 菜单栏“图形平移”选项对应的响应函数
void CGraphView::OnImgtransTranslate()
{
	// TODO: 在此添加命令处理程序代码
	m_GraphMode = ImgTrans_Translate;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}


// 菜单栏“关于X轴对称”选项对应的响应函数
void CGraphView::OnImgtransSymmetryX()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Symmetry_X;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

// 菜单栏“关于Y轴对称”选项对应的响应函数
void CGraphView::OnImgtransSymmetryY()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Symmetry_Y;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

// 菜单栏“关于原点对称”选项对应的响应函数
void CGraphView::OnImgtransSymmetryO()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Symmetry_O;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

// 菜单栏“放大”选项对应的响应函数
void CGraphView::OnImgtransScaleLarger()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Scale_Larger;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

// 菜单栏“缩小”选项对应的响应函数
void CGraphView::OnImgtransScaleSmaller()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Scale_Smaller;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

// 菜单栏“绕X轴旋转”选项对应的响应函数
void CGraphView::OnImgtransRotateX()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Rotate_X;

	//初始化参数
	//
	//清空顶点数组
	if (m_polygon.m_Nodes != nullptr)
	{
		delete[] m_polygon.m_Nodes;
		m_polygon.m_Nodes = nullptr;
	}
	m_polygon.m_NodeNum = 0;
	m_IsDrawPolygonDone = false;
	m_theta = 0;
	m_isStartRotate = false;

	//创建开始和暂停按钮
	CRect rect(m_width - 120, 20, m_width - 20, 70);
	if (m_startButton.GetSafeHwnd() == NULL)
	{
		m_startButton.Create(_T("开始旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_START);
	}
	if (m_endButton.GetSafeHwnd() == NULL)
	{
		m_endButton.Create(_T("停止旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_END);
	}

	m_startButton.ShowWindow(SW_SHOW);
	m_endButton.ShowWindow(SW_HIDE);

	//清除之前绘制的内容
	CDC* pDC = GetDC();
	CleanOldGraph(pDC);
	ReleaseDC(pDC);

	//绘制坐标轴和按钮到CBitmap对象中
	Draw_XOY_And_Button_2_Bmp(m_startButton, m_width - 120, 20, m_save_bitmap);

	//强制重绘
	m_isNeedInvalidate = true;
	Invalidate(false);

	ShowAutoCloseMessageBox(_T("请开始绘制一个多边形！"), _T("提示"), MB_OK, 1000);
}

// 菜单栏“绕Y轴旋转”选项对应的响应函数
void CGraphView::OnImgtransRotateY()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Rotate_Y;

	//初始化参数
	//
	//清空顶点数组
	if (m_polygon.m_Nodes != nullptr)
	{
		delete[] m_polygon.m_Nodes;
		m_polygon.m_Nodes = nullptr;
	}
	m_polygon.m_NodeNum = 0;
	m_IsDrawPolygonDone = false;
	m_theta = 0;
	m_isStartRotate = false;

	//创建开始和暂停按钮
	CRect rect(m_width - 120, 20, m_width - 20, 70);
	if (m_startButton.GetSafeHwnd() == NULL)
	{
		m_startButton.Create(_T("开始旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_START);
	}
	if (m_endButton.GetSafeHwnd() == NULL)
	{
		m_endButton.Create(_T("停止旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_END);
	}

	m_startButton.ShowWindow(SW_SHOW);
	m_endButton.ShowWindow(SW_HIDE);


	//清除之前绘制的内容
	CDC* pDC = GetDC();
	CleanOldGraph(pDC);
	ReleaseDC(pDC);

	//绘制坐标轴和按钮到CBitmap对象中
	Draw_XOY_And_Button_2_Bmp(m_startButton, m_width - 120, 20, m_save_bitmap);


	//强制重绘
	m_isNeedInvalidate = true;
	Invalidate(false);

	ShowAutoCloseMessageBox(_T("请开始绘制一个多边形！"), _T("提示"), MB_OK, 1000);
}

// 菜单栏“绕Z轴旋转”选项对应的响应函数
void CGraphView::OnImgtransRotateZ()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_Rotate_Z;

	//初始化参数
	//
	//清空顶点数组
	if (m_polygon.m_Nodes != nullptr)
	{
		delete[] m_polygon.m_Nodes;
		m_polygon.m_Nodes = nullptr;
	}
	m_polygon.m_NodeNum = 0;
	m_IsDrawPolygonDone = false;
	m_theta = 0;
	m_isStartRotate = false;

	//创建开始和暂停按钮
	CRect rect(m_width - 120, 20, m_width - 20, 70);
	if (m_startButton.GetSafeHwnd() == NULL)
	{
		m_startButton.Create(_T("开始旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_START);
	}
	if (m_endButton.GetSafeHwnd() == NULL)
	{
		m_endButton.Create(_T("停止旋转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, IDB_BUTTON_END);
	}

	m_startButton.ShowWindow(SW_SHOW);
	m_endButton.ShowWindow(SW_HIDE);


	//清除之前绘制的内容
	CDC* pDC = GetDC();
	CleanOldGraph(pDC);
	ReleaseDC(pDC);

	//绘制坐标轴和按钮到CBitmap对象中
	Draw_XOY_And_Button_2_Bmp(m_startButton, m_width - 120, 20, m_save_bitmap);

	//CImage image;
	//image.Attach(m_save_bitmap);
	//image.Save(_T("E:\\MyBitmap.bmp"), Gdiplus::ImageFormatBMP);
	//image.Detach();

	//强制重绘
	m_isNeedInvalidate = true;
	Invalidate(false);

	ShowAutoCloseMessageBox(_T("请开始绘制一个多边形！"), _T("提示"), MB_OK, 1000);
}

//开始按钮响应函数
void CGraphView::OnButtonStart_Click()
{
	if (m_IsDrawPolygonDone == false)
	{
		return;
	}

	m_isStartRotate = true; //开始旋转

	SetRedraw(FALSE);
	m_startButton.ShowWindow(SW_HIDE);
	m_endButton.ShowWindow(SW_SHOW);
	SetRedraw(TRUE);

	//绘制坐标轴和按钮到CBitmap对象中
	Draw_XOY_And_Button_2_Bmp(m_endButton, m_width - 120, 20, m_save_bitmap);

	//CImage image;
	//image.Attach(m_save_bitmap);
	//image.Save(_T("E:\\MyImage.bmp"), Gdiplus::ImageFormatBMP);
	//image.Detach();

	m_isNeedInvalidate = true;
	Invalidate(false);
}

//暂停按钮响应函数
void CGraphView::OnButtonEnd_Click()
{
	m_isStartRotate = false; //停止旋转

	SetRedraw(FALSE);
	m_startButton.ShowWindow(SW_SHOW);
	m_endButton.ShowWindow(SW_HIDE);
	SetRedraw(TRUE);

	//绘制坐标轴和按钮到CBitmap对象中
	Draw_XOY_And_Button_2_Bmp(m_startButton, m_width - 120, 20, m_save_bitmap);

	m_isNeedInvalidate = false;
	Invalidate(false);
}

// 菜单栏“清除变换”选项对应的响应函数
void CGraphView::OnImgtransCleantrans()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = ImgTrans_CleanTrans;

	m_isNeedInvalidate = true;
	Invalidate(TRUE);
}

//计时器
void CGraphView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case(1):
	{
		//关闭定时器
		KillTimer(1);
		CWnd* hWnd = FindWindow(NULL, _T("提示"));
		if (hWnd)
		{
			hWnd->PostMessage(WM_CLOSE, NULL, NULL);
		}
		break;
	}
	}

	CView::OnTimer(nIDEvent);
}