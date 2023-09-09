#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>

// 窗口类名
static TCHAR szWindowClass[] = TEXT("WindowClass"); 

// 窗口标题
static TCHAR szTitle[] = TEXT("我在VC驿站的第一个窗口程序");

// 第五步： 窗口函数中处理消息窗口

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = TEXT("VC驿站的各位网友们，大家好啊！");

	switch (uMsg)
	{
	case WM_CREATE:
		{
			// 创建子控件
			UINT IDC_BUTTON_1 = 200; // 定义Button的ID，传入HMENU参数
			HWND hButtonWnd = CreateWindow(WC_BUTTON, TEXT("我是按钮"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 100,
				100, 50, // 宽和高
				hWnd, // 父窗口
				(HMENU)IDC_BUTTON_1, // 这里没有按钮，但是他可以用作控件ID的作用，
				NULL,
				NULL);
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdSHow)
{
	// 第一步：注册窗口类， 指定窗口类的名称与窗口回调函数
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("窗口类注册失败！"), TEXT("Tip"), NULL);
		return 1;
	}

	// 第二步： 创建窗口，指定注册窗口类，窗口标题，窗口的大小
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("创建窗口失败！"), TEXT("Tip"), NULL);

	}
	// 时机1 ：可以在这里创建子窗口或者控件
	//CreateWindow 创建子窗口

	// 第三步： 显示窗口
	ShowWindow(hWnd, nCmdSHow);

	// 第四步： 开始消息循环
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}