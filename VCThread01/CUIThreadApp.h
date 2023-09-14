#pragma once



// CUIThreadApp

class CUIThreadApp : public CWinThread
{
	DECLARE_DYNCREATE(CUIThreadApp)

protected:
	CUIThreadApp();           // protected constructor used by dynamic creation
	virtual ~CUIThreadApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


