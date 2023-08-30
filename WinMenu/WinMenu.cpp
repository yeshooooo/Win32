#include <windows.h>
#include "resource.h"
HANDLE g_hOutput = 0;
void OnPaint(HWND hWnd) {
	const char* pszText = "WM_PAINT\n";
	WriteConsole(g_hOutput, pszText, strlen(pszText), NULL, NULL);


}

HINSTANCE g_hInstance = 0;
void OnCreate(HWND hWnd) {
	HMENU myMenu = LoadMenu(g_hInstance, (LPCTSTR)IDR_MENU1);
	SetMenu(hWnd, myMenu);
}

void OnCommand(HWND hWnd, WPARAM wParam) {
	switch (LOWORD(wParam))
	{
	case ID_NEW:
		MessageBox(hWnd, "�½������", "Info", MB_OK);
		break;
	case ID_EXIT:
		MessageBox(hWnd, "�˳������", "Info", MB_OK);
		break;

	case ID_ABOUT:
		MessageBox(hWnd, "���ڱ����", "Info", MB_OK);
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
	case WM_CREATE:
		OnCreate(hWnd);
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
	wc.hCursor = NULL;
	wc.hInstance = hInstance;
	wc.lpszClassName = "Main";

	// ����������ز˵�
	//wc.lpszMenuName = (char*)IDR_MENU1;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ���������и�ֵȫ��д�����ϵͳ
	RegisterClass(&wc);

	// ���ڴ��д�������
	HWND hWnd = CreateWindowEx(0, "Main", "windows", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// ���ز˵���Դ
	//HMENU myMenu = LoadMenu(hInstance, (LPCTSTR)IDR_MENU1);
	//HWND hWnd = CreateWindowEx(0, "Main", "windows", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, myMenu, hInstance, NULL);

	// ��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}


}