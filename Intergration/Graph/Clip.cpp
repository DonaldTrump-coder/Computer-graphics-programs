#include "pch.h"
#include "Clip.h"
#include <math.h>


//////////////������ֱ�߲ü��㷨/////////////

int LINE_CLIP::Encode(CPoint p, int XL, int XR, int YB, int YT)
{
    int LEFT = 0x1;
    int RIGHT = 0x2;
    int BOTTOM = 0x4;
    int TOP = 0x8;

    int code = 0x00;

    if (p.x < XL)
        code |= LEFT;
    if (p.x > XR)
        code |= RIGHT;
    if (p.y < YB)
        code |= BOTTOM;
    if (p.y > YT)
        code |= TOP;

    return code;
}


//���캯��
LINE_CLIP::LINE_CLIP()
{
    m_Begin = CPoint(0, 0);
    m_End = CPoint(0, 0);
    m_color = RGB(0, 0, 0);
}

//���ع��캯��
LINE_CLIP::LINE_CLIP(CPoint Begin, CPoint End, COLORREF color)
{
    m_Begin = Begin;
    m_End = End;
    m_color = color;
}

//SouthLand�㷨�ü�����  
bool LINE_CLIP::LineClip(int XL, int XR, int YB, int YT, LINE& line1)
{
    int LEFT = 0x1;
    int RIGHT = 0x2;
    int BOTTOM = 0x4;
    int TOP = 0x8;

    int code1, code2;    //���������˵��������
    int code;
    float x = 0.0f, y = 0.0f;
    float k = 0.0f;
    //б��
    CPoint GP1 = m_Begin;//�˵�1
    CPoint GP2 = m_End;//�˵�2

    int isPointInWindow = 0; //�ж�ֱ���봰�ڵĽ����Ƿ��ڴ��ڷ�Χ��

    code1 = Encode(GP1, XL, XR, YB, YT);
    code2 = Encode(GP2, XL, XR, YB, YT);

    //��ֱ�������ڴ����������ȫ�ڴ�����,��˵��ֱ�߲���Ҫ�ü�������false
    if (((code1 & code2) != 0) || (code1 == 0 && code2 == 0))
    {
        return false;
    }

    while (code1 != 0 || code2 != 0)         //��ǰֱ������ȫ����  
    {
        if ((code1 & code2) != 0) //��ֹ����
        {
            if (GP1 == GP2) //���ֱ��ֻ���봰���ཻ��һ��
            {
                return false;
            }
            return true;
        }

        if (code1 != 0) code = code1; 
        else code = code2;//��lineBegin�˵��ڲü������򽻻������˵�ʹ���ڲü�����  

        //����б��  
        if (GP1.x != GP2.x)
            k = (GP2.y - GP1.y) * 1.0f / (GP2.x - GP1.x);
        //��ʼ�ü�,���������������Ϊ�棬  
        //��lineBegin�ڱ߽��⣬��ʱ��lineBegin����ֱ����ñ߽�Ľ���  
        if ((code & LEFT) != 0)
        {
            x = XL;
            y = GP1.y + k * (XL - GP1.x);

            if (y >= YB && y <= YT)
            {
                isPointInWindow |= LEFT;
            }
        }
        else if ((code & RIGHT) != 0)
        {
            x = XR;
            y = GP1.y + k * (XR - GP1.x);

            if (y >= YB && y <= YT)
            {
                isPointInWindow |= RIGHT;
            }
        }
        else if ((code & TOP) != 0)
        {
            y = YT;
            x = GP1.x + (YT - GP1.y) / k;

            if (x >= XL && x <= XR)
            {
                isPointInWindow |= TOP;
            }
        }
        else if ((code & BOTTOM) != 0)
        {
            y = YB;
            x = GP1.x + (YB - GP1.y) / k;

            if (x >= XL && x <= XR)
            {
                isPointInWindow |= BOTTOM;
            }
        }


        if (isPointInWindow == 0) //���ֱ���봰��û�н���
        {
            return false;
        }

        if (code == code1)
        {
            GP1 = CPoint((int)x, (int)y);
            code1 = Encode(GP1, XL, XR, YB, YT);
        }
        else
        {
            GP2 = CPoint((int)x, (int)y);
            code2 = Encode(GP2, XL, XR, YB, YT);
        }
    }
    line1 = LINE(GP1, GP2, m_color);//���»�ֱ��

    return true;
}

