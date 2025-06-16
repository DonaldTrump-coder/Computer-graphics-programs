#include "pch.h"
#include "POLYGON.h"

//调试使用
void printET(Edge** ET, int ET_num)
{
	Edge* p;
	for (int i = 0; i < ET_num; i++)
	{
		p = ET[i];
		printf("y=%d\t", p->Ymax);
		p = p->Next;
		while (p != nullptr)
		{
			printf("Ymax=%d,x=%.2f,dx=%.2f\t", p->Ymax, p->Xi, p->dX);
			p = p->Next;
		}
		printf("\n\n");
	}
	printf("\n\n\n");
}

//调试使用
void print_active(Edge* active)
{
	Edge* p = active;

	printf("active,y=%d\t", p->Ymax);
	p = p->Next;
	while (p != nullptr)
	{
		printf("Ymax=%d,x=%.2f,dx=%.2f\t", p->Ymax, p->Xi, p->dX);
		p = p->Next;
	}
	printf("\n\n");
}

//找到顶点中的y的最小值
int POLYGON::MinInY()
{
	int min_y = m_Nodes[0].y;
	for (int i = 0; i < m_NodeNum - 1; i++)
	{
		min_y = min_y <= m_Nodes[i + 1].y ? min_y : m_Nodes[i + 1].y;
	}
	return min_y;
}

//找到顶点中的y的最大值
int POLYGON::MaxInY()
{
	int min_y = m_Nodes[0].y;
	for (int i = 0; i < m_NodeNum - 1; i++)
	{
		min_y = min_y >= m_Nodes[i + 1].y ? min_y : m_Nodes[i + 1].y;
	}
	return min_y;
}

//构造函数
POLYGON::POLYGON()
{
	m_NodeNum = 0;
	m_Nodes = nullptr;
	m_fill_color = RGB(0, 0, 0);
	m_old_color = RGB(0, 0, 0);
}

//拷贝构造函数
POLYGON::POLYGON(POLYGON& copy)
{
	m_fill_color = copy.m_fill_color;
	m_old_color = copy.m_old_color;
	m_NodeNum = copy.m_NodeNum;
	m_Nodes = new CPoint[m_NodeNum];
	for (int i = 0; i < m_NodeNum; i++)
	{
		m_Nodes[i] = copy.m_Nodes[i];
	}
}

//析构函数
POLYGON::~POLYGON()
{
	//清空顶点数组
	if (m_Nodes != nullptr)
	{
		delete[] m_Nodes;
		m_Nodes = nullptr;
	}
	m_NodeNum = 0;
}

//增加顶点
void POLYGON::AddNode(CPoint& p)
{
	if (m_NodeNum == 0)
	{
		m_Nodes = new CPoint;
		m_Nodes[0] = p;
		m_NodeNum = 1;
		return;
	}
	CPoint* newNodes = new CPoint[m_NodeNum + 1];
	for (int i = 0; i < m_NodeNum; i++)
	{
		newNodes[i] = m_Nodes[i];
	}
	newNodes[m_NodeNum] = p;
	delete[] m_Nodes; //删除旧的数组，防止内存泄漏
	m_Nodes = newNodes;//新的数组
	m_NodeNum++;
}

//种子填充算法（递归方法，区域不能太大，否则会出现溢出）
void POLYGON::SeedFill4(CDC* pDC, CPoint initial_seed)
{
	COLORREF c = pDC->GetPixel(initial_seed.x, initial_seed.y);
	if (c == m_fill_color || c != m_old_color) return;
	pDC->SetPixel(initial_seed.x, initial_seed.y, m_fill_color);
	SeedFill4(pDC, CPoint(initial_seed.x, initial_seed.y + 1));
	SeedFill4(pDC, CPoint(initial_seed.x, initial_seed.y - 1));
	SeedFill4(pDC, CPoint(initial_seed.x - 1, initial_seed.y));
	SeedFill4(pDC, CPoint(initial_seed.x + 1, initial_seed.y));
}

//种子填充算法（堆栈方法）
void POLYGON::SeedFill4withStack(CDC* pDC, CPoint initial_seed)
{
	stack<CPoint> stk;
	stk.push(initial_seed); //第1步，种子点入站
	while (!stk.empty())
	{
		CPoint seed = stk.top(); //第2步，取当前种子点
		stk.pop();
		//第3步，填充
		COLORREF c = pDC->GetPixel(seed.x, seed.y);
		if (c == m_fill_color || c != m_old_color) continue;//如果已经填充新颜色或到达边界，跳过
		pDC->SetPixel(seed.x, seed.y, m_fill_color);
		stk.push(CPoint(seed.x, seed.y + 1));
		stk.push(CPoint(seed.x, seed.y - 1));
		stk.push(CPoint(seed.x - 1, seed.y));
		stk.push(CPoint(seed.x + 1, seed.y));
	}
}




