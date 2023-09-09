#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>

// ��������
static TCHAR szWindowClass[] = TEXT("WindowClass"); 

// ���ڱ���
static TCHAR szTitle[] = TEXT("����VC��վ�ĵ�һ�����ڳ���");

// ���岽�� ���ں����д�����Ϣ����

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = TEXT("VC��վ�ĸ�λ�����ǣ���Һð���");

	switch (uMsg)
	{
	case WM_CREATE:
		{
			// �����ӿؼ�
			UINT IDC_BUTTON_1 = 200; // ����Button��ID������HMENU����
			HWND hButtonWnd = CreateWindow(WC_BUTTON, TEXT("���ǰ�ť"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 100,
				100, 50, // ��͸�
				hWnd, // ������
				(HMENU)IDC_BUTTON_1, // ����û�а�ť�����������������ؼ�ID�����ã�
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
	// ��һ����ע�ᴰ���࣬ ָ��������������봰�ڻص�����
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("������ע��ʧ�ܣ�"), TEXT("Tip"), NULL);
		return 1;
	}

	// �ڶ����� �������ڣ�ָ��ע�ᴰ���࣬���ڱ��⣬���ڵĴ�С
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
		MessageBox(NULL, TEXT("��������ʧ�ܣ�"), TEXT("Tip"), NULL);

	}
	// ʱ��1 �����������ﴴ���Ӵ��ڻ��߿ؼ�
	//CreateWindow �����Ӵ���

	// �������� ��ʾ����
	ShowWindow(hWnd, nCmdSHow);

	// ���Ĳ��� ��ʼ��Ϣѭ��
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}