//�е��㷨
bool LINE_CLIP::LineMidClip(int XL, int XR, int YB, int YT, LINE& L)
{
    LINE_CLIP L1(m_Begin, m_End, m_color);
    LINE_CLIP L2(m_End, m_Begin, m_color);

    CPoint GP1 = L1.MidClipPoint(XL, XR, YB, YT);
    CPoint GP2 = L2.MidClipPoint(XL, XR, YB, YT);

    if (GP1 == GP2) //�����ֱ���봰��ֻ��һ��
    {
        return false;
    }

    L = LINE(GP1, GP2, m_color);

    return true;

}
//==========================================================================
//�е�ü��㷨���󽻵��㷨
//���ܣ�����߶Σ�begin,end���ڲü�������end����ĵ�һ���ɼ���
//==========================================================================
CPoint LINE_CLIP::MidClipPoint(int XL, int XR, int YB, int YT)
{
    CPoint mid;
    CPoint temp;
    if (IsInArea(m_Begin, XL, XR, YB, YT)) temp = m_Begin;//���begin�ڲü�����
    else if (NotIntersect(XL, XR, YB, YT)) temp = m_Begin;//�߶Σ��˵�ֱ�Ϊbegin,end�����ڲü�����
    else
    {
        mid.x = (m_Begin.x + m_End.x) / 2; mid.y = (m_Begin.y + m_End.y) / 2;//ȡ�߶Σ��˵�ֱ�Ϊbegin,end�����е�
        if (abs(mid.x - m_End.x) <= 1 && abs(mid.y - m_End.y) <= 1) temp = mid;//����е�ӽ�end
        else//�е㲻�ӽ�end
        {
            //�߶Σ�begin,mid����ȫ���ڲü����ڣ������߶Σ�mid,end�����������߶Σ�begin,mid��

            LINE_CLIP temLine1(m_Begin, mid, m_color);
            LINE_CLIP temLine2(mid, m_End, m_color);

            if (temLine1.NotIntersect(XL, XR, YB, YT))
                temp = temLine2.MidClipPoint(XL, XR, YB, YT);
            else
                temp = temLine1.MidClipPoint(XL, XR, YB, YT);
        }
    }
    return temp;
}

//MidClip�ĸ�������:�б�P���Ƿ��ڲü�����
bool LINE_CLIP::IsInArea(CPoint P, int XL, int XR, int YB, int YT)
{
    if (P.x >= XL && P.x <= XR && P.y >= YB && P.y <= YT) return true;
    else return false;
}
//MidClip�ĸ�������:�б��߶Σ��˵�ֱ�Ϊbegin,edn���Ƿ��ڲü�����
bool LINE_CLIP::NotIntersect(int XL, int XR, int YB, int YT)
{
    int maxx, maxy, minx, miny;
    maxx = (m_Begin.x > m_End.x) ? m_Begin.x : m_End.x;
    minx = (m_Begin.x < m_End.x) ? m_Begin.x : m_End.x;
    maxy = (m_Begin.y > m_End.y) ? m_Begin.y : m_End.y;
    miny = (m_Begin.y < m_End.y) ? m_Begin.y : m_End.y;
    if (maxx < XL || minx > XR || maxy < YB || miny > YT) return true;
    else return false;
}


//�����ж���Liang-Barskyֱ�߲ü��㷨
//=============================================================================================
bool LINE_CLIP::LBClip(int XL, int XR, int YB, int YT, LINE& line)
{
    CPoint P1 = m_Begin; CPoint P2 = m_End;
    int dx = P2.x - P1.x, dy = P2.y - P1.y;
    line = LINE(P1, P2, m_color);

    float u1 = 0, u2 = 1.0f;



    if (ClipT(-dx, P1.x - XL, u1, u2))
        if (ClipT(dx, XR - P1.x, u1, u2))
            if (ClipT(-dy, P1.y - YB, u1, u2))
                if (ClipT(dy, YT - P1.y, u1, u2))
                {
                    int x1 = P1.x;
                    int y1 = P1.y;
                    if (u1 > 0)
                        P1 = CPoint((int)(x1 + u1 * dx), (int)(y1 + u1 * dy));
                    if (u2 < 1)
                        P2 = CPoint((int)(x1 + u2 * dx), (int)(y1 + u2 * dy));

                    line = LINE(P1, P2, m_color);
                    return true;
                }

    return false;
}
bool LINE_CLIP::ClipT(int p, int q, float& u1, float& u2)
{
    float r;
    if (p < 0)
    {
        r = (float)q / p;
        if (r > u2) return false;
        else if (r > u1)
        {
            u1 = r;
            return true;
        }
    }

    else if (p > 0)
    {
        r = (float)q / p;
        if (r < u1) return false;
        else if (r < u2)
        {
            u2 = r;
            return true;
        }
    }

    else if (q < 0) return false;
    return true;
}