//================================================================================================================// < 扫描线种子填充算法 >
//
// 算法的基本过程如下：
// 当给定种子点(x, y)时，
// 首先分别向左和向右两个方向填充种子点所在扫描线上的位于给定区域的一个区段，
// 同时记下这个区段的范围[xLeft, xRight]，
// 然后确定与这一区段相连通的上、下两条扫描线上位于给定区域内的区段，
// 并依次保存下来。反复这个过程，直到填充结束。
//
// 四个步骤实现算法：
// (1) 初始化一个空的栈用于存放种子点，将种子点(x, y)入栈；
// (2) 判断栈是否为空，如果栈为空则结束算法，否则取出栈顶元素作为当前扫描线的种子点(x, y)，y是当前的扫描线；
// (3) 从种子点(x, y)出发，沿当前扫描线向左、右两个方向填充，直到边界。分别标记区段的左、右端点坐标为xLeft和xRight；
// (4) 分别检查与当前扫描线相邻的y - 1和y + 1两条扫描线在区间[xLeft, xRight]中的像素，
// 从xLeft开始向xRight方向搜索，若存在非边界且未填充的像素点，则找出这些相邻的像素点中最右边的一个，并将其作为种子点压入栈中，
// 然后返回第（2）步；
//
//================================================================================================================
void POLYGON::ScanLineSeedFill(CDC* pDC, CPoint initial_seed)
{
	stack<CPoint> stk;
	stk.push(initial_seed); //第1步，种子点入站
	while (!stk.empty())
	{
		CPoint seed = stk.top(); //第2步，取当前种子点
		stk.pop();
		//第3步，向左右填充
		CPoint LeftSd(seed.x - 1, seed.y);
		int count = FillLine(pDC, -1, LeftSd);//向左填充
		int xLeft = seed.x - count;
		count = FillLine(pDC, 1, seed);//向右填充
		int xRight = seed.x + count - 1;
		//第4步，处理相邻两条扫描线
		SearchLineNewSeed(pDC, stk, xLeft, xRight, seed.y - 1);
		SearchLineNewSeed(pDC, stk, xLeft, xRight, seed.y + 1);
	}
}
//Direct = 1向右填充
//Direct = -1向左填充
//m_old_color is BoundaryColor
int POLYGON::FillLine(CDC* pDC, int Direct, CPoint seedPoint)
{
	//if (Direct != 1 && Direct != -1) return 0;
	//if (m_old_color == m_fill_color) return 0;
	//if (!pDC) return 0;

	int x = seedPoint.x;
	int y = seedPoint.y;
	int count = 0;
	COLORREF c = pDC->GetPixel(x, y);
	while (c == m_old_color)
	{
		pDC->SetPixel(x, y, m_fill_color);
		x += Direct;
		count++;
		c = pDC->GetPixel(x, y);
	}
	return count;
}

//m_old_color is BoundaryColor
void POLYGON::SearchLineNewSeed(CDC* pDC, stack<CPoint>& stk, int xLeft, int xRight, int y)
{
	int maxY = MaxInY();
	if (y <= 0) return;
	if (y > maxY) return;
	int xt = xLeft;
	bool findNewSeed = false;
	while (xt < xRight)
	{
		findNewSeed = false;
		//只要还有未填的，就地直往右找，找到最右的一个像素点
		COLORREF c = pDC->GetPixel(xt, y);
		while (c == m_old_color && c != m_fill_color && xt < xRight)
		{
			findNewSeed = true;
			xt++;
			c = pDC->GetPixel(xt, y);
		}
		if (findNewSeed)//找到了
		{
			if (c == m_old_color && c != m_fill_color && xt == xRight)
			{
				CPoint newP(xt, y);
				stk.push(newP);
			}
			else//边界退一格
			{
				CPoint newP(xt - 1, y);
				stk.push(newP);
			}
		}
		xt++;
	}
}

//=================================================
//将边插入边表,把新边表edge的边结点，用插入排序法
//插入活性边表ActiveEdge中，使之按X坐标递增顺序排序
//=================================================
void POLYGON::insertEdge(Edge* edge, Edge* ActiveHead)
{
	Edge* p, * q;
	q = ActiveHead;
	p = q->Next;
	while (p != nullptr)
	{
		if (edge->Xi < p->Xi) break;
		else 
		{ 
			q = p;
			p = q->Next; 
		}
	}
	edge->Next = q->Next;
	q->Next = edge;
}

