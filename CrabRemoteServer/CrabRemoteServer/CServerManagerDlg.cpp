// CServerManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "CrabRemoteServer.h"
#include "CServerManagerDlg.h"
#include "afxdialogex.h"


// CServerManagerDlg 对话框

IMPLEMENT_DYNAMIC(CServerManagerDlg, CDialogEx)

CServerManagerDlg::CServerManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_MANAGER_DIALOG, pParent)
{

}

CServerManagerDlg::~CServerManagerDlg()
{
}

void CServerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CServerManagerDlg, CDialogEx)
	ON_BN_CLICKED(ID_APPLY_BUTTON, &CServerManagerDlg::OnBnClickedApplyButton)
	ON_EN_CHANGE(IDC_SET_MAX_CONNECTS2, &CServerManagerDlg::OnEnChangeSetMaxConnects2)
END_MESSAGE_MAP()


// CServerManagerDlg 消息处理程序


void CServerManagerDlg::OnBnClickedApplyButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CServerManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int ListenPort = 0;
	int MaxConnections = 0;

	m_ApplyButton.ShowWindow(SW_HIDE);
	m_ApplyButton.EnableWindow(FALSE);

	m_Configfile.GetIntFromCongfigFile(_T("Settings"), _T("ListenPort"), (int*)&ListenPort);
	m_Configfile.GetIntFromCongfigFile(_T("Settings"), _T("MaxConnections"), (int*)&MaxConnections);
	m_setListenPort = ListenPort;
	m_setMaxConnections = MaxConnections;

	//刷新窗口
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




void CServerManagerDlg::OnEnChangeSetMaxConnects2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
