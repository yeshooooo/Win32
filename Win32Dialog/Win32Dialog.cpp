// Win32 Dialog
#include <Windows.h>
#include "resource.h"

// �Ի���ص�����
INT_PTR CALLBACK CcTryDialogProc(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ڶ�������ʹ��΢���ṩ�ĺ����ԴIDת���ַ���MAKEINTRESOURCE
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CcTryDialogProc);
	return 0;
}