//==========================================================================
//生成边表结点，并插入到边表中
//P为参考点（设置Ymax时使用）
//==========================================================================
void POLYGON::makeEdgeTable(CPoint Lower, CPoint Upper, Edge** ET)
{
	Edge* edge = new Edge;
	edge->dX = (float)(Upper.x - Lower.x) / (Upper.y - Lower.y);
	edge->Xi = Lower.x;
	edge->Ymax = Upper.y;
	
	insertEdge(edge, ET[Lower.y - ET[0]->Ymax]);
}
void POLYGON::CreatEdgeTable(Edge** ET)
{
	for (int i = 0; i < m_NodeNum; i++)
	{
		//边L
		CPoint P1 = m_Nodes[i];//当前点
		CPoint P2 = m_Nodes[(i + 1) % m_NodeNum];//当前点的后一点
		if (P2.y != P1.y) //不处理水平线
		{
			if (P2.y > P1.y)
				makeEdgeTable(P1, P2, ET);
			else
				makeEdgeTable(P2, P1, ET);
		}
	}
}

//========================================================================
//填充一对交点之间的像素
//========================================================================
void POLYGON::fillScan(CDC* pDC, int y, Edge* active)//填充一对交点
{
	Edge* p1, * p2;
	int i;
	p1 = active->Next;
	while (p1 != nullptr)
	{
		p2 = p1->Next;//第二个结点
		//if (p2 == nullptr) break;
		//填充一对交点之间的像素
		for (i = (int)p1->Xi; i <= p2->Xi; i++)
			pDC->SetPixel((int)i, y, m_fill_color);
		p1 = p2->Next;//下一对的第一个结点
	}
}

//==========================================================
//水平边直接画线填充
//==========================================================
void POLYGON::HorizonEdgeFill(CDC* pDC)
{
	for (int i = 0; i < m_NodeNum; i++)
	{
		//边L
		CPoint P1, P2;
		P1.x = m_Nodes[i].x;//当前点
		P1.y = m_Nodes[i].y;//当前点
		P2.x = m_Nodes[(i + 1) % m_NodeNum].x;//当前点的后一点
		P2.y = m_Nodes[(i + 1) % m_NodeNum].y;//当前点的后一点
		if (P2.y == P1.y) //如果是水平线
		{
			for (int j = min(P1.x, P2.x); j <= max(P1.x, P2.x); j++)//填充一对交点之间的像素
				pDC->SetPixel(j, P1.y, m_fill_color);
		}
	}
}

//=========================================================================================
//scan为扫描线的编号，ET为边表，active为生成的新活动表
//=========================================================================================
void POLYGON::RefreshActiveList(int scan, Edge** ET, Edge* active)//填充完后，更新活动边表
{
	int y0 = ET[scan]->Ymax + 1;//此处ET[scan].Ymax是实际的扫描线的值：y=y0
	Edge* p = active->Next;
	Edge* q = active;
	while (p != nullptr)//如果当前扫描线 scan的值大于或等于p所指向的边的ymax，则删除该边
	{
		if (y0 == p->Ymax)
		{
			q->Next = p->Next;
			delete p;
			p = q->Next;
		}
		else
		{
			p->Xi = p->Xi + p->dX;
			q = p;
			p = q->Next;
		}
	}

	
	//把新边加入到活动表中
	q = active->Next;
	active->Next = nullptr;
	active = ET[scan + 1];
	while (q != nullptr)
	{
		p = q->Next;
		insertEdge(q, active);
		q = p;
	}
}


//======================================================
//扫描线多边形填充算法
//======================================================
void POLYGON::ScanLineFill(CDC* pDC)
{
	int ymin = MinInY();
	int ymax = MaxInY();

	//根据扫描线的数目建立边表，ET[i].Ymax中放置了实际的扫描线值，i是相对值
	int ET_num = ymax - ymin + 1;
	Edge** ET = new Edge*[ET_num];
	
	for (int i = 0; i < ET_num; i++)
	{
		ET[i] = new Edge;
		ET[i]->Ymax = i + ymin; 
	}

	CreatEdgeTable(ET);

	HorizonEdgeFill(pDC); //水平边直接画线填充
	Edge* active;
	for (int scan = 0; scan < ET_num - 1; scan++)
	{
		active = ET[scan];
		fillScan(pDC, scan + ymin, active);
		RefreshActiveList(scan, ET, active);
	}

	//删除边表，防止内存泄漏
	Edge* q, * p;
	for (int i = 0; i < ET_num; i++)
	{
		q = ET[i]->Next;
		delete ET[i];
		while (q != nullptr)
		{
			p = q->Next;
			delete q;
			q = p;
		}
		
	}
	delete[] ET;
}

void POLYGON::DrawPolygon(CDC* pDC, COLORREF color)
{
	if (m_NodeNum == 0 || m_Nodes == nullptr)
	{
		return;
	}
	for (int i = 0; i < m_NodeNum - 1; i++)
	{
		LINE line(m_Nodes[i], m_Nodes[i + 1], color);
		line.Bresenham2(pDC);
	}
	LINE line(m_Nodes[m_NodeNum - 1], m_Nodes[0], color);
	line.Bresenham2(pDC);
}
