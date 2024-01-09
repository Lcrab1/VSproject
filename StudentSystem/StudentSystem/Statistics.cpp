// Statistics.cpp: 实现文件
//

#include "pch.h"
#include "StudentSystem.h"
#include "Statistics.h"
#include "afxdialogex.h"


// Statistics 对话框

IMPLEMENT_DYNAMIC(Statistics, CDialogEx)

Statistics::Statistics(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Statistics, pParent)
{

}

Statistics::~Statistics()
{
}

void Statistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list2);
}


BEGIN_MESSAGE_MAP(Statistics, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Statistics::OnLvnItemchangedList1)
END_MESSAGE_MAP()

void Statistics::upDateList(DataInterFace dataInterFace) {
	m_list2.DeleteAllItems();
	CString str;
	for (int i = 0; i < dataInterFace.vStudent.size(); i++) {
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getId());
		m_list2.InsertItem(i, str);
		str.Format(TEXT("%s"), dataInterFace.vStudent[i].getName().c_str());
		m_list2.SetItemText(i, 1, str);
		str.Format(TEXT("%s"), dataInterFace.vStudent[i].getSex().c_str());
		m_list2.SetItemText(i, 2, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getMath());
		m_list2.SetItemText(i, 3, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getCPP());
		m_list2.SetItemText(i, 4, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getEnglish());
		m_list2.SetItemText(i, 5, str);
		str.Format(TEXT("%f"), dataInterFace.vStudent[i].getAverage());
		m_list2.SetItemText(i, 6, str);
	}
}

BOOL Statistics::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	//设置列表风格
	DWORD dwStyle = m_list2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;				//选中某行使整行高亮	
	dwStyle |= LVS_EX_GRIDLINES;					//网格线
	m_list2.SetExtendedStyle(dwStyle);				//设置扩展风格

	// TODO: 在此添加额外的初始化代码
	m_list2.InsertColumn(0, TEXT("分数段"), 0, 90);
	m_list2.InsertColumn(1, TEXT("高等数学"), 0,90);
	m_list2.InsertColumn(2, TEXT("计算机"), 0, 90);
	m_list2.InsertColumn(3, TEXT("英语"), 0, 90);
	m_list2.InsertColumn(4, TEXT("平均成绩"), 0, 90);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}




// Statistics 消息处理程序


void Statistics::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
