#include "pch.h"
#include "POLYGON.h"

//����ʹ��
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

//����ʹ��
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

//�ҵ������е�y����Сֵ
int POLYGON::MinInY()
{
	int min_y = m_Nodes[0].y;
	for (int i = 0; i < m_NodeNum - 1; i++)
	{
		min_y = min_y <= m_Nodes[i + 1].y ? min_y : m_Nodes[i + 1].y;
	}
	return min_y;
}

//�ҵ������е�y�����ֵ
int POLYGON::MaxInY()
{
	int min_y = m_Nodes[0].y;
	for (int i = 0; i < m_NodeNum - 1; i++)
	{
		min_y = min_y >= m_Nodes[i + 1].y ? min_y : m_Nodes[i + 1].y;
	}
	return min_y;
}

//���캯��
POLYGON::POLYGON()
{
	m_NodeNum = 0;
	m_Nodes = nullptr;
	m_fill_color = RGB(0, 0, 0);
	m_old_color = RGB(0, 0, 0);
}

//�������캯��
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

//��������
POLYGON::~POLYGON()
{
	//��ն�������
	if (m_Nodes != nullptr)
	{
		delete[] m_Nodes;
		m_Nodes = nullptr;
	}
	m_NodeNum = 0;
}

//���Ӷ���
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
	delete[] m_Nodes; //ɾ���ɵ����飬��ֹ�ڴ�й©
	m_Nodes = newNodes;//�µ�����
	m_NodeNum++;
}

//��������㷨���ݹ鷽����������̫�󣬷������������
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

//��������㷨����ջ������
void POLYGON::SeedFill4withStack(CDC* pDC, CPoint initial_seed)
{
	stack<CPoint> stk;
	stk.push(initial_seed); //��1�������ӵ���վ
	while (!stk.empty())
	{
		CPoint seed = stk.top(); //��2����ȡ��ǰ���ӵ�
		stk.pop();
		//��3�������
		COLORREF c = pDC->GetPixel(seed.x, seed.y);
		if (c == m_fill_color || c != m_old_color) continue;//����Ѿ��������ɫ�򵽴�߽磬����
		pDC->SetPixel(seed.x, seed.y, m_fill_color);
		stk.push(CPoint(seed.x, seed.y + 1));
		stk.push(CPoint(seed.x, seed.y - 1));
		stk.push(CPoint(seed.x - 1, seed.y));
		stk.push(CPoint(seed.x + 1, seed.y));
	}
}




//================================================================================================================// < ɨ������������㷨 >
//
// �㷨�Ļ����������£�
// ���������ӵ�(x, y)ʱ��
// ���ȷֱ������������������������ӵ�����ɨ�����ϵ�λ�ڸ��������һ�����Σ�
// ͬʱ����������εķ�Χ[xLeft, xRight]��
// Ȼ��ȷ������һ��������ͨ���ϡ�������ɨ������λ�ڸ��������ڵ����Σ�
// �����α�������������������̣�ֱ����������
//
// �ĸ�����ʵ���㷨��
// (1) ��ʼ��һ���յ�ջ���ڴ�����ӵ㣬�����ӵ�(x, y)��ջ��
// (2) �ж�ջ�Ƿ�Ϊ�գ����ջΪ��������㷨������ȡ��ջ��Ԫ����Ϊ��ǰɨ���ߵ����ӵ�(x, y)��y�ǵ�ǰ��ɨ���ߣ�
// (3) �����ӵ�(x, y)�������ص�ǰɨ��������������������䣬ֱ���߽硣�ֱ������ε����Ҷ˵�����ΪxLeft��xRight��
// (4) �ֱ����뵱ǰɨ�������ڵ�y - 1��y + 1����ɨ����������[xLeft, xRight]�е����أ�
// ��xLeft��ʼ��xRight���������������ڷǱ߽���δ�������ص㣬���ҳ���Щ���ڵ����ص������ұߵ�һ������������Ϊ���ӵ�ѹ��ջ�У�
// Ȼ�󷵻صڣ�2������
//
//================================================================================================================
void POLYGON::ScanLineSeedFill(CDC* pDC, CPoint initial_seed)
{
	stack<CPoint> stk;
	stk.push(initial_seed); //��1�������ӵ���վ
	while (!stk.empty())
	{
		CPoint seed = stk.top(); //��2����ȡ��ǰ���ӵ�
		stk.pop();
		//��3�������������
		CPoint LeftSd(seed.x - 1, seed.y);
		int count = FillLine(pDC, -1, LeftSd);//�������
		int xLeft = seed.x - count;
		count = FillLine(pDC, 1, seed);//�������
		int xRight = seed.x + count - 1;
		//��4����������������ɨ����
		SearchLineNewSeed(pDC, stk, xLeft, xRight, seed.y - 1);
		SearchLineNewSeed(pDC, stk, xLeft, xRight, seed.y + 1);
	}
}
//Direct = 1�������
//Direct = -1�������
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
		//ֻҪ����δ��ģ��͵�ֱ�����ң��ҵ����ҵ�һ�����ص�
		COLORREF c = pDC->GetPixel(xt, y);
		while (c == m_old_color && c != m_fill_color && xt < xRight)
		{
			findNewSeed = true;
			xt++;
			c = pDC->GetPixel(xt, y);
		}
		if (findNewSeed)//�ҵ���
		{
			if (c == m_old_color && c != m_fill_color && xt == xRight)
			{
				CPoint newP(xt, y);
				stk.push(newP);
			}
			else//�߽���һ��
			{
				CPoint newP(xt - 1, y);
				stk.push(newP);
			}
		}
		xt++;
	}
}

