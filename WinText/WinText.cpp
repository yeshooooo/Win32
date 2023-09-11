#include <windows.h>
HANDLE g_hOutput = 0;
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);

	//设置颜色和背景
	SetTextColor(hdc, RGB(255, 0, 255));
	SetBkColor(hdc, RGB(0, 255, 0));
	// 通过设置背景模式设置背景色透明的，SetBkColor在TRANSPARENT模式下失效，只适用于OPAQUE不透明模式
	SetBkMode(hdc, TRANSPARENT); //颜色跟背景一体

	// 创建字体
	HFONT hFont = CreateFont(
		30, //高度给30
		0, // 宽度给0，会按高度匹配宽度
		45, // 倾斜角度
		0, // 选择角度，二维没有
		900, // 字体粗细
		1, // 是不是斜体
		1, //要不要下划线
		1, // 要不要删除线
		GB2312_CHARSET, // 字符集
		0, 0, 0, 0, //废弃参数
		"黑体" //字体名字，双击字体文件第一行看

	);
	HGDIOBJ nOldFont =  SelectObject(hdc, hFont); //送给画家，并接受原来的字体

	char szText[] = "hello Text";
	TextOut(hdc, 100, 100, szText, strlen(szText));
	RECT rc;
	rc.left = 100;
	rc.top = 150;
	rc.right = 200;
	rc.bottom = 200;
	Rectangle(hdc, 100, 150, 200, 200); //绘制rc确定的矩形范围
	// DT_VCENTER和DT_BOTTOM只适用于DT_SINGLELINE，他俩和DT_WORDBREAK冲突
	DrawText(hdc, szText, strlen(szText), &rc, 
		//DT_LEFT | DT_TOP
	DT_CENTER | DT_VCENTER // 水平和垂直都居中，垂直居中只适合单行，他和DT_WORDBREAK冲突
	| DT_WORDBREAK // 多行绘制
	| DT_NOCLIP // 放不开的时候不裁剪，在外面的也画
	);

	// 画完后把自己要回来
	SelectObject(hdc, nOldFont);
	DeleteObject(hFont); // 销毁字体内存
	EndPaint(hWnd, &ps);


}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
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
	//AllocConsole();
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