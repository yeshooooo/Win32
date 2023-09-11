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
	switch (uMsg)
	{
	case WM_INITDIALOG:
		MessageBox(hwndDlg, TEXT("�Ի��򴰿ڳ�ʼ�����"), TEXT("Tip"), MB_OK);
		break;
	case WM_COMMAND:
	{
		UINT nCtrlID = LOWORD(wParam);
		UINT nCode = HIWORD(wParam);

		if ( nCode == BN_CLICKED)
		{
			if ( nCtrlID == IDC_BTN_1)
			{
				MessageBox(hwndDlg, TEXT("��ť1�������"), TEXT("Tip"), MB_OK);
			}
			else if ( nCtrlID == IDC_BTN_2)
			{
				MessageBox(hwndDlg, TEXT("��ť2�������"), TEXT("Tip"), MB_OK);
			}
			// ȡ����ť
			else if ( nCtrlID == IDCANCEL)
			{
				int iret = MessageBox(hwndDlg, TEXT("��ȷ��Ҫ�رոô�����"), TEXT("Tip"), MB_OKCANCEL);
				if (iret)
				{
					EndDialog(hwndDlg, 888);
				}
				
			}

			// �����˾�return true;
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
	// �ڶ�������ʹ��΢���ṩ�ĺ����ԴIDת���ַ���MAKEINTRESOURCE
	int nRet = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CcTryDialogProc);
	TCHAR szRet[16] = { 0 };

	return 0;
}