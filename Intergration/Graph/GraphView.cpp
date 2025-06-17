
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
#include "SetLightDiaglog.h"
#include <chrono>

#define DEG2RAD(angle) ((angle) * 3.14159265 / 180.0)

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

	//test5添加
	ON_COMMAND(ID_LINECLIP_CS, &CGraphView::OnLineclipCs)
	ON_COMMAND(ID_LINECLIP_MID, &CGraphView::OnLineclipMid)
	ON_COMMAND(ID_LINECLIP_LB, &CGraphView::OnLineclipLb)
	ON_COMMAND(ID_POLYGONCLIP_SH, &CGraphView::OnPolygonclipSh)

	//test6添加
	ON_COMMAND(ID_CURVE_DRAW, &CGraphView::OnCurveDraw)
	ON_COMMAND(ID_CURVE_MOVE, &CGraphView::OnCurveMove)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_Light, &CGraphView::OnLight)
	ON_COMMAND(ID_Gouraud, &CGraphView::OnGouraud)
	ON_COMMAND(Phong, &CGraphView::OnPhong)
	ON_COMMAND(Blinn_Phong, &CGraphView::OnBlinn_Phong)
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

	//test5添加，确保直线裁剪前绘制了直线，而且允许直线裁剪后再次进行直线裁剪
	if (m_isBeginLineClip == true)
	{
		if ((m_GraphMode >= 1 && m_GraphMode <= 5) || (m_GraphMode >= 24 && m_GraphMode <= 26))
		{
		}
		else
		{
			m_isBeginLineClip = false;
		}
	}

	//test5添加，确保直线裁剪前绘制了直线，而且允许直线裁剪后再次进行直线裁剪
	if (m_isBeginPolygonClip == true)
	{
		if ((m_GraphMode == Draw_Polygon) || (m_GraphMode == PolygonClip_SH))
		{
		}
		else
		{
			m_isBeginPolygonClip = false;
		}
	}

	//test6添加
	if (m_isDrawCurveDone == true)
	{
		if (m_GraphMode < 28 || m_GraphMode > 29)
		{
			m_isDrawCurveDone = false;
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
		//test5修改
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

				//test5修改
				if (m_GraphMode >= 1 && m_GraphMode <= 5)
				{
					m_isBeginLineClip = true;

					//保存绘制直线时的相关信息
					m_line_clip = LINE_CLIP(m_StartPoint, m_EndPoint, m_color);
					m_line_GraphMode = m_GraphMode;

					//把绘制好的直线保存下来
					CDC* pDC = GetDC();
					SaveGraph2Bmp(pDC, m_save_bitmap);
					ReleaseDC(pDC);
				}
			}
		}
		//test5修改
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

				//test5修改
				m_isBeginPolygonClip = true;
				//保存绘制多边形时的相关信息
				m_polygon_clip.SetPolygonClip(m_polygon);
				//把绘制好的多边形保存下来
				CDC* pDC = GetDC();
				SaveGraph2Bmp(pDC, m_save_bitmap);
				ReleaseDC(pDC);
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
				M = temp.Translating(m_width / 5, 0, 0);
				int a = M.m_Data[0][0];
				temp.PointsConvert(M, m_GraphPoints, TransedPoints);
				message = _T("向右平移了窗口的1/5！");
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
			DrawGraphPoints(pDC, m_GraphPoints, m_GraphPointsColors);
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
		else if (m_GraphMode >= 24 && m_GraphMode <= 26) //如果是直线裁剪
		{
			if (m_isBeginLineClip == false)
			{
				m_isStop = true;
				return;
			}
			if (m_StartPoint == m_EndPoint) //如果是左键双击
			{
				return;
			}

			//使用双缓冲技术
			//
			//创建临时CBitmap和CDC对象
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

			if (m_isStop == false)
			{
				//绘制虚线框,显示轨迹
				DrawDottedBox(&memDC, m_StartPoint, m_MovePoint);
			}
			else if (m_isStop == true)
			{
				//清除所有绘制的内容
				CleanOldGraph(&memDC);
				//绘制裁剪后的直线
				DrawGraph(&memDC, m_StartPoint, m_EndPoint, m_line_clip.m_color, m_GraphMode);
			}

			pDC->BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY);

			//把裁剪好的直线保存下来，以便继续裁剪
			if (m_isStop == true)
			{
				SaveGraph2Bmp(pDC, m_save_bitmap);
			}

			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			bitmap.DeleteObject();
		}
		else if (m_GraphMode == PolygonClip_SH) //如果是多边形裁剪
		{
			if (m_isBeginPolygonClip == false)
			{
				m_isStop = true;
				return;
			}

			if (m_StartPoint == m_EndPoint) //如果是左键双击
			{
				return;
			}

			if (m_polygon_clip.m_NodeNum == 0)
			{
				return;
			}

			//使用双缓冲技术
			//
			//创建临时CBitmap和CDC对象
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

			if (m_isStop == false)
			{
				//绘制虚线框,显示轨迹
				DrawDottedBox(&memDC, m_StartPoint, m_MovePoint);
			}
			else if (m_isStop == true)
			{
				//清除所有绘制的内容
				CleanOldGraph(&memDC);
				//绘制裁剪后的多边形
				DrawGraph(&memDC, m_StartPoint, m_EndPoint, m_Polygon_color, m_GraphMode);
			}

			pDC->BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY);

			//把裁剪好的多边形保存下来，以便继续裁剪
			if (m_isStop == true)
			{
				SaveGraph2Bmp(pDC, m_save_bitmap);
			}

			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			bitmap.DeleteObject();
		}
		else if (m_GraphMode == Curve_Draw) //如果是曲线绘制
		{
			if (m_isStop == false)
			{
				//绘制图形,显示绘制轨迹
				DrawGraph(pDC, m_Curve_Point, m_MovePoint, m_color, m_GraphMode);
			}
			else
			{
				//绘制曲线
				m_curve.m_color = m_color;
				m_curve.Bezier(pDC, 10000);
			}
		}
		else if (m_GraphMode == Curve_Move) //如果是移动曲线控制点
		{
			//使用双缓冲技术
			//
			//创建临时CBitmap和CDC对象
			CBitmap bitmap;
			bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);


			//绘制新的曲线和控制线
			CleanOldGraph(&memDC);
			m_curve.m_CtrlPt[m_Curve_Point_index] = m_New_Curve_Point;
			DrawOldCurve(&memDC, Line_Bresenham2, m_color);
			m_curve.Bezier(&memDC, 10000);
			DrawRectangle(&memDC, m_color, m_New_Curve_Point, 5);


			pDC->BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			bitmap.DeleteObject();
		}
		else if (m_GraphMode == SetLight) //如果是设置光源
		{

			// TODO: 在此添加命令处理程序代码
			//绘制坐标轴
			COLORREF color_xyz = RGB(0, 0, 0);
			DrawXYZ(pDC, color_xyz);

			// 设置光源位置
			lightPos = { m_LightPoint.x, m_LightPoint.y, m_LightPoint.z };  // 点光源
			//设置观察者位置
			viewPos = { m_BallPoint.x, m_BallPoint.y + 500, m_BallPoint.z };
			//设置光源颜色
			float r = float(GetRValue(m_LightColor)) / 255;
			float g = float(GetGValue(m_LightColor)) / 255;
			float b = float(GetBValue(m_LightColor)) / 255;
			lightColor = { r, g, b };  
			//设置环境光
			ambientColor = { 0.2f, 0.2f, 0.2f }; 


			//绘制球面
			int stacks = 20;            // 纬线数
			int slices = 20;            // 经线数
			auto ellipses = m_renderer.generateSphereEllipses(m_BallScreenPoint.x, m_BallScreenPoint.y, m_Radius, stacks, slices);
			for (auto it = ellipses.begin(); it != ellipses.end(); ++it) {
				m_circle = CIRCLE(CPoint(it->cx, it->cy), it->rx, it->ry, m_color);
				m_circle.MidPointEllipse(pDC);
			}
			DrawLight(pDC);

		}
		else if (m_GraphMode == Gouraud || m_GraphMode == PhongShading || m_GraphMode == Blinn_PhongShading)
		{
			// TODO: 在此添加命令处理程序代码

			//绘制三维坐标
			COLORREF color_xyz = RGB(0, 0, 0);
			DrawXYZ(pDC, color_xyz);

			//生成球体矢量
			std::vector<std::pair<Vec3, Vec3>> sphere = m_renderer.generateSphere(2000, 2000, m_Radius); 
			// 存储结果
			std::vector<std::pair<Vec3, Color>> results;  

			//调用着色函数
			int ShadingMode = 0;
			if (m_GraphMode == Gouraud) ShadingMode = 1;
			else if (m_GraphMode == PhongShading) ShadingMode = 2;
			else if (m_GraphMode == Blinn_PhongShading) ShadingMode = 3;

			m_renderer.applyShadingModel(
				sphere, viewPos, lightPos, lightColor, ambientColor, m_Luminance, ShadingMode, results  // 2 = Phong模型
			);
			DrawLight(pDC);
			//绘制正面
			m_circle = CIRCLE(CPoint(m_BallScreenPoint.x, m_BallScreenPoint.y) , m_Radius, m_color);
			m_circle.MidPointCircle(pDC);

			// 遍历获取的颜色结果
			for (const auto& vertex : results) {
				Vec3 position = vertex.first;
				Color color;
				color.r = int(vertex.second.r * 255);
				color.g = int(vertex.second.g * 255);
				color.b = int(vertex.second.b * 255);

				COLORREF color_new = RGB(int(vertex.second.r * 255), color.g = int(vertex.second.g * 255), int(vertex.second.b * 255));
				pDC->SetPixel((int)round(vertex.first.x + m_BallScreenPoint.x), (int)round(-vertex.first.z + m_BallScreenPoint.y), color_new);
			}
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

// 计算用于表示球体的椭圆
void  CGraphView::DrawLight(CDC* pDC)
{
	//绘制点光源
	int rlight = 30;
	for (int i = -rlight; i < rlight; i++)  // 修正循环
	{
		for (int j = -rlight; j < rlight; j++)
		{
			if (i * i + j * j < rlight * rlight)  // 判断是否在圆内
			{
				// 计算距离中心的比率（0.0 到 1.0）
				double distance = sqrt(i * i + j * j) / rlight;
				double intensity = distance;  // 越远越暗

				// 获取原始颜色分量
				int r = GetRValue(m_LightColor);
				int g = GetGValue(m_LightColor);
				int b = GetBValue(m_LightColor);

				// 计算新的颜色（线性渐变）
				int new_r = r + (255 - r) * intensity;
				int new_g = g + (255 - g) * intensity;
				int new_b = b + (255 - b) * intensity;

				// 限制 RGB 值在 0~255
				new_r = max(0, min(255, new_r));
				new_g = max(0, min(255, new_g));
				new_b = max(0, min(255, new_b));

				// 生成新颜色
				COLORREF new_color = RGB(new_r, new_g, new_b);

				// 绘制点
				pDC->SetPixel((int)ceil(m_LightScreenPoint.x + i), (int)ceil(m_LightScreenPoint.y + j), new_color);
			}
		}
	}
	pDC->SetPixel((int)ceil(m_LightScreenPoint.x), (int)ceil(m_LightScreenPoint.y), m_LightColor + 1);

	// 绘制光源位置
	CPoint P1(m_LightScreenPoint.x, m_LightScreenPoint.y);
	CPoint P2(m_LightScreenPoint.x, m_LightScreenPoint.y + m_LightPoint.z);
	m_line = LINE(P1, P2, m_color);
	m_line.Bresenham3(pDC);

	P1 = CPoint(m_LightScreenPoint.x + m_LightPoint.y * cos(DEG2RAD(45)), m_height / 2);
	m_line = LINE(P1, P2, m_color);
	m_line.Bresenham3(pDC);

	P1 = CPoint(m_LightScreenPoint.x - m_LightPoint.x, m_LightScreenPoint.y + m_LightPoint.z);
	m_line = LINE(P1, P2, m_color);
	m_line.Bresenham3(pDC);

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

//test1添加，test2修改，test3修改，test5修改，test6修改
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
	case(lineClip_CS):
	{
		int XL = min(P0.x, P1.x);
		int XR = max(P0.x, P1.x);
		int YB = min(P0.y, P1.y);
		int YT = max(P0.y, P1.y);
		bool isLineNeedClip = m_line_clip.LineClip(XL, XR, YB, YT, m_line);

		//如果直线需要裁剪
		if (isLineNeedClip)
		{
			m_line_clip.m_Begin = m_line.m_Begin;
			m_line_clip.m_End = m_line.m_End;
		}

		//重新绘制直线
		DrawGraph(pDC, m_line_clip.m_Begin, m_line_clip.m_End, m_line_clip.m_color, m_line_GraphMode);
		break;
	}
	case(lineClip_Mid):
	{
		int XL = min(P0.x, P1.x);
		int XR = max(P0.x, P1.x);
		int YB = min(P0.y, P1.y);
		int YT = max(P0.y, P1.y);
		bool isLineNeedClip = m_line_clip.LineMidClip(XL, XR, YB, YT, m_line);

		//如果直线需要裁剪
		if (isLineNeedClip)
		{
			m_line_clip.m_Begin = m_line.m_Begin;
			m_line_clip.m_End = m_line.m_End;
		}

		//重新绘制直线
		DrawGraph(pDC, m_line_clip.m_Begin, m_line_clip.m_End, m_line_clip.m_color, m_line_GraphMode);
		break;
	}
	case(lineClip_LB):
	{
		int XL = min(P0.x, P1.x);
		int XR = max(P0.x, P1.x);
		int YB = min(P0.y, P1.y);
		int YT = max(P0.y, P1.y);
		bool isLineNeedClip = m_line_clip.LBClip(XL, XR, YB, YT, m_line);

		//如果直线需要裁剪
		if (isLineNeedClip)
		{
			m_line_clip.m_Begin = m_line.m_Begin;
			m_line_clip.m_End = m_line.m_End;
		}

		//重新绘制直线
		DrawGraph(pDC, m_line_clip.m_Begin, m_line_clip.m_End, m_line_clip.m_color, m_line_GraphMode);
		break;
	}
	case(PolygonClip_SH):
	{
		int XL = min(P0.x, P1.x);
		int XR = max(P0.x, P1.x);
		int YB = min(P0.y, P1.y);
		int YT = max(P0.y, P1.y);

		//保存绘制多边形时的相关信息
		POLYGON_CLIP polygon_clip_temp;
		bool IsPolygonNeedClip = m_polygon_clip.Surtherland_Hondgman(XL, XR, YB, YT, polygon_clip_temp);

		if (IsPolygonNeedClip)
		{
			m_polygon_clip = polygon_clip_temp;
		}

		m_polygon_clip.DrawPolygon(pDC, m_Polygon_color);
		break;
	}
	case(Curve_Draw):
	{
		DrawOldCurve(pDC, Line_Bresenham2, color); //把之前已经绘制过的曲线控制线再重新绘制一遍
		DrawGraph(pDC, P0, P1, color, Line_Bresenham2); //绘制最新的曲线控制线
		break;
	}
	case(SetLight):
	{

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


//test1添加，test2修改，test3修改，test4修改，test5修改
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
	//test5修改
	if ((m_GraphMode >= 1 && m_GraphMode <= 8) || (m_GraphMode >= 24 && m_GraphMode <= 27))
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
	else if (m_GraphMode == Curve_Draw) //如果是绘制曲线
	{
		if (m_isStop == true && m_isDrawCurveDone == false)
		{
			//获得起点
			m_StartPoint = point;
			m_Curve_Point = point;
			m_curve.m_CtrlPt.push_back(point);
			m_isStop = false;
		}
	}
	else if (m_GraphMode == Curve_Move) //如果是移动曲线控制点
	{
		if (m_isDrawCurveDone == false)
		{
			return;
		}

		//寻找距离满足要求的曲线控制点
		for (int i = 0; i < m_curve.m_CtrlPt.size(); i++)
		{
			int x = m_curve.m_CtrlPt[i].x;
			int y = m_curve.m_CtrlPt[i].y;
			if (((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y)) <= 25)
			{
				m_isStop = false;
				m_isFindCtrlPt = true;
				m_Curve_Point_index = i;
				m_New_Curve_Point = m_curve.m_CtrlPt[i];
				m_isNeedInvalidate = true;
				Invalidate(false);
			}
		}


	}

	CView::OnLButtonDown(nFlags, point);
}

//test1添加，test2修改，test3修改，test4修改，test5修改
//鼠标移动的响应函数
void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isStop == true)
	{
		return;
	}

	// 如果绘制已经开始
	//test5修改
	if ((m_GraphMode >= 1 && m_GraphMode <= 8) || (m_GraphMode >= 24 && m_GraphMode <= 27))
	{
		if (nFlags & MK_LBUTTON) //只有鼠标移动的同时鼠标左键按下才会执行
		{
			// 获取当前鼠标的位置，称为移动点m_MovePoint
			m_MovePoint = point;
			// 当m_isNeedInvalidate设为true后,
			// 再使用Invalidate(TRUE)命令后，工作区就会被清空，然后MFC会自动调用OnDraw函数，执行强制重绘
			// 而使用Invalidate(FALSE)命令不会清空工作区，会在工作区已有基础上继续绘制新的内容
			m_isNeedInvalidate = true;
			if (m_GraphMode >= 24 && m_GraphMode <= 27)
			{
				Invalidate(false);
			}
			else
			{
				Invalidate(TRUE); //强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
			}
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
	else if (m_GraphMode == Curve_Draw) //如果是曲线绘制
	{
		if (nFlags & MK_LBUTTON) //只有鼠标移动的同时鼠标左键按下才会执行
		{
			m_MovePoint = point;
			m_isNeedInvalidate = true;
			Invalidate(TRUE);
		}
	}
	else if (m_GraphMode == Curve_Move) //如果是移动曲线控制点
	{
		if (m_isFindCtrlPt == true)
		{
			m_New_Curve_Point = point;
			m_isNeedInvalidate = true;
			Invalidate(false);
		}
	}

	CView::OnMouseMove(nFlags, point);
}


//test1添加，test2修改，test3修改，test4修改，test5修改
//鼠标左键松开的响应函数
void CGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isStop == true)
	{
		return;
	}

	// 如果绘制已经开始
	//test5修改
	if ((m_GraphMode >= 1 && m_GraphMode <= 8) || (m_GraphMode >= 24 && m_GraphMode <= 27))
	{
		// 获取鼠标当前位置，称为终点m_EndPoint
		m_EndPoint = point;
		// 将m_isStop设为true，指示绘制已经停止
		m_isStop = true;
		// 当m_isNeedInvalidate设为true后,
		// 强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
		m_isNeedInvalidate = true;
		if (m_GraphMode >= 24 && m_GraphMode <= 27)
		{
			Invalidate(false);
		}
		else
		{
			Invalidate(TRUE); //强制重绘，MFC将自动调用OnDraw函数进行重绘，TRUE表示擦除之前绘制的内容
		}
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
	else if (m_GraphMode == Curve_Draw) //如果是曲线绘制
	{
		m_curve.m_CtrlPt.push_back(point);
		m_Curve_Point = point; //更新上一个曲线控制点
		m_isDrawCurveDone = true;
	}
	else if (m_GraphMode == Curve_Move) //如果是移动曲线控制点
	{
		m_isStop = true;
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


//test7添加
void CGraphView::DrawXYZ(CDC* pDC, COLORREF color)
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

	//绘制Y轴
	P1 = CPoint(m_width / 2 - m_height / 2, m_height - 10);
	P2 = CPoint(m_width / 2, m_height / 2 );
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);

	int sigma = 30;
	P2 = CPoint(m_width / 2 - m_height / 2 +  (7 * cos(DEG2RAD(sigma)) - (-25) * sin(DEG2RAD(sigma))), m_height +(7 * sin(DEG2RAD(sigma)) + (-25) * cos(DEG2RAD(sigma))));
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);

	P2 = CPoint(m_width / 2 - m_height / 2 + (-7 * cos(DEG2RAD(sigma)) - (-25) * sin(DEG2RAD(sigma))), m_height + (-7 * sin(DEG2RAD(sigma)) + (-25) * cos(DEG2RAD(sigma))));
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	// 绘制文字
	str = _T("Y");
	pDC->TextOut(m_width / 2 - m_height / 2 + 13, m_height - 25, str);

	//绘制Z轴
	P1 = CPoint(m_width / 2, m_height / 2);
	P2 = CPoint(m_width / 2, 10 );
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);

	P1 = CPoint(m_width / 2 + 7, 25);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	P1 = CPoint(m_width / 2 - 7, 25);
	m_line = LINE(P1, P2, color);
	m_line.Bresenham2(pDC);
	// 绘制文字
	str = _T("Z");
	pDC->TextOut(m_width / 2 + 13, 25, str);

	// 恢复原来的字体和 CDC 对象
	pDC->SelectObject(pOldFont);
}
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

