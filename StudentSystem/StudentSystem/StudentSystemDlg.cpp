
// StudentSystemDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "StudentSystem.h"
#include "StudentSystemDlg.h"
#include "afxdialogex.h"
#include "ChildDlg.h"
#include"Statistics.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStudentSystemDlg 对话框



CStudentSystemDlg::CStudentSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDENTSYSTEM_DIALOG, pParent)
	, m_find(_T(""))
	, m_findName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_find);
	DDX_Text(pDX, IDC_EDIT2, m_findName);
}

BEGIN_MESSAGE_MAP(CStudentSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CStudentSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CStudentSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CStudentSystemDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CStudentSystemDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CStudentSystemDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CStudentSystemDlg::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT1, &CStudentSystemDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CStudentSystemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CStudentSystemDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON9, &CStudentSystemDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON11, &CStudentSystemDlg::OnBnClickedButton11)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CStudentSystemDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON10, &CStudentSystemDlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CStudentSystemDlg 消息处理程序

//初始化界面
BOOL CStudentSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//设置列表风格
	DWORD dwStyle = m_list.GetExtendedStyle();		
	dwStyle |= LVS_EX_FULLROWSELECT;				//选中某行使整行高亮	
	dwStyle |= LVS_EX_GRIDLINES;					//网格线
	m_list.SetExtendedStyle(dwStyle);				//设置扩展风格

	// TODO: 在此添加额外的初始化代码
	m_list.InsertColumn(0, TEXT("学号"), 0, 170);
	m_list.InsertColumn(1, TEXT("姓名"), 0, 150);
	m_list.InsertColumn(2, TEXT("性别"), 0, 50);
	m_list.InsertColumn(3, TEXT("高等数学"), 0, 130);
	m_list.InsertColumn(4, TEXT("计算机"), 0, 130);
	m_list.InsertColumn(5, TEXT("英语"), 0, 130);
	m_list.InsertColumn(6, TEXT("平均成绩"), 0, 130);
	is_saved = false;
	is_open = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStudentSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStudentSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStudentSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开
void CStudentSystemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//设置过滤器
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);

	//显示打开文件对话框
	if (IDOK == fileDlg.DoModal()) { 
		strFilePath = fileDlg.GetPathName(); 
		dataInterFace.openFile(strFilePath);
		is_open = true;
		upDateList();
	}

}


//添加
void CStudentSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码 
	if (is_open) {
		ChildDlg dlg;
		if (IDOK == dlg.DoModal())
		{
			Student student(atoi(dlg.m_id), dlg.m_name.GetBuffer(), dlg.m_sex.GetBuffer(),atoi(dlg.m_math), atoi(dlg.m_cpp), atoi(dlg.m_english));
			dataInterFace.addStudent(student);
			upDateList();
		}
	}
	else
	{
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
	
}


//查找
void CStudentSystemDlg::OnBnClickedButton7()
{
	if (is_open) {
		UpdateData(TRUE);
		Student findStudent = dataInterFace.searchId(atoi(m_find));
		if (findStudent.getId() == -1) {
			MessageBox(TEXT("查无此记录！"), TEXT("提示"));
		}
		else {
			CString str;
			str.Format(TEXT("查找成功！\n\n学号：%d\n姓名：%s\n性别：%s\n高数：%d\nC++：%d\n英语：%d\n平均成绩：%f"),
				findStudent.getId(), findStudent.getName().c_str(),findStudent.getSex().c_str(),findStudent.getMath(), findStudent.getCPP(), findStudent.getEnglish(),findStudent.getAverage());
			MessageBox(str, TEXT("提示"));
		}
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
	
	// TODO: 在此添加控件通知处理程序代码
}


//更新列表
void CStudentSystemDlg::upDateList()
{
	m_list.DeleteAllItems();
	CString str;
	for (int i = 0; i < dataInterFace.vStudent.size(); i++) {
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getId());
			m_list.InsertItem(i,str);
		str.Format(TEXT("%s"), dataInterFace.vStudent[i].getName().c_str());
			m_list.SetItemText(i, 1,str);
		str.Format(TEXT("%s"), dataInterFace.vStudent[i].getSex().c_str());
			m_list.SetItemText(i, 2, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getMath());
			m_list.SetItemText(i, 3, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getCPP());
			m_list.SetItemText(i, 4, str);
		str.Format(TEXT("%d"), dataInterFace.vStudent[i].getEnglish());
			m_list.SetItemText(i, 5, str);
		str.Format(TEXT("%f"), dataInterFace.vStudent[i].getAverage());
			m_list.SetItemText(i, 6, str);
	}


}


