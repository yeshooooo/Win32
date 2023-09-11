// Win32 Dialog
#include <Windows.h>
#include "resource.h"

// 对话框回调函数
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
	// 第二个参数使用微软提供的宏把资源ID转成字符串MAKEINTRESOURCE
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CcTryDialogProc);
	return 0;
}