//////////////�����Ƕ���βü��㷨/////////////


//���캯��
POLYGON_CLIP::POLYGON_CLIP()
{
    m_NodeNum = 0;
    m_Nodes = nullptr;
    m_fill_color = RGB(0, 0, 0);
    m_old_color = RGB(0, 0, 0);
}

//�������캯��
POLYGON_CLIP::POLYGON_CLIP(POLYGON_CLIP& copy)
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
POLYGON_CLIP::~POLYGON_CLIP()
{
    //��ն�������
    if (m_Nodes != nullptr)
    {
        delete[] m_Nodes;
        m_Nodes = nullptr;
    }
    m_NodeNum = 0;
}

//isVisible����
//�ж�P�Ƿ������ü��Ĵ���ClipEdge�Ŀɼ��࣬����ϵ����Windows�Ĵ�������ϵ
//�ü������ε��ĸ����㰴��˳ʱ���˳����
//==================================================================
bool POLYGON_CLIP::isVisible(CPoint& P, LINE& ClipEdge)
{
    if (ClipEdge.m_End.x > ClipEdge.m_Begin.x)//�ϱ�
        if (P.y >= ClipEdge.m_Begin.y) return true; else return false;

    if (ClipEdge.m_End.x < ClipEdge.m_Begin.x)//�±�
        if (P.y <= ClipEdge.m_Begin.y) return true; else return false;

    if (ClipEdge.m_End.y > ClipEdge.m_Begin.y)//�ұ�
        if (P.x <= ClipEdge.m_Begin.x) return true; else return false;

    if (ClipEdge.m_End.y < ClipEdge.m_Begin.y)//���
        if (P.x >= ClipEdge.m_Begin.x) return true; else return false;

    return false;
}
//�����εı�PolygonEdge�����ü��Ĵ���ClipEdge�Ľ���
CPoint POLYGON_CLIP::Intersect(LINE& PolygonEdge, LINE& ClipEdge)
{
    CPoint P;
    if (ClipEdge.m_End.x == ClipEdge.m_Begin.x)//��ֱ��
    {
        P.x = ClipEdge.m_End.x;
        double y = PolygonEdge.m_Begin.y + (P.x - PolygonEdge.m_Begin.x) * (PolygonEdge.m_End.y - PolygonEdge.m_Begin.y) * 1.0f / (PolygonEdge.m_End.x - PolygonEdge.m_Begin.x);
        P.y = (int)y;
    }
    if (ClipEdge.m_End.y == ClipEdge.m_Begin.y)//ˮƽ��
    {
        P.y = ClipEdge.m_End.y;
        double x = PolygonEdge.m_Begin.x + (P.y - PolygonEdge.m_Begin.y) * (PolygonEdge.m_End.x - PolygonEdge.m_Begin.x) * 1.0f / (PolygonEdge.m_End.y - PolygonEdge.m_Begin.y);
        P.x = (int)x;
    }

    return P;
}
//����ε������˳ʱ�뷽��
//=======================================================================
void POLYGON_CLIP::SingleEdgeClip(LINE& ClipEdge, POLYGON_CLIP& polygon_out)
{
    CPoint S = (m_Nodes[m_NodeNum - 1]); 
    for (int i = 0; i < m_NodeNum; i++)
    {
        CPoint P(m_Nodes[i]);
        if (isVisible(P, ClipEdge) == true)//P�ڴ�����
        {
            if (isVisible(S, ClipEdge) == true)
            {
                polygon_out.AddNode(P);//����1��S��P���ڴ�����
            }
            else//����4��S�ڴ����⣬P�ڴ�����
            {

                LINE PolygonEdge(S, P, m_old_color);//�������εı�SP
                CPoint IP = Intersect(PolygonEdge, ClipEdge);//��������ߵĽ���
                polygon_out.AddNode(IP);
                polygon_out.AddNode(P);
            }
        } 
        else //P�ڴ�����
        {
            if (isVisible(S, ClipEdge) == true)//����3��P�ڴ����⣬S�ڴ�����
            {
                LINE PolygonEdge(S, P, m_old_color);//�������εı�SP
                CPoint IP = Intersect(PolygonEdge, ClipEdge);//��������ߵĽ���
                polygon_out.AddNode(IP);
            }
            else //����2��S��P���ڴ�����
            {
                //�����
            }
        }
        

        S = P;
    }
    return;
}

