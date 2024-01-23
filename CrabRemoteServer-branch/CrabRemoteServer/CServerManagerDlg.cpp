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
	, m_setListenPort()
	, m_setMaxConnects()
{

}

CServerManagerDlg::~CServerManagerDlg()
{
}

void CServerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_LISTEN_PORT, m_setListenPort);
	DDX_Control(pDX, IDC_SET_MAX_CONNECTS, m_setMaxConnects);
	DDX_Control(pDX, IDC_APPLY_BUTTON, m_applyButton);
}


BEGIN_MESSAGE_MAP(CServerManagerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, &CServerManagerDlg::OnBnClickedApplyButton)
END_MESSAGE_MAP()


// CServerManagerDlg 消息处理程序


void CServerManagerDlg::OnBnClickedApplyButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CServerManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int ListenPort = 0;
	int MaxConnections = 0;

	m_applyButton.ShowWindow(SW_HIDE);
	m_applyButton.EnableWindow(FALSE);
	 
	m_ConfigFile.GetIntFromCongfigFile(_T("Settings"), _T("ListenPort"), (int*)&ListenPort);
	m_ConfigFile.GetIntFromCongfigFile(_T("Settings"), _T("MaxConnections"), (int*)&MaxConnections);
	m_SetListenPort = ListenPort;
	m_SetMaxConnections = MaxConnections;

	//刷新窗口
	UpdateData(FALSE);



	m_ApplyButton.ShowWindow(SW_NORMAL);
	m_ApplyButton.EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
