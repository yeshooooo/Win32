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
			MessageBox(hWnd, "新建菜单项被点击", "Info", MB_OK);
		}
		else if (HIWORD(wParam) == 1) {
			MessageBox(hWnd, "加速键Ctrl+M被按下", "Info", MB_OK);
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
		// 这里只能改一瞬间，因为如果不return的话，下面的DefWindowProc又会自动改回注册窗口类的时候设置的类型
		HCURSOR hCur = LoadCursor(g_hInstance, (LPCTSTR)IDC_CURSOR2);
		// 客户区域设置成黑方块
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(hCur);
			return 0;
		}
		// 非客户区活动不用设置，他就走到了默认处理函数那里，被重置为注册窗口类的时候设置的样式
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

	// 挂菜单
	
	wc.lpszMenuName = (char*)IDR_MENU1;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 将以上所有赋值全部写入操作系统
	RegisterClass(&wc);

	// 在内存中创建窗口
	char szTitle[256] = { 0 };
	LoadString(hInstance, IDS_WND, szTitle, 256);
	HWND hWnd = CreateWindowEx(0, "Main", szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// 显示窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环
	MSG nMsg = { 0 };
	HACCEL hAccel = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR1);
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		// 快捷键应该放在TranslateMessage前面，否则就会区分大小写了，这里不需要区分大小写
		if (!TranslateAccelerator(hWnd, hAccel, &nMsg))
		{
			// 不是加速键，就进来，该翻译翻译，该转发转发
			TranslateMessage(&nMsg);
			DispatchMessage(&nMsg);
		}

	}


}