//����Sutherland-Hodgman�㷨�У����ڱߵ�һЩ��������
//==================================================
void POLYGON_CLIP::SHCheck(int XL, int XR, int YB, int YT)
{
    int snL = 0, snR = 0, snB = 0, snT = 0;
    for (int i = 0; i < m_NodeNum; i++)
    {
        if (m_Nodes[i].x == XL) snL++;
        if (m_Nodes[i].x == XR) snR++;
        if (m_Nodes[i].y == YB) snB++;
        if (m_Nodes[i].y == YT) snT++;
    }
    for (int i = 0; i < m_NodeNum; i++)
    {
        if (snL > 2 && m_Nodes[i].x == XL) m_Nodes[i].x = -XL;
        if (snR > 2 && m_Nodes[i].x == XR) m_Nodes[i].x = -XR;
        if (snB > 2 && m_Nodes[i].y == YB) m_Nodes[i].y = -YB;
        if (snT > 2 && m_Nodes[i].y == YT) m_Nodes[i].y = -YT;
    }
}
//����Sutherland-Hodgman�㷨�У����ڱߵ�һЩ�������λ������
//===================================================================================
void POLYGON_CLIP::SHDrawPolygon(CDC* pDC, int XL, int XR, int YB, int YT)
{
    POLYGON_CLIP PL, PR, PB,  PT;
    LINE PolygonEdge;
    for (int i = 0; i < m_NodeNum; i++)
    {
        CPoint S(m_Nodes[i]);
        if (S.x == XL) PL.AddNode(S);
        if (S.x == XR) PR.AddNode(S);
        if (S.y == YB) PB.AddNode(S);
        if (S.y == YT) PT.AddNode(S);

        CPoint P(m_Nodes[(i + 1) % m_NodeNum]);//��֤�����β�������γɷ��ͼ��
        if ((S.x == XL && P.x == XL) || (S.x == XR && P.x == XR) || (S.y == YB && P.y == YB) || (S.y == YT && P.y == YT)) continue;//�ü����ڱ߽��ϵĵ���ʱ������

        PolygonEdge.m_Begin = S;
        PolygonEdge.m_End = P;
        PolygonEdge.m_color = m_old_color;
        PolygonEdge.Bresenham2(pDC);
        S = P;
        //S.SetPoint(P);
    }
    //�Զ�������
    PL.SortInY();
    PR.SortInY();
    PB.SortInX();
    PT.SortInX();


    //��������ߣ������Winows���ڣ�
    for (int i = 0; i < PL.m_NodeNum - 1; i += 2)
    {
        CPoint S(PL.m_Nodes[i]);
        CPoint P(PL.m_Nodes[i + 1]);
        PolygonEdge.m_Begin = S;
        PolygonEdge.m_End = P;
        PolygonEdge.m_color = m_old_color;
        PolygonEdge.Bresenham2(pDC);
    }
    //�������ұ�
    for (int i = 0; i < PR.m_NodeNum - 1; i += 2)
    {
        CPoint S(PR.m_Nodes[i]);
        CPoint P(PR.m_Nodes[i + 1]);
        PolygonEdge.m_Begin = S;
        PolygonEdge.m_End = P;
        PolygonEdge.m_color = m_old_color;
        PolygonEdge.Bresenham2(pDC);
    }
    //�������ϱ�
    for (int i = 0; i < PB.m_NodeNum - 1; i += 2)
    {
        CPoint S(PB.m_Nodes[i]);
        CPoint P(PB.m_Nodes[i + 1]);
        PolygonEdge.m_Begin = S;
        PolygonEdge.m_End = P;
        PolygonEdge.m_color = m_old_color;
        PolygonEdge.Bresenham2(pDC);
    }
    //�������±�
    for (int i = 0; i < PT.m_NodeNum - 1; i += 2)
    {
        CPoint S(PT.m_Nodes[i]);
        CPoint P(PT.m_Nodes[i + 1]);
        PolygonEdge.m_Begin = S;
        PolygonEdge.m_End = P;
        PolygonEdge.m_color = m_old_color;
        PolygonEdge.Bresenham2(pDC);
    }
}
//����X����
//====================
void POLYGON_CLIP::SortInX()
{
    for (int i = 1; i < m_NodeNum; i++)
    {
        CPoint temp;
        temp = m_Nodes[i];
        int j = i - 1;
        while (j >= 0 && temp.x < m_Nodes[j].x)
        {
            m_Nodes[j + 1] = m_Nodes[j];
            j--;
        }
        m_Nodes[j + 1] = temp;
    }
}
//����Y����
//====================
void POLYGON_CLIP::SortInY()
{
    for (int i = 1; i < m_NodeNum; i++)
    {
        CPoint temp(m_Nodes[i]);
        int j = i - 1;
        while (j >= 0 && temp.y < m_Nodes[j].y)
        {
            m_Nodes[j + 1] = m_Nodes[j];
            j--;
        }
        m_Nodes[j + 1] = temp;
    }
}


