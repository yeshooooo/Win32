
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

// ������Ϣ�ź���Ϣ��Ӧ����
BEGIN_MESSAGE_MAP(CFileFinderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE_BTN, &CFileFinderDlg::OnBnClickedBrowseBtn)
	ON_BN_CLICKED(IDC_START_BTN, &CFileFinderDlg::OnBnClickedStartBtn)
	ON_MESSAGE(WM_ADD_ITEM, &CFileFinderDlg::OnAddItem) // �Զ����
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
	// ΪListControl������ʽ,������ʽ����������ExtendStyle
	m_ResultList.SetExtendedStyle(
		m_ResultList.GetExStyle() // ��֤listcontrol��ǰ����ʽ������
		| LVS_EX_GRIDLINES // ����
		| LVS_EX_FULLROWSELECT // ����ѡ��

	);

	// ����һ��
	m_ResultList.InsertColumn(0, TEXT("���ҽ��"), LVCFMT_LEFT, 1050); // ����룬���1050���Ҳ�Ҫ����λ�ø�������

	SetTimer(1, 100, NULL); // ������ʱ��
	// �ڵ���ģʽ�¸������ı������Ĭ��ֵ
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


// ����ͳ���ļ�������ȫ�ֱ���
LONG g_FindCount;

// ͳ���̸߳���
LONG g_ThreadNum;
// ���浱ǰ��������״̬��ȫ�ֱ���������ֹͣ��ť�İ�ȫ����
BOOL bFinding = FALSE;
// �����ť���ܵ����
// vc++�������16��
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

// ���Ұ�ť
UINT __cdecl FindProc(LPVOID pParam)
{
	CFindItem* pitem = (CFindItem*)pParam;

	// ����ö���ļ����࣬ CFileFind
	CString strRootPath = pitem->m_PathFind;
	// Ҫ�жϽ�β��û��\��û�õĻ�Ҫ�������ϣ���Ϊ��Ҫ����ƴ��*.*
	if (strRootPath[strRootPath.GetLength() - 1] != '\\')
	{
		strRootPath += TEXT("\\");
	}
	CFileFind mFinder;
	BOOL bFind = mFinder.FindFile(strRootPath + _T("*.*"));// �ڶ�������û�ã���һ�����ļ�·��
	while (bFind && bFinding)
	{
		bFind = mFinder.FindNextFileW();
		if (mFinder.IsDots())
		{
			// �ҵ�����..
			continue;
		} else if (mFinder.IsDirectory()) // ����ҵ�����Ŀ¼���������߳�
		{
			// ����
			CFindItem* pSubItem = new CFindItem(*pitem);
			pSubItem->m_PathFind = mFinder.GetFilePath();
			AfxBeginThread(FindProc, pSubItem);


		}
		else
		{
			// ˵���ҵ������ļ�,���бȶ�
			CString strFileName = mFinder.GetFileName();
			if (strFileName.CompareNoCase(pitem->m_FName) == 0)
			{
				// ����ҵ��ˣ������+1
				InterlockedIncrement(&g_FindCount);
				g_ThreadNum += 1;
				CString strPath = mFinder.GetFilePath();
				// ������Ϸ���Ϣ�����������ڹ����߳��ϲ�������
				// ·��ͨ��wParam����lParam���͹�ȥ
				//����PostMessage����,�������
				// ��ΪPostMessage���첽�ģ������ͳ�ȥ�˲����㴦����û�У����Ͼͻ᷵��
				//PostMessage(pitem->m_hMainDlg, WM_ADD_ITEM, 0, (LPARAM)(LPCTSTR)strPath);

				// SendMessage��ͬ����SendMessage�ǷǶ�����Ϣ
				SendMessage(pitem->m_hMainDlg, WM_ADD_ITEM, 0, (LPARAM)(LPCTSTR)strPath);
			}
		}
	}

	//������е��ڴ�
	delete pitem;
	// �˳��̵߳�ʱ���߳�����-1
	InterlockedDecrement(&g_ThreadNum);
	return 0;
}
void CFileFinderDlg::OnBnClickedStartBtn()
{
	// TODO: Add your control notification handler code here
	// ÿ�ε�����Ұ�ť��ʱ�򣬸�������
	g_FindCount = 0;
	g_ThreadNum = 0;
	bFinding = TRUE;
	// ÿ�ε�����Ұ�ť������¸߼��б�ؼ�
	m_ResultList.DeleteAllItems();
	// ����ı��˼����ÿ����һ��Ŀ¼�Ϳ���һ���߳�

		//0. �Զ������,�ӽ�������ֵ
	// ����Ҫ�ڶ��������ڴ棬����ť�������˾ͱ��Զ�������,�����ʱ��������ص������
	CFindItem *pitem = new CFindItem(); 
	GetDlgItemText(IDC_PATH_EDIT, pitem->m_PathFind);
	GetDlgItemText(IDC_FNAME_EDIT, pitem->m_FName);
	//�ѵ�ǰ�Ի���Ĵ��ھ������ȥ������ͨ��,���̺߳�����ʹ��
	pitem->m_hMainDlg = GetSafeHwnd();
	// 1. �������߳�
	// ��Ϊ����ƽ���Ķ��������������̼߳���
	AfxBeginThread(FindProc, pitem); // �첽��


	


	

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
	// �ö�ʱ��
	if (g_ThreadNum > 0)
	{
		// �̴߳���0 ���ñ༭����
		GetDlgItem(IDC_PATH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FNAME_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_BTN)->EnableWindow(TRUE); // ֹͣ��ť����
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
