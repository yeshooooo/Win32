
// VCThread01.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVCThread01App:
// See VCThread01.cpp for the implementation of this class
//

class CVCThread01App : public CWinApp
{
public:
	CVCThread01App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CVCThread01App theApp;
