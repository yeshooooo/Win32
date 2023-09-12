
// VCThread01Dlg.h : header file
//

#pragma once


// CVCThread01Dlg dialog
class CVCThread01Dlg : public CDialogEx
{
// Construction
public:
	CVCThread01Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VCTHREAD01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	int m_Num;
};
