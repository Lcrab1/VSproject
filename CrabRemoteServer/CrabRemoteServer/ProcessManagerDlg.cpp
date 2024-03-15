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
	m_IocpServer = IocpServer;
	m_ContextObject = ContextObject;
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
	if (m_ContextObject != NULL)
	{

		CancelIo((HANDLE)m_ContextObject->clientSocket);
		closesocket(m_ContextObject->clientSocket);
	}
	CDialogEx::OnClose();
}


BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString v1;
	sockaddr_in  ClientAddress;
	memset(&ClientAddress, 0, sizeof(ClientAddress));
	int ClientAddressLength = sizeof(ClientAddress);
	BOOL IsOk = getpeername(m_ContextObject->clientSocket, (SOCKADDR*)&ClientAddress, &ClientAddressLength); //得到连接的ip 
	v1.Format("\\\\%s - 远程进程管理", IsOk != INVALID_SOCKET ? inet_ntoa(ClientAddress.sin_addr) : "");
	//设置对话框标题
	SetWindowText(v1);



	//设置对话框上数据
	LOGFONT  Logfont;
	CFont* v2 = m_ProcessInfoList.GetFont();
	v2->GetLogFont(&Logfont);
	//调整比例
	Logfont.lfHeight = Logfont.lfHeight * 1.3;   //这里可以修改字体的高比例
	Logfont.lfWidth = Logfont.lfWidth * 1.3;     //这里可以修改字体的宽比例
	CFont  v3;
	v3.CreateFontIndirect(&Logfont);
	m_ProcessInfoList.SetFont(&v3);
	m_ProcessInfoList.SetFont(&v3);
	v3.Detach();




	//被控端传回的数据
	char* BufferData = (char*)(m_ContextObject->m_ReceivedBufferDataDecompressed.GetArray(0));
	m_ProcessInfoList.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	switch (BufferData[0])
	{
	case CLIENT_PROCESS_MANAGER_REPLY:
	{

		m_ProcessInfoList.InsertColumn(0, _T("PID"), LVCFMT_LEFT, 80);
		m_ProcessInfoList.InsertColumn(1, _T("进程镜像"), LVCFMT_LEFT, 180);
		m_ProcessInfoList.InsertColumn(2, _T("程序路径"), LVCFMT_LEFT, 250);
		m_ProcessInfoList.InsertColumn(3, _T("程序位数"), LVCFMT_LEFT, 120);
		ShowProcessInfoList();   //由于第一个发送来的消息后面紧跟着进程的数据所以把数据显示到列表当中\0\0
		break;
	}
	default:
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CProcessManagerDlg::GetProcessInfoList()
{
	BYTE IsToken = CLIENT_PROCESS_MANAGER_REFRESH_REQUIRE;
	m_IocpServer->OnPrepareSending(m_ContextObject, &IsToken, 1);
}

void CProcessManagerDlg::ShowProcessInfoList(void)
{
	char* BufferData = (char*)(m_ContextObject->m_ReceivedBufferDataDecompressed.GetArray(1));
	char* ProcessImageName = NULL;
	char* ProcessFullPath = NULL;
	char* IsWow64Process = NULL;
	DWORD	Offset = 0;
	CString v1;
	m_ProcessInfoList.DeleteAllItems();
	//遍历发送来的每一个字符别忘了他的数据结构啊 Id+进程名+0+完整名+0
	int i;
	for (i = 0; Offset < m_ContextObject->m_ReceivedBufferDataDecompressed.GetArrayLength() - 1; i++)
	{
		HANDLE* ProcessIdentity = (HANDLE*)(BufferData + Offset);              //进程ID
		ProcessImageName = BufferData + Offset + sizeof(HANDLE);         //进程名
		ProcessFullPath = ProcessImageName + lstrlen(ProcessImageName) + 1;            //进程完整路径
		IsWow64Process = ProcessFullPath + lstrlen(ProcessFullPath) + 1;

		v1.Format(_T("%5u"), *ProcessIdentity);
		m_ProcessInfoList.InsertItem(i, v1);       //将得到的数据加入到列表当中

		m_ProcessInfoList.SetItemText(i, 1, ProcessImageName);
		m_ProcessInfoList.SetItemText(i, 2, ProcessFullPath);
		m_ProcessInfoList.SetItemText(i, 3, IsWow64Process);
		//ItemData 为进程ID
		m_ProcessInfoList.SetItemData(i, (DWORD_PTR)*ProcessIdentity);   //设置为隐藏
		//跳过这个数据结构进入下一个循环
		Offset += sizeof(HANDLE) + lstrlen(ProcessImageName) + lstrlen(ProcessFullPath) + lstrlen(IsWow64Process) + 3;

	}

	v1.Format(_T("程序个数 / %d"), i);
	LVCOLUMN v3;
	v3.mask = LVCF_TEXT;
	v3.pszText = v1.GetBuffer(0);
	v3.cchTextMax = v1.GetLength();
	m_ProcessInfoList.SetColumn(3, &v3); //在列表中显示有多少个进程
}

void CProcessManagerDlg::OnReceiveComplete(void)
{
	switch (m_ContextObject->m_ReceivedBufferDataDecompressed.GetArray(0)[0])
	{
	case CLIENT_PROCESS_MANAGER_REPLY:
	{

		ShowProcessInfoList();
		break;
	}

	default:
	{
		break;
	}
	}
	return;
}


void CProcessManagerDlg::OnRefreshProcess()
{
	// TODO: 在此添加命令处理程序代码
	GetProcessInfoList();
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

	CMenu Menu;
	Menu.LoadMenu(IDR_CLIENT_PROCESS_MENU);
	CPoint Point;
	GetCursorPos(&Point);
	SetForegroundWindow();   //设置当前窗口
	Menu.GetSubMenu(0)->TrackPopupMenu(
		TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		Point.x, Point.y, this, NULL);
	*pResult = 0;
}


void CProcessManagerDlg::OnCustomdrawProcessInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// Take the default processing unless we set this to something else below.  
	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF NewTextColor, NewBackgroundColor;
		int     Item = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		CString ProcessImageName = m_ProcessInfoList.GetItemText(Item, 1);
		if (_tcscmp(ProcessImageName.GetBuffer(0), _T("explorer.exe")) == 0)
		{
			NewTextColor = RGB(0, 0, 0);
			NewBackgroundColor = RGB(255, 0, 0);
		}
		else if (_tcscmp(ProcessImageName.GetBuffer(0), _T("CalculatorApp.exe")) == 0)
		{
			NewTextColor = RGB(0, 0, 0);
			NewBackgroundColor = RGB(0, 0, 255);
		}

		else {
			NewTextColor = RGB(0, 0, 0);
			NewBackgroundColor = RGB(255, 255, 255);
		}

		pLVCD->clrText = NewTextColor;
		pLVCD->clrTextBk = NewBackgroundColor;
		*pResult = CDRF_DODEFAULT;
	}
}
