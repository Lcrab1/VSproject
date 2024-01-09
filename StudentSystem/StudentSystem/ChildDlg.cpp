// ChildDlg.cpp: 实现文件
//

#include "pch.h"
#include "StudentSystem.h"
#include "ChildDlg.h"
#include "afxdialogex.h"


// ChildDlg 对话框

IMPLEMENT_DYNAMIC(ChildDlg, CDialogEx)

ChildDlg::ChildDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ChildDlg, pParent)
	, m_id(_T(""))
	, m_name(_T(""))
	, m_math(_T(""))
	, m_cpp(_T(""))
	, m_english(_T(""))
	, m_sex(_T(""))
{

}

ChildDlg::~ChildDlg()
{
}

void ChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_id);
	DDX_Text(pDX, IDC_EDIT3, m_name);
	DDX_Text(pDX, IDC_EDIT4, m_math);
	DDX_Text(pDX, IDC_EDIT5, m_cpp);
	DDX_Text(pDX, IDC_EDIT6, m_english);
	DDX_Text(pDX, IDC_EDIT2, m_sex);
}


BEGIN_MESSAGE_MAP(ChildDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT3, &ChildDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT6, &ChildDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT1, &ChildDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &ChildDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT5, &ChildDlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON2, &ChildDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ChildDlg 消息处理程序


void ChildDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void ChildDlg::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void ChildDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void ChildDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_id == TEXT("") ||m_sex==TEXT("")|| m_name == TEXT("") || m_math == TEXT("") || m_cpp == TEXT("") || m_english == TEXT("")) {
		MessageBox(TEXT("您的输入为空，请补充完整"), TEXT("提示")); 
	}
	else
	{
		OnOK();
		return;
	}
}


void ChildDlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void ChildDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}
