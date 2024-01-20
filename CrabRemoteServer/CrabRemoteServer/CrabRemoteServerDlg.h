
// CrabRemoteServerDlg.h: 头文件
//

#pragma once
#include"TrueColorToolBar.h"

// CCrabRemoteServerDlg 对话框
class CCrabRemoteServerDlg : public CDialogEx
{
// 构造
public:
	CCrabRemoteServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRABREMOTESERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ClientInfoList;
	CListCtrl m_ServerInfoList;
	CTrueColorToolBar m_TrueColorToolBar;	//控件变量的声明

public:
	//初始化表格 
	void initListCtrl();
	
	//展示时间
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void getLocalTime();

	void intiSolidMenu();	//初始化菜单
	afx_msg void OnMenuSetLocal();
	afx_msg void OnMenuAddInformation();
	afx_msg void OnMenuExit();
	afx_msg void OnClose();

	//初始化真彩工具棒
	void TrueToolBarInit();

};
