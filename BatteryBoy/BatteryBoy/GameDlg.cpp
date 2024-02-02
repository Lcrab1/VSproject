// GameDlg.cpp: 实现文件
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
	DDX_Control(pDX, IDC_BATTERY_1, m_battery1);
	DDX_Control(pDX, IDC_BATTERY_2, m_battery2);
	DDX_Control(pDX, IDC_BATTERY_3, m_battery3);
}

void CGameDlg::gameInit()
{
	SetWindowPos(NULL, 0, 0, 1400, 600, SWP_NOMOVE | SWP_NOZORDER);

	m_background.SetWindowPos(NULL, 0, 0, 1400, 600, SWP_NOMOVE | SWP_NOZORDER);

	m_battery1.SetWindowPos(NULL, 0, 0, 130, 130, SWP_NOMOVE | SWP_NOZORDER);
	m_battery2.SetWindowPos(NULL, 0, 0, 130, 130, SWP_NOMOVE | SWP_NOZORDER);
	m_battery3.SetWindowPos(NULL, 0, 0, 130, 130, SWP_NOMOVE | SWP_NOZORDER);
	MessageBox( "爱护环境人人有责！", "提示", 0);
	MessageBox( "您可以使用WASD移动，在废弃电池附近按下空格即可回收电池!", "提示", 0);
	MessageBox( "收集完所有的废弃电池即可完成游戏！", "提示", 0);




}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_STN_CLICKED(IDC_GAME_BACKGROUND, &CGameDlg::OnStnClickedGameBackground)
	ON_WM_KEYDOWN(IDC_GAME_BACKGROUND, &CGameDlg::PreTranslateMessage)
	ON_STN_CLICKED(IDC_BATTERY_1, &CGameDlg::OnStnClickedBattery1)
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
	if (pMsg->message == WM_KEYUP)
	{
		CRect rectPictureCtrl;
		m_player.GetWindowRect(&rectPictureCtrl);
		ScreenToClient(&rectPictureCtrl);
		switch (pMsg->wParam)
		{
		case 'W':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left, rectPictureCtrl.top - 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'A':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left - 30, rectPictureCtrl.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'S':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left, rectPictureCtrl.top + 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'D':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left + 30, rectPictureCtrl.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case VK_SPACE:  // 处理空格键的事件
		{
			CRect rectPic2;
			m_battery1.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			CRect intersection;
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery1.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox("提示", "恭喜您！收集完了所有的电池！", 0);
					exit(0);
				}
			}

			m_battery2.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery2.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox( "恭喜您！收集完了所有的电池！", "提示", 0);
					exit(0);
				}
			}

			m_battery3.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery3.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox("提示", "恭喜您！收集完了所有的电池！", 0);
					exit(0);
				}
			}
			return TRUE;  // 返回TRUE表示消息已被处理
		}
		default:
			return TRUE;  // 返回TRUE表示消息已被处理
		}
		
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		CRect rectPictureCtrl;
		m_player.GetWindowRect(&rectPictureCtrl);
		ScreenToClient(&rectPictureCtrl);
		switch (pMsg->wParam)
		{
		case 'W':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left, rectPictureCtrl.top - 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'A':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left - 30, rectPictureCtrl.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'S':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left, rectPictureCtrl.top + 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case 'D':
			m_player.SetWindowPos(NULL, rectPictureCtrl.left + 30, rectPictureCtrl.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			return TRUE;
		case VK_SPACE:  // 处理空格键的事件
		{
			CRect rectPic2;
			m_battery1.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			CRect intersection;
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery1.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox("提示", "恭喜您！收集完了所有的电池！", 0);
				}
			}

			m_battery2.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery2.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox("提示", "恭喜您！收集完了所有的电池！", 0);
				}
			}

			m_battery3.GetWindowRect(&rectPic2);  // 获取ID_PIC2图片的位置
			ScreenToClient(&rectPic2);
			if (intersection.IntersectRect(&rectPictureCtrl, &rectPic2))  // 检查两个矩形是否有交集
			{
				m_battery3.ShowWindow(SW_HIDE);  // 隐藏ID_PIC2图片
				m_count++;
				if (m_count == 3) {
					MessageBox( "提示", "恭喜您！收集完了所有的电池！",0);
				}
			}


			return TRUE;  // 返回TRUE表示消息已被处理
		}
		default:
			return TRUE;  // 返回TRUE表示消息已被处理
		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
	
}


void CGameDlg::OnStnClickedBattery1()
{
	// TODO: 在此添加控件通知处理程序代码
}
