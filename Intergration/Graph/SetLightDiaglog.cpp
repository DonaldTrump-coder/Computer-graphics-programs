// SetLightDiaglog.cpp: 实现文件
//

#include "pch.h"
#include "Graph.h"
#include "afxdialogex.h"
#include "SetLightDiaglog.h"


// SetLightDiaglog 对话框

IMPLEMENT_DYNAMIC(SetLightDiaglog, CDialogEx)

SetLightDiaglog::SetLightDiaglog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SetLightDiaglog::~SetLightDiaglog()
{
}

void SetLightDiaglog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_strX);
	DDX_Text(pDX, IDC_EDIT_Y, m_strY);
	DDX_Text(pDX, IDC_EDIT_Z, m_strZ);
	DDX_Text(pDX, IDC_EDIT_I, m_strI);
	DDX_Text(pDX, IDC_EDIT_CX, m_strCX);
	DDX_Text(pDX, IDC_EDIT_CY, m_strCY);
	DDX_Text(pDX, IDC_EDIT_CZ, m_strCZ);
	DDX_Text(pDX, IDC_EDIT_R, m_strR);
}


BEGIN_MESSAGE_MAP(SetLightDiaglog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SetColor, &SetLightDiaglog::OnBnClickedButtonSetcolor)
	ON_EN_CHANGE(IDC_EDIT_X, &SetLightDiaglog::OnEnChangeEditX)
END_MESSAGE_MAP()


// SetLightDiaglog 消息处理程序


void SetLightDiaglog::OnBnClickedButtonSetcolor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dig;	//构造一个颜色选择对话框

	dig.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT; // CC_RGBINIT可以让上次选择的颜色作为初始颜色显示出来
	dig.m_cc.rgbResult = m_LightColor;	//保存前一个选择的色素

	if (IDOK == dig.DoModal())	//判断是否选中了色素(按下OK按钮)
	{
		m_LightColor = dig.m_cc.rgbResult;	//获取当前在颜色对话框中选择的色素
	}
}


void SetLightDiaglog::OnEnChangeEditX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


BOOL SetLightDiaglog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_isSetLight == true)
	{
		GetDlgItem(IDC_EDIT_X)->SetWindowText(m_strX);
		GetDlgItem(IDC_EDIT_Y)->SetWindowText(m_strY);
		GetDlgItem(IDC_EDIT_Z)->SetWindowText(m_strZ);

		GetDlgItem(IDC_EDIT_CX)->SetWindowText(m_strCX);
		GetDlgItem(IDC_EDIT_CY)->SetWindowText(m_strCY);
		GetDlgItem(IDC_EDIT_CZ)->SetWindowText(m_strCZ);

		GetDlgItem(IDC_EDIT_R)->SetWindowText(m_strR);

		GetDlgItem(IDC_EDIT_I)->SetWindowText(m_strI);
	}
	// TODO:  在此添加额外的初始化
	else
	{
		GetDlgItem(IDC_EDIT_X)->SetWindowText(CString("-500"));
		GetDlgItem(IDC_EDIT_Y)->SetWindowText(CString("300"));
		GetDlgItem(IDC_EDIT_Z)->SetWindowText(CString("500"));

		GetDlgItem(IDC_EDIT_CX)->SetWindowText(CString("0"));
		GetDlgItem(IDC_EDIT_CY)->SetWindowText(CString("0"));
		GetDlgItem(IDC_EDIT_CZ)->SetWindowText(CString("0"));

		GetDlgItem(IDC_EDIT_R)->SetWindowText(CString("100"));

		GetDlgItem(IDC_EDIT_I)->SetWindowText(CString("32"));

		m_LightColor = RGB(0.1, 0.5, 0.7); 
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