//test5添加
//绘制虚线框
void CGraphView::DrawDottedBox(CDC* pDC, CPoint P0, CPoint P2)
{
	CPoint P1(P2.x, P0.y);
	CPoint P3(P0.x, P2.y);
	DrawGraph(pDC, P0, P1, RGB(0, 0, 0), Line_Bresenham3);
	DrawGraph(pDC, P1, P2, RGB(0, 0, 0), Line_Bresenham3);
	DrawGraph(pDC, P2, P3, RGB(0, 0, 0), Line_Bresenham3);
	DrawGraph(pDC, P3, P0, RGB(0, 0, 0), Line_Bresenham3);
}

//test6添加
//把之前已经绘制过的曲线控制线再重新绘制一遍
void CGraphView::DrawOldCurve(CDC* pDC, GRAPHMODE GraphMode, COLORREF color)
{
	for (int i = 0; i < m_curve.m_CtrlPt.size() - 1; i++)
	{
		DrawGraph(pDC, m_curve.m_CtrlPt[i], m_curve.m_CtrlPt[i + 1], color, GraphMode);
	}
}

//以一点为中心绘制矩形
void CGraphView::DrawRectangle(CDC* pDC, COLORREF color, CPoint center_point, int length)
{
	int half_length = length / double(2);
	for (int i = -half_length; i < half_length; i++)
	{
		pDC->SetPixel(CPoint(center_point.x + i, center_point.y - half_length), color);
		pDC->SetPixel(CPoint(center_point.x + i, center_point.y + half_length), color);
		pDC->SetPixel(CPoint(center_point.x - half_length, center_point.y + i), color);
		pDC->SetPixel(CPoint(center_point.x + half_length, center_point.y + i), color);
	}
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

	// CImage image;
	// image.Attach(m_save_bitmap);
	// image.Save(_T("E:\\MyBitmap.bmp"), Gdiplus::ImageFormatBMP);
	// image.Detach();

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

//test4添加
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


//test5添加
// 菜单栏“Cohen-Sutherland算法”选项对应的响应函数
void CGraphView::OnLineclipCs()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = lineClip_CS;

	if (m_isBeginLineClip == false)
	{
		MessageBox(_T("请先绘制一条直线！"), _T("提示"), MB_OK);
		m_isStop = true; //防止鼠标移动时再次触发绘制
		return;
	}
}

