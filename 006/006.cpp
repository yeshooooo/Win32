#include <windows.h>

// �Զ��崰�ڹ��̣�����ǩ��ȥDefWindowProc������
LRESULT CALLBACK MyWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// 1. ע�ᴰ����
	// ��������msdn���� https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/ns-winuser-wndclassa
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = MyWindowProc; //Ĭ�ϴ��ں���
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;
	// LoadIcon
	wnd.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	// ����ʹ��LoadCursor
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	// �������GetStockObject����
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd.lpszMenuName = L"�˵���Դ����";
	wnd.lpszClassName = L"MyWindow"; // 
	RegisterClass(&wnd);

	// 2. ��������(����֮ǰ�ᷢ��WM_CREATE��Ϣ)
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

	// 3. ��ʾ����
	ShowWindow(hwnd,  nShowCmd);

	// 4. ���´���(���´��ڻᷢ��WM_PAINT��Ϣ)
	UpdateWindow(hwnd);

	// 5. ��Ϣѭ��
	MSG msg;
	//while (GetMessage(&msg, hwnd, 0, 0))
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // ������Ϣ
		DispatchMessage(&msg); // �ַ���Ϣ�����ڻص�
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