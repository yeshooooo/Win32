#include <windows.h>
#include "resource.h"
HANDLE g_hOutput = 0;
HINSTANCE g_hInstance = 0;
void OnPaint(HWND hWnd) {
	const char* pszText = "WM_PAINT\n";
	WriteConsole(g_hOutput, pszText, strlen(pszText), NULL, NULL);


}

void OnCommand(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_NEW:
		if (HIWORD(wParam == 0))
		{
			MessageBox(hWnd, "�½��˵�����", "Info", MB_OK);
		}
		else if (HIWORD(wParam) == 1) {
			MessageBox(hWnd, "���ټ�Ctrl+M������", "Info", MB_OK);
		}
		
		break;
	default:
		break;
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
	case WM_COMMAND:
		OnCommand(hWnd, wParam);
		break;
	case WM_SETCURSOR:
	{
		// ����ֻ�ܸ�һ˲�䣬��Ϊ�����return�Ļ��������DefWindowProc�ֻ��Զ��Ļ�ע�ᴰ�����ʱ�����õ�����
		HCURSOR hCur = LoadCursor(g_hInstance, (LPCTSTR)IDC_CURSOR2);
		// �ͻ��������óɺڷ���
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(hCur);
			return 0;
		}
		// �ǿͻ�����������ã������ߵ���Ĭ�ϴ��������������Ϊע�ᴰ�����ʱ�����õ���ʽ
		else
		{

		}
		
	}

		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdline, int nShowCmd) {

	g_hInstance = hInstance;

	//
	AllocConsole();
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(hInstance, (LPCTSTR)IDC_CURSOR1);

	wc.hIcon = LoadIcon(hInstance, (char*)IDI_ICON1);
	wc.hInstance = hInstance;
	wc.lpszClassName = "Main";

	// �Ҳ˵�
	
	wc.lpszMenuName = (char*)IDR_MENU1;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ���������и�ֵȫ��д�����ϵͳ
	RegisterClass(&wc);

	// ���ڴ��д�������
	char szTitle[256] = { 0 };
	LoadString(hInstance, IDS_WND, szTitle, 256);
	HWND hWnd = CreateWindowEx(0, "Main", szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// ��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
	MSG nMsg = { 0 };
	HACCEL hAccel = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR1);
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		// ��ݼ�Ӧ�÷���TranslateMessageǰ�棬����ͻ����ִ�Сд�ˣ����ﲻ��Ҫ���ִ�Сд
		if (!TranslateAccelerator(hWnd, hAccel, &nMsg))
		{
			// ���Ǽ��ټ����ͽ������÷��뷭�룬��ת��ת��
			TranslateMessage(&nMsg);
			DispatchMessage(&nMsg);
		}

	}


}