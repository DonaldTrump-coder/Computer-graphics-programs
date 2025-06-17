#pragma once
#include "afxdialogex.h"


// SetLightDiaglog 对话框

class SetLightDiaglog : public CDialogEx
{
	DECLARE_DYNAMIC(SetLightDiaglog)

public:
	SetLightDiaglog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetLightDiaglog();
	bool m_isSetLight = false;
	CString m_strX;  // 用于存储 IDC_EDIT_X 的文本内容
	CString m_strY;  // 用于存储 IDC_EDIT_Y 的文本内容
	CString m_strZ;  // 用于存储 IDC_EDIT_Z 的文本内容

	CString m_strI;  // 用于存储 IDC_EDIT_I 的文本内容

	CString m_strCX;  // 用于存储 IDC_EDIT_CX 的文本内容
	CString m_strCY;  // 用于存储 IDC_EDIT_CY 的文本内容
	CString m_strCZ;  // 用于存储 IDC_EDIT_CZ 的文本内容

	CString m_strR;  // 用于存储 IDC_EDIT_R 的文本内容

	COLORREF m_LightColor = RGB(0.1, 0.5, 0.7); //图形颜色,默认黑色
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetcolor();
	afx_msg void OnEnChangeEditX();
	virtual BOOL OnInitDialog();
	void GetData( float X, float Y, float Z, float I, float CX, float CY, float CZ, float R, COLORREF Color)
	{
		m_strX.Format(_T("%.2f"), X);
		this->SetDlgItemText(IDC_EDIT_X, m_strX);
		//SetDlgItemText(IDC_EDIT_X, m_strX);
		//m_strY.Format(_T("%.2f"), Y);
		//SetDlgItemText(IDC_EDIT_Y, m_strY);
		//m_strZ.Format(_T("%.2f"), Z);
		//SetDlgItemText(IDC_EDIT_Z, m_strZ);
		//m_strI.Format(_T("%.2f"), I);
		//SetDlgItemText(IDC_EDIT_I, m_strI);
		//m_strCX.Format(_T("%.2f"), CX);
		//SetDlgItemText(IDC_EDIT_CX, m_strCX);
		//m_strCY.Format(_T("%.2f"), CY);
		//SetDlgItemText(IDC_EDIT_CY, m_strCY);
		//m_strCZ.Format(_T("%.2f"), CZ);
		//SetDlgItemText(IDC_EDIT_CZ, m_strCZ);
		//m_strR.Format(_T("%.2f"), R);
		//SetDlgItemText(IDC_EDIT_R, m_strR);

		m_LightColor = Color;

	}

};
