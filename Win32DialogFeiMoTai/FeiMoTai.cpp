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
		//MessageBox(hwndDlg, TEXT("�Ի��򴰿ڳ�ʼ�����"), TEXT("Tip"), MB_OK);
		break;
	case WM_COMMAND:
	{
		UINT nCtrlID = LOWORD(wParam);
		UINT nCode = HIWORD(wParam);

		if (nCode == BN_CLICKED)
		{
			if (nCtrlID == IDC_BUTTON1)
			{
				//����ģ̬
				DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_CHILD), hwndDlg, DialogProc);
			}
			else if (nCtrlID == IDC_BUTTON2)
			{
				// ������ģ̬
				HWND hSubWnd =  CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_CHILD), hwndDlg, DialogProc);
				ShowWindow(hSubWnd, SW_SHOW);

			}

			// ȡ����ť
			else if (nCtrlID == IDCANCEL)
			{
				int iret = MessageBox(hwndDlg, TEXT("��ȷ��Ҫ�رոô�����"), TEXT("Tip"), MB_OKCANCEL);
				if (iret)
				{
					DestroyWindow(hwndDlg); //��ģ̬�Ի�����DestroyWindow�ر�
					PostQuitMessage(0); // ���ҷ����˳���Ϣ��������Ϣѭ�����ܽ���
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
	// ��������ʾ�Ի���(��ģ̬�Ի���ͬ)
	// ����ֵ�Ƿ�ģ̬�Ի���Ĵ��ھ��
	HWND hWndDlg =  CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(hWndDlg,SW_SHOW); //�÷�ģ̬�Ի�����ʾ
	
	// ���������Ϣѭ������Ȼ�����ˣ������ⴰ�ڵ��Ӵ��ڲ��ü���Ϣѭ����
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}