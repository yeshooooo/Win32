#include <windows.h>
#include <stdio.h>
#include <iostream>

HANDLE g_hOutput = 0; // 接受标准输出句柄
// 自定义窗口过程，函数签名去DefWindowProc抄就行
LRESULT CALLBACK MyWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// 自定义消息
// WM_USER就是0x400因为一般不直接写0x400
#define WM_MYMESSAGE WM_USER+1001

void OnCreate(HWND hWnd, LPARAM lParam);
void OnSize(HWND hWnd, LPARAM lParam);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	AllocConsole();// 增加dos窗口

	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // 拿到标准输出句柄
	
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
	// 
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
	// GetMessage会阻塞，效率太低
	//while (GetMessage(&msg, hwnd, 0, 0))
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg); // 翻译消息
	//	DispatchMessage(&msg); // 分发消息到窗口回调
	//}
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// 有消息
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// GetMessage(&msg, NULL, 0, 0)的天敌WM_QUIT
			else
			{
				return 0;
			}
		}
		else
		{
			// 没消息
			// 体面的说法叫空闲处理

		}
	}



}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// lParam低位字节传递的是水平坐标，高位字节传递的是垂直坐标
	switch (Msg)
	{
	case WM_SIZE:
		OnSize(hWnd, lParam);
		break;
	case WM_CREATE:
		OnCreate(hWnd, lParam);
		break;
	case WM_DESTROY: 
		//PostQuitMessage(0); 
		// 
		//exit(0);
		// 试验PostQuitMessage底层调用的是SendMessage还是PostMessage
		//SendMessage(hWnd, WM_QUIT, 0, 0);
		PostMessage(hWnd, WM_QUIT, 0, 0);
		return 0;
	
	// 点击最大化，最小化，关闭等都会直接产生这个消息
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet =  MessageBox(hWnd, TEXT("是否退出？"), TEXT("Info"), MB_YESNO);
			if (nRet ==IDYES)
			{
				// 什么都不写就会穿透到default下最终退出
			}
			else
			{
				return 0; // return 啥都行，目的是不让DefWindowProc执行
			}
		}
		
	default:
		// 这俩会再处理一遍WM_SYSCOMMAND，产生WM_DESTROY消息，再回上面的
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

void OnCreate(HWND hWnd, LPARAM lParam)
{
	MessageBox(NULL, TEXT("WM_CREATE"), TEXT("Info"), MB_OK);

	CreateWindow(L"Edit", L"hello", WS_CHILD|WS_VISIBLE|WS_BORDER, 0, 0, 200, 200, hWnd, NULL, 0, NULL);
}
void OnSize(HWND hWnd, LPARAM lParam) {
	// 拿到变化后的宽跟高
	short nWidth = LOWORD(lParam);
	short nHeight = HIWORD(lParam);

	char szText[256] = { 0 };
	sprintf_s(szText, "WM_SIZE: 宽：%d,高：%d\n", nWidth, nHeight);
	//AllocConsole();
	FILE* stream;
	freopen_s(&stream,"CONOUT$", "w", stdout);

	printf("i的值为%s\n", szText);
	//FreeConsole();
	//WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
	
}