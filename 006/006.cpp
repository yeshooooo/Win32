#include <windows.h>

// 自定义窗口过程，函数签名去DefWindowProc抄就行
LRESULT CALLBACK MyWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// 1. 注册窗口类
	// 我是照着msdn设置 https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/ns-winuser-wndclassa
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = MyWindowProc; //默认窗口函数
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;
	// LoadIcon
	wnd.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	// 这里使用LoadCursor
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	// 这里才用GetStockObject设置
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd.lpszMenuName = L"菜单资源名称";
	wnd.lpszClassName = L"MyWindow"; // 
	RegisterClass(&wnd);

	// 2. 创建窗口(返回之前会发送WM_CREATE消息)
	HWND hwnd = CreateWindow(
		L"MyWindow",
		L"left top title",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		300,
		300,
		NULL,
		NULL,
		hInstance,
		NULL

	);

	// 3. 显示窗口
	ShowWindow(hwnd,  nShowCmd);

	// 4. 更新窗口(更新窗口会发送WM_PAINT消息)
	UpdateWindow(hwnd);

	// 5. 消息循环
	MSG msg;
	//while (GetMessage(&msg, hwnd, 0, 0))
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // 翻译消息
		DispatchMessage(&msg); // 分发消息到窗口回调
	}



}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY: {
		PostQuitMessage(WM_QUIT);
		//exit(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}