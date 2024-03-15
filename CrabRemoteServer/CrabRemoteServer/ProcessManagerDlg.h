#pragma once
#include"IocpServer.h"

// CProcessManagerDlg 对话框

class CProcessManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessManagerDlg)

public:
	CProcessManagerDlg(CWnd* pParent = NULL, CIocpServer* IocpServer = NULL, PCONTEXT_OBJECT ContextObject = NULL);   // 标准构造函数
	virtual ~CProcessManagerDlg();

	CIocpServer* m_IocpServer;
	PCONTEXT_OBJECT m_ContextObject;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_MANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CListCtrl m_ProcessInfoList;



	afx_msg void OnRefreshProcess();
	afx_msg void OnKillProcess();
	afx_msg void OnSuspendProcess();
	afx_msg void OnResumeProcess();
	afx_msg void OnProcessModules();
	afx_msg void OnProcessThreads();
	afx_msg void OnProcessMemory();
	afx_msg void OnProcessHandles();
	afx_msg void OnRclickProcessInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawProcessInfoList(NMHDR* pNMHDR, LRESULT* pResult);
};