// 菜单栏“中点分割裁剪算法”选项对应的响应函数
void CGraphView::OnLineclipMid()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = lineClip_Mid;

	if (m_isBeginLineClip == false)
	{
		MessageBox(_T("请先绘制一条直线！"), _T("提示"), MB_OK);
		m_isStop = true; //防止鼠标移动时再次触发绘制
		return;
	}
}

// 菜单栏“梁友栋-Barsky算法”选项对应的响应函数
void CGraphView::OnLineclipLb()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = lineClip_LB;

	if (m_isBeginLineClip == false)
	{
		MessageBox(_T("请先绘制一条直线！"), _T("提示"), MB_OK);
		m_isStop = true; //防止鼠标移动时再次触发绘制
		return;
	}
}

//test5添加
// 菜单栏“多边形裁剪”/“Sutherland-Hodgeman算法”选项对应的响应函数
void CGraphView::OnPolygonclipSh()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = PolygonClip_SH;

	if (m_isBeginPolygonClip == false)
	{
		MessageBox(_T("请先绘制一个多边形！"), _T("提示"), MB_OK);
		return;
	}
}


//test6添加
// 菜单栏“Bezier曲线”选项对应的响应函数
void CGraphView::OnCurveDraw()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = Curve_Draw;

	CDC* pDC = GetDC();
	CleanOldGraph(pDC);
	ReleaseDC(pDC);
	m_curve.m_CtrlPt.clear();
	m_isDrawCurveDone = false;
}