//删除
void CStudentSystemDlg::OnBnClickedButton5()
{
	//删除信息
	// TODO: 在此添加控件通知处理程序代码
	if (is_open) {
		int index = m_list.GetSelectionMark();
		if (index > -1) 
		{
			UINT i;
			i = MessageBox(_T("确定要删除这条记录吗？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);
			if (i == IDYES) {
				dataInterFace.deleteId(index);
				MessageBox(TEXT("删除成功！"), TEXT("提示"));
				upDateList();
			}
			else
			{
				return;
			}
		}
		else 
		{
			MessageBox(TEXT("请先点击列表选择需要删除的记录！"), TEXT("提示"));
		}
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}

}


//修改
void CStudentSystemDlg::OnBnClickedButton6()
{
	//修改信息
	// TODO: 在此添加控件通知处理程序代码
	if (is_open) {
		int64_t index = m_list.GetSelectionMark();
		if (index > -1) {
			ChildDlg dlg;
			CString str;
			str.Format(TEXT("%d"), dataInterFace.vStudent[index].getId());
			dlg.m_id = str;
			str.Format(TEXT("%s"), dataInterFace.vStudent[index].getName().c_str());
			dlg.m_name = str;
			str.Format(TEXT("%s"), dataInterFace.vStudent[index].getSex().c_str());
			dlg.m_sex = str;
			str.Format(TEXT("%d"), dataInterFace.vStudent[index].getMath());
			dlg.m_math = str;
			str.Format(TEXT("%d"), dataInterFace.vStudent[index].getCPP());
			dlg.m_cpp = str;
			str.Format(TEXT("%d"), dataInterFace.vStudent[index].getEnglish());
			dlg.m_english = str;
	
			if (IDOK == dlg.DoModal())
			{
				Student student(atoi(dlg.m_id), dlg.m_name.GetBuffer(),dlg.m_sex.GetBuffer(),atoi(dlg.m_math), atoi(dlg.m_cpp), atoi(dlg.m_english));
				dataInterFace.changeId(index, student);
				upDateList();
			}
			else
			{
				return;
			}
		}
		else {
			MessageBox(TEXT("请先点击列表选择需要修改的记录！"), TEXT("提示"));
		}
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
}


void CStudentSystemDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


//保存文件
void CStudentSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (is_open) {
		if (dataInterFace.saveFile(strFilePath)) {
			MessageBox(TEXT("已保存！"), TEXT("提示"));
			is_saved = true;
		}
		else {
			MessageBox(TEXT("保存失败！"), TEXT("提示"));
		}
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
	
	
}

//退出
void CStudentSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (is_open) {
		if (is_saved == false) {
			UINT i;
			i = MessageBox(_T("您还没有保存，确定要退出吗？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);
			if (i == IDYES) {
				exit(0);
			}
			else
			{
				return;
			}
		}
		else {
			exit(0);
		}
	}
	else {
		exit(0);
	}
	

}

//升序
void CStudentSystemDlg::OnBnClickedButton8()
{
	if (is_open) {
		dataInterFace.ascendingSort();
		upDateList();
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
}

//降序
void CStudentSystemDlg::OnBnClickedButton9()
{
	if (is_open) {
		dataInterFace.descendingSort();
		upDateList();
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
}

//按姓名查找
void CStudentSystemDlg::OnBnClickedButton11()
{
	if (is_open) {
		UpdateData(TRUE);
		Student findStudent = dataInterFace.searchName(m_findName.GetString());
		if (findStudent.getId() == -1) {
			MessageBox(TEXT("查无此记录！"), TEXT("提示"));
		}
		else {
			CString str;
			str.Format(TEXT("查找成功！\n\n学号：%d\n姓名：%s\n性别：%s\n高数：%d\nC++：%d\n英语：%d\n平均成绩：%f"),
				findStudent.getId(), findStudent.getName().c_str(), findStudent.getSex().c_str(), findStudent.getMath(), findStudent.getCPP(), findStudent.getEnglish(),findStudent.getAverage());
			MessageBox(str, TEXT("提示"));
		}
	}
	else {
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
}


void CStudentSystemDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//统计
void CStudentSystemDlg::OnBnClickedButton10()
{
	if (is_open) {
		Statistics dlg;
		if (IDOK == dlg.DoModal())
		{
		
			upDateList();
		}
	}
	else
	{
		MessageBox(TEXT("请先打开文件"), TEXT("提示"));
	}
}
