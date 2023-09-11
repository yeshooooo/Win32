#include <windows.h>
HANDLE g_hOutput = 0;
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);

	//������ɫ�ͱ���
	SetTextColor(hdc, RGB(255, 0, 255));
	SetBkColor(hdc, RGB(0, 255, 0));
	// ͨ�����ñ���ģʽ���ñ���ɫ͸���ģ�SetBkColor��TRANSPARENTģʽ��ʧЧ��ֻ������OPAQUE��͸��ģʽ
	SetBkMode(hdc, TRANSPARENT); //��ɫ������һ��

	// ��������
	HFONT hFont = CreateFont(
		30, //�߶ȸ�30
		0, // ��ȸ�0���ᰴ�߶�ƥ����
		45, // ��б�Ƕ�
		0, // ѡ��Ƕȣ���άû��
		900, // �����ϸ
		1, // �ǲ���б��
		1, //Ҫ��Ҫ�»���
		1, // Ҫ��Ҫɾ����
		GB2312_CHARSET, // �ַ���
		0, 0, 0, 0, //��������
		"����" //�������֣�˫�������ļ���һ�п�

	);
	HGDIOBJ nOldFont =  SelectObject(hdc, hFont); //�͸����ң�������ԭ��������

	char szText[] = "hello Text";
	TextOut(hdc, 100, 100, szText, strlen(szText));
	RECT rc;
	rc.left = 100;
	rc.top = 150;
	rc.right = 200;
	rc.bottom = 200;
	Rectangle(hdc, 100, 150, 200, 200); //����rcȷ���ľ��η�Χ
	// DT_VCENTER��DT_BOTTOMֻ������DT_SINGLELINE��������DT_WORDBREAK��ͻ
	DrawText(hdc, szText, strlen(szText), &rc, 
		//DT_LEFT | DT_TOP
	DT_CENTER | DT_VCENTER // ˮƽ�ʹ�ֱ�����У���ֱ����ֻ�ʺϵ��У�����DT_WORDBREAK��ͻ
	| DT_WORDBREAK // ���л���
	| DT_NOCLIP // �Ų�����ʱ�򲻲ü����������Ҳ��
	);

	// �������Լ�Ҫ����
	SelectObject(hdc, nOldFont);
	DeleteObject(hFont); // ���������ڴ�
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