// 菜单栏“移动曲线控制点”选项对应的响应函数
void CGraphView::OnCurveMove()
{
	// TODO: 在此添加命令处理程序代码

	m_GraphMode = Curve_Move;

	if (m_isDrawCurveDone == false)
	{
		MessageBox(_T("请先绘制曲线！"), _T("提示"), MB_OK);
		return;
	}

	m_isFindCtrlPt = false;


}


//鼠标右键松开事件的响应函数
void CGraphView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_isStop == true)
	{
		return;
	}

	if (m_GraphMode == Curve_Draw)
	{
		//如果鼠标右键松开，则曲线控制线绘制结束
		m_isStop = true;
		m_isDrawCurveDone = true;
		m_isNeedInvalidate = true;
		Invalidate(false);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CGraphView::OnLight()
{
	SetLightDiaglog SetLightDlg;

	//构造新窗口，并设置初值
	if (m_isSetLight)
	{
		//SetLightDlg.GetData(m_LightPoint.x, m_LightPoint.y, m_LightPoint.z, m_Luminance, m_BallPoint.x, m_BallPoint.y, m_BallPoint.z, m_Radius, m_LightColor);
		//CString m_strX;
		SetLightDlg.m_isSetLight = m_isSetLight;
		SetLightDlg.m_strX.Format(_T("%.2f"), m_LightPoint.x);
		SetLightDlg.m_strY.Format(_T("%.2f"), m_LightPoint.y);
		SetLightDlg.m_strZ.Format(_T("%.2f"), m_LightPoint.z);
		SetLightDlg.m_strCX.Format(_T("%.2f"), m_BallPoint.x);
		SetLightDlg.m_strCY.Format(_T("%.2f"), m_BallPoint.y);
		SetLightDlg.m_strCZ.Format(_T("%.2f"), m_BallPoint.z);
		SetLightDlg.m_strI.Format(_T("%.2f"), m_Luminance);
		SetLightDlg.m_strR.Format(_T("%.2f"), m_Radius);
		SetLightDlg.m_LightColor = m_LightColor;
	}
	SetLightDlg.DoModal();

	//获取窗口中初始化值
	m_LightPoint.x = static_cast<float>(wcstod(SetLightDlg.m_strX, nullptr)); 
	m_LightPoint.y = static_cast<float>(wcstod(SetLightDlg.m_strY, nullptr));  
	m_LightPoint.z = static_cast<float>(wcstod(SetLightDlg.m_strZ, nullptr));  

	m_BallPoint.x = static_cast<float>(wcstod(SetLightDlg.m_strCX, nullptr));
	m_BallPoint.y = static_cast<float>(wcstod(SetLightDlg.m_strCY, nullptr));
	m_BallPoint.z = static_cast<float>(wcstod(SetLightDlg.m_strCZ, nullptr));

	m_Luminance= static_cast<float>(wcstod(SetLightDlg.m_strI, nullptr));
	m_Radius = static_cast<float>(wcstod(SetLightDlg.m_strR, nullptr));

	m_LightColor = SetLightDlg.m_LightColor;

	m_renderer.TransformToScreen(m_LightPoint.x, m_LightPoint.y, m_LightPoint.z, m_LightScreenPoint, m_width, m_height);
	m_renderer.TransformToScreen(m_BallPoint.x, m_BallPoint.y, m_BallPoint.z, m_BallScreenPoint, m_width, m_height);

	//设置模式
	m_GraphMode = SetLight;
	m_isNeedInvalidate = true;
	m_isSetLight = true;
	Invalidate(true);

}


void CGraphView::OnGouraud()
{
	// TODO: 在此添加命令处理程序代码
	if (m_isSetLight == false)
	{
		MessageBox(_T("请先设置光源！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = Gouraud;
	m_isNeedInvalidate = true;
	m_isSetLight = true;
	Invalidate(true);


}


void CGraphView::OnPhong()
{
	if (m_isSetLight == false)
	{
		MessageBox(_T("请先设置光源！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = PhongShading;
	m_isNeedInvalidate = true;
	m_isSetLight = true;
	Invalidate(true);
}


void CGraphView::OnBlinn_Phong()
{
	// TODO: 在此添加命令处理程序代码
	if (m_isSetLight == false)
	{
		MessageBox(_T("请先设置光源！"), _T("提示"), MB_OK);
		return;
	}
	m_GraphMode = Blinn_PhongShading;
	m_isNeedInvalidate = true;
	m_isSetLight = true;
	Invalidate(true);
}


void CGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
}
