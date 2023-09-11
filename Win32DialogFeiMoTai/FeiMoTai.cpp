#include <Windows.h>
#include "resource.h"
INT_PTR CALLBACK DialogProc(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		//MessageBox(hwndDlg, TEXT("对话框窗口初始化完成"), TEXT("Tip"), MB_OK);
		break;
	case WM_COMMAND:
	{
		UINT nCtrlID = LOWORD(wParam);
		UINT nCode = HIWORD(wParam);

		if (nCode == BN_CLICKED)
		{
			if (nCtrlID == IDC_BUTTON1)
			{
				//创建模态
				DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_CHILD), hwndDlg, DialogProc);
			}
			else if (nCtrlID == IDC_BUTTON2)
			{
				// 创建非模态
				HWND hSubWnd =  CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_CHILD), hwndDlg, DialogProc);
				ShowWindow(hSubWnd, SW_SHOW);

			}

			// 取消按钮
			else if (nCtrlID == IDCANCEL)
			{
				int iret = MessageBox(hwndDlg, TEXT("您确定要关闭该窗口码"), TEXT("Tip"), MB_OKCANCEL);
				if (iret)
				{
					DestroyWindow(hwndDlg); //非模态对话框用DestroyWindow关闭
					PostQuitMessage(0); // 并且发送退出消息，否则消息循环不能结束
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
	// 创建与显示对话框(与模态对话框不同)
	// 返回值是非模态对话框的窗口句柄
	HWND hWndDlg =  CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(hWndDlg,SW_SHOW); //让非模态对话框显示
	
	// 必须加上消息循环，不然会闪退（在另外窗口的子窗口不用加消息循环）
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}