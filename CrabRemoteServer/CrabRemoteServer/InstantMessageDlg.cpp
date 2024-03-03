// InstantMessageDlg.cpp: 实现文件
//

#include "pch.h"
#include "CrabRemoteServer.h"
#include "InstantMessageDlg.h"
#include "afxdialogex.h"


// CInstantMessageDlg 对话框

IMPLEMENT_DYNAMIC(CInstantMessageDlg, CDialogEx)

CInstantMessageDlg::CInstantMessageDlg(CWnd* pParent, CIocpServer*
	IocpServer, CONTEXT_OBJECT* ContextObject)
	: CDialogEx(IDD_INSTANT_MESSAGE_DIALOG, pParent)
{

}

CInstantMessageDlg::~CInstantMessageDlg()
{
}

void CInstantMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstantMessageDlg, CDialogEx)
END_MESSAGE_MAP()


// CInstantMessageDlg 消息处理程序
