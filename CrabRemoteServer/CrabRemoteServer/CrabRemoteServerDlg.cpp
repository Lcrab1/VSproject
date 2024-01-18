﻿
// CrabRemoteServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CrabRemoteServer.h"
#include "CrabRemoteServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****定义我的列表数据结构体*****/
typedef struct
{
	char*	TitleData;             //列表的名称
	int		TitleWidth;            //列表的宽度
}COLUMN_DATA;

/*****Client表头*****/
COLUMN_DATA __ClientInfoList[] =
{
	{ "IP",			    120 },
	{ "Region",			60 },
	{ "Computer name / Note",	200 },
	{ "OS",		128 },
	{ "CPU",			    180 },
	{ "Camera",		    81 },
	{ "Ping",			151 }
};

/*****Server表头*****/
COLUMN_DATA __ServerInfoList[] =
{
	{ "Type",		200 },
	{ "Time",			200 },
	{ "Cotent",	    550 }
};



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCrabRemoteServerDlg 对话框



CCrabRemoteServerDlg::CCrabRemoteServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CRABREMOTESERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//
void CCrabRemoteServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CRAB_CLIENT_INFORMATION_LIST, m_ClientInfoList);
	DDX_Control(pDX, CRAB_SERVER_INFORMATION_LIST, m_ServerInfoList);
}

BEGIN_MESSAGE_MAP(CCrabRemoteServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCrabRemoteServerDlg 消息处理程序

BOOL CCrabRemoteServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码



	/*****调用时钟事件*****/
	SetTimer(0, 1000, NULL);		//1000从现在开始每隔1秒 NULL默认调用OnTimer()函数
	getLocalTime();

	/*****初始化主窗口*****/ 
	initListCtrl();				//先调用时钟，再初始化主窗口，可以让时间瞬时显示


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCrabRemoteServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCrabRemoteServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCrabRemoteServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCrabRemoteServerDlg::initListCtrl()
{
	//sizeof(__ClientInfoList) / sizeof(COLUMN_DATA) 计算ClientInfoList的列数
	for (int i = 0; i < sizeof(__ClientInfoList) / sizeof(COLUMN_DATA); i++)
	{
		//设置ClientInfoList的标题栏
		m_ClientInfoList.InsertColumn(i, __ClientInfoList[i].TitleData,
			LVCFMT_CENTER, __ClientInfoList[i].TitleWidth);
		//LVCFMT_CENTER是文字居中
	}
	m_ClientInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//sizeof(__ServerInfoList) / sizeof(COLUMN_DATA) 计算ServerInfoList的列数
	for (int i = 0; i < sizeof(__ServerInfoList) / sizeof(COLUMN_DATA); i++)
	{
		//设置ServerInfoList的标题栏
		m_ServerInfoList.InsertColumn(i, __ServerInfoList[i].TitleData, LVCFMT_CENTER,
			__ServerInfoList[i].TitleWidth);
	}
	m_ServerInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}



void CCrabRemoteServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);

	getLocalTime();
}

void CCrabRemoteServerDlg::getLocalTime()
{
	auto  Object = time(NULL);   //自动变量(Time的对象)   
	tm v1;						//tm是系统定义的结构体
	char  v2[MAX_PATH];
	localtime_s(&v1, &Object);		//获取本地时间，放入v1中
	strftime(v2, _countof(v2), "%Y-%m-%d %H:%M:%S", &v1);   //获得的时间v1以该格式进行字符串格式化 放入v2中
	SetWindowText(v2);   //设置到主窗口的标题栏上
}