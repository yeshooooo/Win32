#include <windows.h>
#include <stdio.h>
HANDLE g_hOutput = 0;
void OnPaint(HWND hWnd) {
	const char* pszText = "WM_PAINT\n";
	WriteConsole(g_hOutput, pszText, strlen(pszText), NULL, NULL);


	// 绘图代码，必须放在处理WM_PAINT消息时调用。
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);
	TextOut(hdc, 100, 100, "hello", 5);
	EndPaint(hWnd, &ps);



}


void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_LBUTTONDOWN: 其他按键状态:%d, X = %d, Y= %d \n", wParam, LOWORD(lParam),
		HIWORD(lParam));
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

}

void OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_LBUTTONUP: 其他按键状态:%d, X = %d, Y= %d \n", wParam, LOWORD(lParam),
		HIWORD(lParam));
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

}

void OnKeyDown(HWND hWnd, WPARAM wParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_KEYDOWN:键码值:=%d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

}
void OnKeyUp(HWND hWnd, WPARAM wParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_KEYUP:键码值:=%d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnChar(HWND hWnd, WPARAM wParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_CHAR:wParam传递的ascii码为:=%d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

}

void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_MOUSEMOVE: 其他按键状态:%d, X = %d, Y= %d \n", wParam, LOWORD(lParam),
		HIWORD(lParam));
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

}

void OnLButtonDblClk(HWND hWnd) {
	const char* szText = "WM_LBUTTONDBLCLK\n";
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);


}
void OnMouseWheel(HWND hWnd, WPARAM wParam) {
	short nDelta = HIWORD(wParam); // 偏移量
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_MOUSEWHEEL: nDetal = %d\n", nDelta);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);


}

void OnTimer(HWND hWnd, WPARAM wParam) {
	//switch (wParam)
	//{
	//case 1:
	//	break;
	//case 2:
	//	break;
	//default:
	//	break;
	//}
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_TIMER: 定时器ID = %d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);


}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
	//case WM_PAINT:
	//	OnPaint(hWnd);
	//	break;
	//case WM_LBUTTONDOWN:
	//	InvalidateRect(hWnd, NULL, TRUE);
	//	break;

	//
	case WM_TIMER:
		OnTimer(hWnd, wParam);
		break;
	// 窗口创建成功之后，显示之前
	case WM_CREATE:
		// 这两个定时器到点就交给WM_TIMER情况下的函数处理
		SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 2000, NULL);

		break;
	
	// 
	case WM_MOUSEWHEEL:
		OnMouseWheel(hWnd, wParam);
		break;

	// 键盘
	case WM_CHAR:
		OnChar(hWnd, wParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd,wParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, wParam);
		break;

	// 鼠标
	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, wParam, lParam);
		break;
	// 鼠标移动
	//case WM_MOUSEMOVE:
	//	OnMouseMove(hWnd, wParam, lParam);
	//	break;

	// 左键双击
	case WM_LBUTTONDBLCLK:
		OnLButtonDblClk(hWnd);
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
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // CS_DBLCLKS 不加的话双击没反应
	// 将以上所有赋值全部写入操作系统
	RegisterClass(&wc);

	// 在内存中创建窗口
	HWND hWnd = CreateWindowEx(0, "Main", "windows", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// 显示窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}


}