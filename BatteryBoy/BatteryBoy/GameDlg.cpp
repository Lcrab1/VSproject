﻿// GameDlg.cpp: 实现文件
//

#include "pch.h"
#include "BatteryBoy.h"
#include "GameDlg.h"
#include "afxdialogex.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_BACKGROUND, m_background);
	DDX_Control(pDX, IDC_PLAYER, m_player);
}

void CGameDlg::gameInit()
{
	SetWindowPos(NULL, 0, 0, 1400, 600, SWP_NOMOVE | SWP_NOZORDER);

	m_background.SetWindowPos(NULL, 0, 0, 1400, 600, SWP_NOMOVE | SWP_NOZORDER);







}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_STN_CLICKED(IDC_GAME_BACKGROUND, &CGameDlg::OnStnClickedGameBackground)
	ON_WM_KEYDOWN(IDC_GAME_BACKGROUND, &CGameDlg::PreTranslateMessage)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	gameInit();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CGameDlg::OnStnClickedGameBackground()
{
	// TODO: 在此添加控件通知处理程序代码
}


//void CGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}



BOOL CGameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYUP)
	{
		CRect rectPictureCtrl;
		m_player.GetWindowRect(&rectPictureCtrl);
		ScreenToClient(&rectPictureCtrl);
		switch (pMsg->wParam)
		{
		case 'W':
			rectPictureCtrl.top -= 10;
			m_player.MoveWindow(&rectPictureCtrl, TRUE);
			break;
		default:
			break;
		}
	}

		return CDialogEx::PreTranslateMessage(pMsg);
	
}