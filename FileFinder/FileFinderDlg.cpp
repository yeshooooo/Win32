
// FileFinderDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FileFinder.h"
#include "FileFinderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileFinderDlg dialog



CFileFinderDlg::CFileFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FILEFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT_LIST, m_ResultList);
}

// 关联消息号和消息响应函数
BEGIN_MESSAGE_MAP(CFileFinderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE_BTN, &CFileFinderDlg::OnBnClickedBrowseBtn)
	ON_BN_CLICKED(IDC_START_BTN, &CFileFinderDlg::OnBnClickedStartBtn)
	ON_MESSAGE(WM_ADD_ITEM, &CFileFinderDlg::OnAddItem) // 自定义的
	ON_BN_CLICKED(IDC_STOP_BTN, &CFileFinderDlg::OnBnClickedStopBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFileFinderDlg message handlers

BOOL CFileFinderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 为ListControl设置样式,设置样式是设置他的ExtendStyle
	m_ResultList.SetExtendedStyle(
		m_ResultList.GetExStyle() // 保证listcontrol以前的样式还存在
		| LVS_EX_GRIDLINES // 网格
		| LVS_EX_FULLROWSELECT // 单行选择

	);

	// 插入一列
	m_ResultList.InsertColumn(0, TEXT("查找结果"), LVCFMT_LEFT, 1050); // 左对齐，宽度1050，右侧要留点位置给滚动条

	SetTimer(1, 100, NULL); // 启动定时器
	// 在调试模式下给两个文本框加上默认值
#ifdef _DEBUG
	SetDlgItemText(IDC_PATH_EDIT, TEXT("E:\\codes"));
	SetDlgItemText(IDC_FNAME_EDIT, TEXT("123.txt"));
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 用于统计文件个数的全局变量
LONG g_FindCount;

// 统计线程个数
LONG g_ThreadNum;
// 表面当前程序运行状态的全局变量，用于停止按钮的安全处理
BOOL bFinding = FALSE;
// 浏览按钮功能的添加
// vc++基础班第16课
//https://www.bilibili.com/video/BV1rt411B7V9?p=18&vd_source=c6ca89f75d00cd4da634736edfcca1ae
void CFileFinderDlg::OnBnClickedBrowseBtn()
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[MAX_PATH] = { 0 };
	BROWSEINFO mBroInfo = { 0 };
	mBroInfo.hwndOwner = m_hWnd;
	ITEMIDLIST* pid1 = SHBrowseForFolder(&mBroInfo);
	if (SHGetPathFromIDList(pid1, szPath))
	{
		SetDlgItemText(IDC_PATH_EDIT, szPath);
	}
	CoTaskMemFree(pid1);
}

// 查找按钮
UINT __cdecl FindProc(LPVOID pParam)
{
	CFindItem* pitem = (CFindItem*)pParam;

	// 用于枚举文件的类， CFileFind
	CString strRootPath = pitem->m_PathFind;
	// 要判断结尾有没有\，没用的话要给他加上，因为还要给他拼接*.*
	if (strRootPath[strRootPath.GetLength() - 1] != '\\')
	{
		strRootPath += TEXT("\\");
	}
	CFileFind mFinder;
	BOOL bFind = mFinder.FindFile(strRootPath + _T("*.*"));// 第二个参数没用，第一个是文件路径
	while (bFind && bFinding)
	{
		bFind = mFinder.FindNextFileW();
		if (mFinder.IsDots())
		{
			// 找到的是..
			continue;
		} else if (mFinder.IsDirectory()) // 如果找到的是目录，开启多线程
		{
			// 套娃
			CFindItem* pSubItem = new CFindItem(*pitem);
			pSubItem->m_PathFind = mFinder.GetFilePath();
			AfxBeginThread(FindProc, pSubItem);


		}
		else
		{
			// 说明找到的是文件,进行比对
			CString strFileName = mFinder.GetFileName();
			if (strFileName.CompareNoCase(pitem->m_FName) == 0)
			{
				// 如果找到了，则个数+1
				InterlockedIncrement(&g_FindCount);
				g_ThreadNum += 1;
				CString strPath = mFinder.GetFilePath();
				// 向界面上发消息，而不建议在工作线程上操作界面
				// 路径通过wParam或者lParam发送过去
				//先用PostMessage试试,结果乱码
				// 因为PostMessage是异步的，他发送出去了不管你处理完没有，马上就会返回
				//PostMessage(pitem->m_hMainDlg, WM_ADD_ITEM, 0, (LPARAM)(LPCTSTR)strPath);

				// SendMessage是同步的SendMessage是非队列消息
				SendMessage(pitem->m_hMainDlg, WM_ADD_ITEM, 0, (LPARAM)(LPCTSTR)strPath);
			}
		}
	}

	//清除堆中的内存
	delete pitem;
	// 退出线程的时候，线程数量-1
	InterlockedDecrement(&g_ThreadNum);
	return 0;
}
void CFileFinderDlg::OnBnClickedStartBtn()
{
	// TODO: Add your control notification handler code here
	// 每次点击查找按钮的时候，个数清零
	g_FindCount = 0;
	g_ThreadNum = 0;
	bFinding = TRUE;
	// 每次点击查找按钮，清空下高级列表控件
	m_ResultList.DeleteAllItems();
	// 这里的编程思想是每遇到一个目录就开启一个线程

		//0. 自定义的类,从界面上拿值
	// 这里要在堆中申请内存，否则按钮点下完了就被自动回收了,清除的时候，在上面回调中清除
	CFindItem *pitem = new CFindItem(); 
	GetDlgItemText(IDC_PATH_EDIT, pitem->m_PathFind);
	GetDlgItemText(IDC_FNAME_EDIT, pitem->m_FName);
	//把当前对话框的窗口句柄传出去，用于通信,在线程函数中使用
	pitem->m_hMainDlg = GetSafeHwnd();
	// 1. 开启多线程
	// 因为不设计界面的东西，开启工作线程即可
	AfxBeginThread(FindProc, pitem); // 异步的


	


	

}

LRESULT CFileFinderDlg::OnAddItem(WPARAM wParam, LPARAM lParam)
{
	CString strPath = (LPCTSTR)lParam;
	m_ResultList.InsertItem(m_ResultList.GetItemCount(), strPath);
	return TRUE;
}


void CFileFinderDlg::OnBnClickedStopBtn()
{
	// TODO: Add your control notification handler code here
	bFinding = FALSE;
}


void CFileFinderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	// 用定时器
	if (g_ThreadNum > 0)
	{
		// 线程大于0 ，让编辑框变灰
		GetDlgItem(IDC_PATH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FNAME_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_BTN)->EnableWindow(TRUE); // 停止按钮正常
	}
	else
	{
		GetDlgItem(IDC_PATH_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_FNAME_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BROWSE_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}
