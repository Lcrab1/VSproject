﻿#pragma once


// CServerManagerDlg 对话框

class CServerManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerManagerDlg)

public:
	CServerManagerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServerManagerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_MANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApplyButton();
	int m_setListenPort;
	int m_setMaxConnections;
	afx_msg void OnEnChangeSetMaxConnects2();
};