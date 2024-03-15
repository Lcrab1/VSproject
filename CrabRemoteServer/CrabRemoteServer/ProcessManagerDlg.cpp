// ProcessManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "CrabRemoteServer.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"


// CProcessManagerDlg 对话框

IMPLEMENT_DYNAMIC(CProcessManagerDlg, CDialogEx)

CProcessManagerDlg::CProcessManagerDlg(CWnd* pParent, CIocpServer*
	IocpServer, CONTEXT_OBJECT* ContextObject)
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

	ON_COMMAND(ID_REFRESH_PROCESS, &CProcessManagerDlg::OnRefreshProcess)
	ON_COMMAND(ID_KILL_PROCESS, &CProcessManagerDlg::OnKillProcess)
	ON_COMMAND(ID_SUSPEND_PROCESS, &CProcessManagerDlg::OnSuspendProcess)
	ON_COMMAND(ID_RESUME_PROCESS, &CProcessManagerDlg::OnResumeProcess)
	ON_COMMAND(ID_PROCESS_MODULES, &CProcessManagerDlg::OnProcessModules)
	ON_COMMAND(ID_PROCESS_THREADS, &CProcessManagerDlg::OnProcessThreads)
	ON_COMMAND(ID_PROCESS_MEMORY, &CProcessManagerDlg::OnProcessMemory)
	ON_COMMAND(ID_PROCESS_HANDLES, &CProcessManagerDlg::OnProcessHandles)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_INFO_LIST, &CProcessManagerDlg::OnRclickProcessInfoList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROCESS_INFO_LIST, &CProcessManagerDlg::OnCustomdrawProcessInfoList)
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


void CProcessManagerDlg::OnRefreshProcess()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnKillProcess()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnSuspendProcess()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnResumeProcess()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnProcessModules()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnProcessThreads()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnProcessMemory()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnProcessHandles()
{
	// TODO: 在此添加命令处理程序代码
}


void CProcessManagerDlg::OnRclickProcessInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CProcessManagerDlg::OnCustomdrawProcessInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
