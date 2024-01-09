
// StudentSystemDlg.h: 头文件
//

#pragma once
#include"DataInterFace.h"

// CStudentSystemDlg 对话框
class CStudentSystemDlg : public CDialogEx
{
// 构造
public:
	CStudentSystemDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDENTSYSTEM_DIALOG };
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
	CListCtrl m_list;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

	DataInterFace dataInterFace;
	CString strFilePath; 
	void upDateList();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnEnChangeEdit1();
	CString m_find;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	bool is_saved;
	bool is_open;
	afx_msg void OnBnClickedButton9();
	CString m_findName;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton10();
};
