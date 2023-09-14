// CUIThreadApp.cpp : implementation file
//

#include "pch.h"
#include "VCThread01.h"
#include "CUIThreadApp.h"
#include "CTestDlg.h"

// CUIThreadApp

IMPLEMENT_DYNCREATE(CUIThreadApp, CWinThread)

CUIThreadApp::CUIThreadApp()
{
}

CUIThreadApp::~CUIThreadApp()
{
}

BOOL CUIThreadApp::InitInstance()
{
	//CTestDlg dlg;
	//dlg.DoModal();
	// TODO:  perform and per-thread initialization here

	//return FALSE; // return TRUE后不会调用析构和ExitInstance，而是跳到.Run上执行

	// 创建非模态对话框
	CTestDlg* pTestDlg = new CTestDlg();
	pTestDlg->Create(IDD_DIALOG1);
	pTestDlg->ShowWindow(SW_SHOW);

	return TRUE; // 这时候要返回TRUE，因为要运行自带的消息循环
}

int CUIThreadApp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThreadApp, CWinThread)
END_MESSAGE_MAP()


// CUIThreadApp message handlers
