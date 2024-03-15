// ProcessManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "CrabRemoteServer.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"


// CProcessManagerDlg 对话框

IMPLEMENT_DYNAMIC(CProcessManagerDlg, CDialogEx)

CProcessManagerDlg::CProcessManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_MANAGER_DIALOG, pParent)
{

}

CProcessManagerDlg::~CProcessManagerDlg()
{
}

void CProcessManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_INFO_LIST, m_ProcessInfoList);
}


BEGIN_MESSAGE_MAP(CProcessManagerDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CProcessManagerDlg 消息处理程序


void CProcessManagerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
