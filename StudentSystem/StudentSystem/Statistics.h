#pragma once
#include"afxcmn.h"
#include"StudentSystem.h"
#include"StudentSystemDlg.h"
// Statistics 对话框

class Statistics : public CDialogEx
{
	DECLARE_DYNAMIC(Statistics)

public:
	Statistics(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Statistics();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Statistics };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list2;
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	void upDateList(DataInterFace dataInterFace);
};
