#include <windows.h>
#include "resource1.h"
HANDLE g_hOutput = 0;

// 保存WinMain的第一个参数供加载位图使用
HINSTANCE g_hInstance = 0;
// 画点的函数
void DrawPit(HDC hdc)
{
	//SetPixel(hdc, 100, 100, RGB(255, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			SetPixel(hdc, i, j, RGB(i, j, 0));
		}
	}
}

// 画线
void DrawLine(HDC hdc)
{
	MoveToEx(hdc, 100, 100, NULL); 
	LineTo(hdc, 300, 300);
	LineTo(hdc, 0, 300);
}

// 画矩形
void DrawRect(HDC hdc)
{
	Rectangle(hdc, 100, 100, 300, 300); // 左，上，右，下
}

// 画圆形
void DrawEll(HDC hdc)
{
	Ellipse(hdc, 100, 100, 300, 300); //圆形的参数是外接矩形
}

// 绘制位图
void DrawBmp(HDC hdc)
{
	// 1. 添加位图资源(猪头)

	// 2. 加载位图(到本进程的内存资源中去找
	// 不得已设置全局变量
	HBITMAP hBmp =  LoadBitmap(g_hInstance, (LPCTSTR)IDB_BITMAP1);
	// 3. 创建和当前DC匹配的虚拟内存DC,并且构建一个虚拟区域，并且内存DC在虚拟区域中绘图
	HDC hMemdc = CreateCompatibleDC(hdc);
	// 4. SelectObject 将位图数据送给内存DC
	// 内存DC在虚拟区域中将位图绘制出来
	// 返回的是老的位图，这里是假的，因为原来没有，为了后面将真的换回来
	HGDIOBJ nOldBmp =  SelectObject(hMemdc, hBmp);

	// 5. 成像
	// 1:1 成像
	BitBlt(
		hdc, // 目的dc
		100, 100, // 目的左上角
		48, 48, // 目的大小
		hMemdc, // 源DC
		0, 0, // 从源的什么位置开始拷贝
		SRCCOPY // 成像方式，原样成像
	);

	// 缩放成像
	StretchBlt(
		hdc, // 目的dc
		200, 200, // 窗口成像位置
		24, 24, // 成像区域大小，也能放大成像，改变成像目标区域大小即可
		hMemdc, // 源dc
		0, 0, //从源的什么位置开始拷贝
		48, 48, // 拷贝多少成像
		SRCCOPY // 成像方式，原样成像
	);


	// 6. 把位图图像要回来
	SelectObject(hMemdc, nOldBmp);
	// 7-8 释放内存
	DeleteObject(hBmp);
	DeleteDC(hMemdc);

}
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 }; 
	// ps结构体里的成员是系统api内部使用的
	HDC hdc =  BeginPaint(hWnd, &ps);
	// 使用GDI画笔对象画线
	// PS_DASH 虚线
	// PS_SOLID 实线，只有实线可以设置任意像素，其他设置非1值会失效，变回实线
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
	HGDIOBJ nOldPen = SelectObject(hdc, hPen);

	// 画刷
	//HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0)); // 实线画刷
	//HBRUSH hBrush = CreateHatchBrush(HS_CROSS, RGB(0, 255, 0)); // 第一个参数是纹理线的样式
		// 使用操作系统预设的画刷
	HGDIOBJ hBrush = GetStockObject(NULL_BRUSH);
	HGDIOBJ nOldBrush = SelectObject(hdc, hBrush);




	//画点
	//DrawPit(hdc);

	// 画线
	//DrawLine(hdc);

	// 画矩形
	//DrawRect(hdc);

	// 画圆形
	//DrawEll(hdc);

	// 绘制位图
	DrawBmp(hdc);

	SelectObject(hdc, nOldPen); // 换回老画笔才能销毁
	DeleteObject(hPen);
	SelectObject(hdc, nOldBrush); // 换回老画刷
	//DeleteObject(hBrush); // 系统的刷子不用销毁



	EndPaint(hWnd, &ps);


}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
		//绘制图形一定要在绘图消息里绘制
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

	//
	AllocConsole();
	//g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	g_hInstance = hInstance;
	//
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3); // 修改窗口背景颜色 +1 为白色， +3 为黑色
	wc.hCursor = NULL;
	wc.hInstance = hInstance;
	wc.lpszClassName = "Main";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
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