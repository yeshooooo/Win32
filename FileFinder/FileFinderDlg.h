
// FileFinderDlg.h : header file
//

#pragma once
// 自定义消息，用于从工作线程向界面线程通知，因为不建议在工作线程操作界面
#define WM_ADD_ITEM (WM_USER+110)

// 声明一个用于查找文件的类

class CFindItem {
public:
	CString m_PathFind; // 要搜索的文件路径
	CString m_FName; // 要搜索的文件名
	HWND m_hMainDlg; // 主窗口的窗口句柄，用于从工作线程向其发送消息
	//这里构造写不写都行，因为CString本身也有构造函数
	CFindItem() {
		m_PathFind.Empty();
		m_FName.Empty();
		m_hMainDlg = NULL;
	}

	CFindItem(CFindItem &tItem) {
		m_PathFind = tItem.m_PathFind;
		m_FName = tItem.m_FName;
		m_hMainDlg = tItem.m_hMainDlg;
	}
};

// CFileFinderDlg dialog
class CFileFinderDlg : public CDialog
{
// Construction
public:
	CFileFinderDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEFINDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowseBtn();
	afx_msg void OnBnClickedStartBtn();
	CListCtrl m_ResultList;
	//用于向界面发送消息的消息响应函数声明
	afx_msg LRESULT OnAddItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
