#include <tchar.h>
#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	TCHAR sz[] = _T("你好");
	MessageBox(NULL, TEXT("程序框文本"), _T("我是标题"), MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON3); // 最后一个参数为MessageBox的按钮
	return 0;
}