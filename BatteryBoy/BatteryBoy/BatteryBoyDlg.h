﻿
// BatteryBoyDlg.h: 头文件
//

#pragma once
#include"GameDlg.h"

// CBatteryBoyDlg 对话框
class CBatteryBoyDlg : public CDialogEx
{
// 构造
public:
	CBatteryBoyDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BATTERYBOY_DIALOG };
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
	void gameStart();
	afx_msg void OnStnClickedGameStart();
	//游戏开始菜单
	CStatic m_gameStartBitmap;
//	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
};