//=================================================
//���߲���߱�,���±߱�edge�ı߽�㣬�ò�������
//������Ա߱�ActiveEdge�У�ʹ֮��X�������˳������
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
//���ɱ߱��㣬�����뵽�߱���
//PΪ�ο��㣨����Ymaxʱʹ�ã�
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
		//��L
		CPoint P1 = m_Nodes[i];//��ǰ��
		CPoint P2 = m_Nodes[(i + 1) % m_NodeNum];//��ǰ��ĺ�һ��
		if (P2.y != P1.y) //������ˮƽ��
		{
			if (P2.y > P1.y)
				makeEdgeTable(P1, P2, ET);
			else
				makeEdgeTable(P2, P1, ET);
		}
	}
}

//========================================================================
//���һ�Խ���֮�������
//========================================================================
void POLYGON::fillScan(CDC* pDC, int y, Edge* active)//���һ�Խ���
{
	Edge* p1, * p2;
	int i;
	p1 = active->Next;
	while (p1 != nullptr)
	{
		p2 = p1->Next;//�ڶ������
		//if (p2 == nullptr) break;
		//���һ�Խ���֮�������
		for (i = (int)p1->Xi; i <= p2->Xi; i++)
			pDC->SetPixel((int)i, y, m_fill_color);
		p1 = p2->Next;//��һ�Եĵ�һ�����
	}
}

//==========================================================
//ˮƽ��ֱ�ӻ������
//==========================================================
void POLYGON::HorizonEdgeFill(CDC* pDC)
{
	for (int i = 0; i < m_NodeNum; i++)
	{
		//��L
		CPoint P1, P2;
		P1.x = m_Nodes[i].x;//��ǰ��
		P1.y = m_Nodes[i].y;//��ǰ��
		P2.x = m_Nodes[(i + 1) % m_NodeNum].x;//��ǰ��ĺ�һ��
		P2.y = m_Nodes[(i + 1) % m_NodeNum].y;//��ǰ��ĺ�һ��
		if (P2.y == P1.y) //�����ˮƽ��
		{
			for (int j = min(P1.x, P2.x); j <= max(P1.x, P2.x); j++)//���һ�Խ���֮�������
				pDC->SetPixel(j, P1.y, m_fill_color);
		}
	}
}

//=========================================================================================
//scanΪɨ���ߵı�ţ�ETΪ�߱�activeΪ���ɵ��»��
//=========================================================================================
void POLYGON::RefreshActiveList(int scan, Edge** ET, Edge* active)//�����󣬸��»�߱�
{
	int y0 = ET[scan]->Ymax + 1;//�˴�ET[scan].Ymax��ʵ�ʵ�ɨ���ߵ�ֵ��y=y0
	Edge* p = active->Next;
	Edge* q = active;
	while (p != nullptr)//�����ǰɨ���� scan��ֵ���ڻ����p��ָ��ıߵ�ymax����ɾ���ñ�
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

	
	//���±߼��뵽�����
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
//ɨ���߶��������㷨
//======================================================
void POLYGON::ScanLineFill(CDC* pDC)
{
	int ymin = MinInY();
	int ymax = MaxInY();

	//����ɨ���ߵ���Ŀ�����߱�ET[i].Ymax�з�����ʵ�ʵ�ɨ����ֵ��i�����ֵ
	int ET_num = ymax - ymin + 1;
	Edge** ET = new Edge*[ET_num];
	
	for (int i = 0; i < ET_num; i++)
	{
		ET[i] = new Edge;
		ET[i]->Ymax = i + ymin; 
	}

	CreatEdgeTable(ET);

	HorizonEdgeFill(pDC); //ˮƽ��ֱ�ӻ������
	Edge* active;
	for (int scan = 0; scan < ET_num - 1; scan++)
	{
		active = ET[scan];
		fillScan(pDC, scan + ymin, active);
		RefreshActiveList(scan, ET, active);
	}

	//ɾ���߱���ֹ�ڴ�й©
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
