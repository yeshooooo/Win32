#include <tchar.h>
#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	TCHAR sz[] = _T("���");
	MessageBox(NULL, TEXT("������ı�"), _T("���Ǳ���"), MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON3); // ���һ������ΪMessageBox�İ�ť
	return 0;
}