
// VCThreadSyncDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "VCThreadSync.h"
#include "VCThreadSyncDlg.h"
#include "afxdialogex.h"

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


// CVCThreadSyncDlg dialog



CVCThreadSyncDlg::CVCThreadSyncDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VCTHREADSYNC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCThreadSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVCThreadSyncDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CVCThreadSyncDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVCThreadSyncDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVCThreadSyncDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CVCThreadSyncDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CVCThreadSyncDlg message handlers

BOOL CVCThreadSyncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CVCThreadSyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCThreadSyncDlg::OnPaint()
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
HCURSOR CVCThreadSyncDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
int g_Num = 0;
UINT __cdecl ThreadProc(LPVOID lpParameter)
{
	for (int i = 0; i < 100; ++i)
	{
		//g_Num = g_Num + 1;
		InterlockedIncrement((long*)&g_Num);
		CString strNum;
		strNum.Format(TEXT("%d"), g_Num);
		//g_Num = g_Num - 1;
		InterlockedDecrement((long*)&g_Num);
	}
	return 0;
}


void CVCThreadSyncDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	for (int i = 1; i <=50; ++i)
	{
		AfxBeginThread(ThreadProc, NULL);
	}
}


void CVCThreadSyncDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	int realNum = g_Num;
}

CStringArray g_ArrString;
CRITICAL_SECTION g_CS;

UINT __cdecl ThreadProcCS(LPVOID lpParameter)
{
	int startIdx = (int)lpParameter;
	for (int idx = startIdx; idx < startIdx + 100; ++idx)
	{
		CString str;
		str.Format(_T("%d"), idx);

		//进入临界区
		EnterCriticalSection(&g_CS);
		g_ArrString.Add(str);
		// 离开临界区
		LeaveCriticalSection(&g_CS);
	}
	return 0;
}

//
void CVCThreadSyncDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	// 创建线程之前进行初始化临界区
	InitializeCriticalSection(&g_CS);
	for (int idx = 1; idx <=50; ++idx)
	{
		AfxBeginThread(ThreadProcCS, (LPVOID)(idx * 10));
	}

}


// 临界区查看结果
void CVCThreadSyncDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString strCount;
	INT_PTR nCount = g_ArrString.GetCount();
	strCount.Format(TEXT("%d"), nCount);
	MessageBox(strCount);

	for (INT_PTR idx = 0; idx < nCount; ++idx)
	{
		//OutputDebugString(g_ArrString.GetAt(nCount)); //win10 vs2019这里会异常
		nCount = nCount; //自己随便加的
	}
	DeleteCriticalSection(&g_CS);
}
