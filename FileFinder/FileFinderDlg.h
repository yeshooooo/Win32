
// FileFinderDlg.h : header file
//

#pragma once
// �Զ�����Ϣ�����ڴӹ����߳�������߳�֪ͨ����Ϊ�������ڹ����̲߳�������
#define WM_ADD_ITEM (WM_USER+110)

// ����һ�����ڲ����ļ�����

class CFindItem {
public:
	CString m_PathFind; // Ҫ�������ļ�·��
	CString m_FName; // Ҫ�������ļ���
	HWND m_hMainDlg; // �����ڵĴ��ھ�������ڴӹ����߳����䷢����Ϣ
	//���ﹹ��д��д���У���ΪCString����Ҳ�й��캯��
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
	//��������淢����Ϣ����Ϣ��Ӧ��������
	afx_msg LRESULT OnAddItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
