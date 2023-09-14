
// VCThread01Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "VCThread01.h"
#include "VCThread01Dlg.h"
#include "afxdialogex.h"
#include "CUIThreadApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CVCThread01Dlg dialog



CVCThread01Dlg::CVCThread01Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VCTHREAD01_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCThread01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVCThread01Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN, &CVCThread01Dlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDC_BUTTON1, &CVCThread01Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVCThread01Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CVCThread01Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_SUSPEND_BTN, &CVCThread01Dlg::OnBnClickedSuspendBtn)
	ON_BN_CLICKED(IDC_RESUME_BTN, &CVCThread01Dlg::OnBnClickedResumeBtn)
END_MESSAGE_MAP()


// CVCThread01Dlg message handlers

BOOL CVCThread01Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCThread01Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCThread01Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCThread01Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 线程函数
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
)
{
	int tipMsg = (int)lpParameter;
	CString strMsg;
	strMsg.Format(TEXT("%d"), tipMsg);
	AfxMessageBox(strMsg);
	return 0;
}

void CVCThread01Dlg::OnBnClickedBtn()
{
	// TODO: Add your control notification handler code here

	// 方式1： 调用CreateThread
	DWORD dwThreadId = 0;
	HANDLE tThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)456, 0, &dwThreadId);

	// 引用计数-1，释放线程资源
	CloseHandle(tThread);
}

UINT __cdecl MyControllingFunction(LPVOID pParam)
{
	MessageBox(NULL, TEXT("AfxBeginThread创建的线程"), TEXT("Tip"), MB_OK);
	return 0;
}
void CVCThread01Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	CWinThread* pThread = AfxBeginThread(MyControllingFunction, (LPVOID)456);
}

// 线程回调
UINT __cdecl ThreadProc2(LPVOID pParam)
{
	CVCThread01Dlg* pThisDlg = (CVCThread01Dlg*)pParam; // 强转为主对话框类的指针
	int tipMsg = pThisDlg->m_Num;
	//int tipMsg = (int)pParam;
	CString strTipMsg;
	while (TRUE)
	{
		strTipMsg.Format(TEXT("num = %d"), tipMsg++);
		OutputDebugString(strTipMsg);
		Sleep(50);

	}
	return 0;
}

// 工作线程函数
UINT __cdecl ThreadProc3(LPVOID pParam)
{
	CVCThread01Dlg* pThisDlg = (CVCThread01Dlg*)pParam; // 强转为主对话框类的指针

// 	// 弹出模态对话框
// 	CTestDlg dlg;
// 	dlg.DoModal(); // 模态对话框的DoModal方法是阻塞的

	// 非模态对话框是动态创建的，创建完后会继续往下走
	CTestDlg* pTestDlg = new CTestDlg();
	pTestDlg->Create(IDD_DIALOG1, NULL);
	pTestDlg->ShowWindow(SW_SHOW);
	
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return 0;
}

void CVCThread01Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	// 
	//Sleep(1000 * 20); // 睡眠10秒钟，睡眠是让当前程序的主线程睡眠(界面线程)
	// 把Sleep放入分线程去做，就不会阻塞主线程

	// 借用住对话框的指针来调用成员函数
	//m_Num = 123;
	//CWinThread* pThread = AfxBeginThread(ThreadProc3, this); // 工作线程创建

	// 使用界面线程
	AfxBeginThread(RUNTIME_CLASS(CUIThreadApp));
	
}

#include "CTestClass.h"
// suspend和resume示例
UINT __cdecl ThreadProct(LPVOID lpParameter)
{
	//int tipMsg = (int)lpParameter;
	//CString strTipMsg;
	//while (TRUE)
	//{
	//	strTipMsg.Format(TEXT("%d"), tipMsg);
	//	OutputDebugString(strTipMsg);
	//	Sleep(50);

	//}

	CTestClass m_Obj;

	return 100;

}
HANDLE hThread = NULL;
void CVCThread01Dlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	CWinThread *pThread = AfxBeginThread(ThreadProct, (LPVOID)456);
	hThread = pThread->m_hThread;
}


void CVCThread01Dlg::OnBnClickedSuspendBtn()
{
	// TODO: Add your control notification handler code here
	SuspendThread(hThread);
}


void CVCThread01Dlg::OnBnClickedResumeBtn()
{
	// TODO: Add your control notification handler code here
	ResumeThread(hThread);
}
