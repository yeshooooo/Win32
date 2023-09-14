// CTestDlg.cpp : implementation file
//

#include "pch.h"
#include "VCThread01.h"
#include "CTestDlg.h"
#include "afxdialogex.h"


// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTestDlg message handlers


void CTestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	PostQuitMessage(0);
	CDialog::OnCancel();
}
