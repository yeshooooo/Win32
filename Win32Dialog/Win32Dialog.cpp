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
	switch (uMsg)
	{
	case WM_INITDIALOG:
		MessageBox(hwndDlg, TEXT("对话框窗口初始化完成"), TEXT("Tip"), MB_OK);
		break;
	case WM_COMMAND:
	{
		UINT nCtrlID = LOWORD(wParam);
		UINT nCode = HIWORD(wParam);

		if ( nCode == BN_CLICKED)
		{
			if ( nCtrlID == IDC_BTN_1)
			{
				MessageBox(hwndDlg, TEXT("按钮1被点击了"), TEXT("Tip"), MB_OK);
			}
			else if ( nCtrlID == IDC_BTN_2)
			{
				MessageBox(hwndDlg, TEXT("按钮2被点击了"), TEXT("Tip"), MB_OK);
			}
			// 取消按钮
			else if ( nCtrlID == IDCANCEL)
			{
				int iret = MessageBox(hwndDlg, TEXT("您确定要关闭该窗口码"), TEXT("Tip"), MB_OKCANCEL);
				if (iret)
				{
					EndDialog(hwndDlg, 888);
				}
				
			}

			// 处理了就return true;
			return TRUE;
		}

	}
	default:
		break;
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 第二个参数使用微软提供的宏把资源ID转成字符串MAKEINTRESOURCE
	int nRet = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CcTryDialogProc);
	TCHAR szRet[16] = { 0 };

	return 0;
}