//�ж��Ƿ���Ҫ�ü�
bool POLYGON_CLIP::IsPolygonNeedClip(int XL, int XR, int YB, int YT)
{
    bool isLineNeedClip = false;
    for (int i = 0; i < m_NodeNum - 1; i++)
    {
        LINE line;
        LINE_CLIP line_clip(m_Nodes[i], m_Nodes[i + 1], RGB(0, 0, 0));
        isLineNeedClip = line_clip.LBClip(XL, XR, YB, YT, line);
        if (isLineNeedClip == true)
        {
            break;
        }
    }
    return isLineNeedClip;
}

//���������
void POLYGON_CLIP::operator=(POLYGON_CLIP& A)
{
    //��ն�������
    if (m_Nodes != nullptr)
    {
        delete[] m_Nodes;
        m_Nodes = nullptr;
    }
    m_NodeNum = 0;

    m_fill_color = A.m_fill_color;
    m_old_color = A.m_old_color;
    m_NodeNum = A.m_NodeNum;
    m_Nodes = new CPoint[m_NodeNum];
    for (int i = 0; i < m_NodeNum; i++)
    {
        m_Nodes[i] = A.m_Nodes[i];
    }
}

// ���ݶ���ε�ֵ���ö���βü���ֵ
void POLYGON_CLIP::SetPolygonClip(POLYGON& polygon)
{
    m_fill_color = polygon.m_fill_color;
    m_old_color = polygon.m_old_color;
    m_NodeNum = polygon.m_NodeNum;
    m_Nodes = new CPoint[m_NodeNum];
    for (int i = 0; i < m_NodeNum; i++)
    {
        m_Nodes[i] = polygon.m_Nodes[i];
    }
}

//=================================================================
//Sutherland-Hodgman�㷨
//����ε������˳ʱ�뷽��
//=================================================================
bool POLYGON_CLIP::Surtherland_Hondgman(int XL, int XR, int YB, int YT, POLYGON_CLIP& polygon_out)
{
    if (!IsPolygonNeedClip(XL, XR, YB, YT))
    {
        return false;
    }
    CPoint P1(XL, YB);
    CPoint P2(XR, YB);
    LINE ClipEdge(P1, P2, m_old_color);
    POLYGON_CLIP PG1;
    SingleEdgeClip(ClipEdge, PG1);

    P1.SetPoint(XR, YB); P2.SetPoint(XR, YT);
    ClipEdge.m_Begin = P1;
    ClipEdge.m_End = P2;
    POLYGON_CLIP PG2;
    PG1.SingleEdgeClip(ClipEdge, PG2);

    P1.SetPoint(XR, YT); P2.SetPoint(XL, YT);
    ClipEdge.m_Begin = P1;
    ClipEdge.m_End = P2;
    POLYGON_CLIP PG3;
    PG2.SingleEdgeClip(ClipEdge, PG3);

    P1.SetPoint(XL, YT); P2.SetPoint(XL, YB);
    ClipEdge.m_Begin = P1;
    ClipEdge.m_End = P2;
    PG3.SingleEdgeClip(ClipEdge, polygon_out);

    return true;
}