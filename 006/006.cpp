#include <windows.h>
#include <stdio.h>
#include <iostream>

HANDLE g_hOutput = 0; // ���ܱ�׼������
// �Զ��崰�ڹ��̣�����ǩ��ȥDefWindowProc������
LRESULT CALLBACK MyWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// �Զ�����Ϣ
// WM_USER����0x400��Ϊһ�㲻ֱ��д0x400
#define WM_MYMESSAGE WM_USER+1001

void OnCreate(HWND hWnd, LPARAM lParam);
void OnSize(HWND hWnd, LPARAM lParam);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	AllocConsole();// ����dos����

	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // �õ���׼������
	
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

	// 3. ��ʾ����
	ShowWindow(hwnd,  nShowCmd);

	// 4. ���´���(���´��ڻᷢ��WM_PAINT��Ϣ)
	UpdateWindow(hwnd);

	// 5. ��Ϣѭ��
	MSG msg; 
	// GetMessage��������Ч��̫��
	//while (GetMessage(&msg, hwnd, 0, 0))
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg); // ������Ϣ
	//	DispatchMessage(&msg); // �ַ���Ϣ�����ڻص�
	//}
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// ����Ϣ
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// GetMessage(&msg, NULL, 0, 0)�����WM_QUIT
			else
			{
				return 0;
			}
		}
		else
		{
			// û��Ϣ
			// �����˵���п��д���

		}
	}



}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// lParam��λ�ֽڴ��ݵ���ˮƽ���꣬��λ�ֽڴ��ݵ��Ǵ�ֱ����
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
		// ����PostQuitMessage�ײ���õ���SendMessage����PostMessage
		//SendMessage(hWnd, WM_QUIT, 0, 0);
		PostMessage(hWnd, WM_QUIT, 0, 0);
		return 0;
	
	// �����󻯣���С�����رյȶ���ֱ�Ӳ��������Ϣ
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet =  MessageBox(hWnd, TEXT("�Ƿ��˳���"), TEXT("Info"), MB_YESNO);
			if (nRet ==IDYES)
			{
				// ʲô����д�ͻᴩ͸��default�������˳�
			}
			else
			{
				return 0; // return ɶ���У�Ŀ���ǲ���DefWindowProcִ��
			}
		}
		
	default:
		// �������ٴ���һ��WM_SYSCOMMAND������WM_DESTROY��Ϣ���ٻ������
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
	// �õ��仯��Ŀ����
	short nWidth = LOWORD(lParam);
	short nHeight = HIWORD(lParam);

	char szText[256] = { 0 };
	sprintf_s(szText, "WM_SIZE: ��%d,�ߣ�%d\n", nWidth, nHeight);
	//AllocConsole();
	FILE* stream;
	freopen_s(&stream,"CONOUT$", "w", stdout);

	printf("i��ֵΪ%s\n", szText);
	//FreeConsole();
	//WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
	
}