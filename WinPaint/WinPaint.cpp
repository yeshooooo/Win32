#include <windows.h>
HANDLE g_hOutput = 0;
void OnPaint(HWND hWnd) {
	const char* pszText = "WM_PAINT\n";
	WriteConsole(g_hOutput, pszText, strlen(pszText),NULL,NULL);


	// ��ͼ���룬������ڴ���WM_PAINT��Ϣʱ���á�
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);
	TextOut(hdc, 100, 100, "hello", 5);
	EndPaint(hWnd, &ps);



}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_LBUTTONDOWN:
		InvalidateRect(hWnd, NULL, TRUE);
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
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ���������и�ֵȫ��д�����ϵͳ
	RegisterClass(&wc);

	// ���ڴ��д�������
	HWND hWnd = CreateWindowEx(0, "Main", "windows", WS_OVERLAPPEDWINDOW,100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// ��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0,0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}


}