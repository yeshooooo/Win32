#include <windows.h>
#include "resource1.h"
HANDLE g_hOutput = 0;

// ����WinMain�ĵ�һ������������λͼʹ��
HINSTANCE g_hInstance = 0;
// ����ĺ���
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

// ����
void DrawLine(HDC hdc)
{
	MoveToEx(hdc, 100, 100, NULL); 
	LineTo(hdc, 300, 300);
	LineTo(hdc, 0, 300);
}

// ������
void DrawRect(HDC hdc)
{
	Rectangle(hdc, 100, 100, 300, 300); // ���ϣ��ң���
}

// ��Բ��
void DrawEll(HDC hdc)
{
	Ellipse(hdc, 100, 100, 300, 300); //Բ�εĲ�������Ӿ���
}

// ����λͼ
void DrawBmp(HDC hdc)
{
	// 1. ���λͼ��Դ(��ͷ)

	// 2. ����λͼ(�������̵��ڴ���Դ��ȥ��
	// ����������ȫ�ֱ���
	HBITMAP hBmp =  LoadBitmap(g_hInstance, (LPCTSTR)IDB_BITMAP1);
	// 3. �����͵�ǰDCƥ��������ڴ�DC,���ҹ���һ���������򣬲����ڴ�DC�����������л�ͼ
	HDC hMemdc = CreateCompatibleDC(hdc);
	// 4. SelectObject ��λͼ�����͸��ڴ�DC
	// �ڴ�DC�����������н�λͼ���Ƴ���
	// ���ص����ϵ�λͼ�������Ǽٵģ���Ϊԭ��û�У�Ϊ�˺��潫��Ļ�����
	HGDIOBJ nOldBmp =  SelectObject(hMemdc, hBmp);

	// 5. ����
	// 1:1 ����
	BitBlt(
		hdc, // Ŀ��dc
		100, 100, // Ŀ�����Ͻ�
		48, 48, // Ŀ�Ĵ�С
		hMemdc, // ԴDC
		0, 0, // ��Դ��ʲôλ�ÿ�ʼ����
		SRCCOPY // ����ʽ��ԭ������
	);

	// ���ų���
	StretchBlt(
		hdc, // Ŀ��dc
		200, 200, // ���ڳ���λ��
		24, 24, // ���������С��Ҳ�ܷŴ���񣬸ı����Ŀ�������С����
		hMemdc, // Դdc
		0, 0, //��Դ��ʲôλ�ÿ�ʼ����
		48, 48, // �������ٳ���
		SRCCOPY // ����ʽ��ԭ������
	);


	// 6. ��λͼͼ��Ҫ����
	SelectObject(hMemdc, nOldBmp);
	// 7-8 �ͷ��ڴ�
	DeleteObject(hBmp);
	DeleteDC(hMemdc);

}
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 }; 
	// ps�ṹ����ĳ�Ա��ϵͳapi�ڲ�ʹ�õ�
	HDC hdc =  BeginPaint(hWnd, &ps);
	// ʹ��GDI���ʶ�����
	// PS_DASH ����
	// PS_SOLID ʵ�ߣ�ֻ��ʵ�߿��������������أ��������÷�1ֵ��ʧЧ�����ʵ��
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
	HGDIOBJ nOldPen = SelectObject(hdc, hPen);

	// ��ˢ
	//HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0)); // ʵ�߻�ˢ
	//HBRUSH hBrush = CreateHatchBrush(HS_CROSS, RGB(0, 255, 0)); // ��һ�������������ߵ���ʽ
		// ʹ�ò���ϵͳԤ��Ļ�ˢ
	HGDIOBJ hBrush = GetStockObject(NULL_BRUSH);
	HGDIOBJ nOldBrush = SelectObject(hdc, hBrush);




	//����
	//DrawPit(hdc);

	// ����
	//DrawLine(hdc);

	// ������
	//DrawRect(hdc);

	// ��Բ��
	//DrawEll(hdc);

	// ����λͼ
	DrawBmp(hdc);

	SelectObject(hdc, nOldPen); // �����ϻ��ʲ�������
	DeleteObject(hPen);
	SelectObject(hdc, nOldBrush); // �����ϻ�ˢ
	//DeleteObject(hBrush); // ϵͳ��ˢ�Ӳ�������



	EndPaint(hWnd, &ps);


}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
		//����ͼ��һ��Ҫ�ڻ�ͼ��Ϣ�����
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3); // �޸Ĵ��ڱ�����ɫ +1 Ϊ��ɫ�� +3 Ϊ��ɫ
	wc.hCursor = NULL;
	wc.hInstance = hInstance;
	wc.lpszClassName = "Main";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ���������и�ֵȫ��д�����ϵͳ
	RegisterClass(&wc);

	// ���ڴ��д�������
	HWND hWnd = CreateWindowEx(0, "Main", "windows", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// ��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}


}