#include <tchar.h>
#include <windows.h>

#pragma comment(lib, "Urlmon.lib")

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	HRESULT hRet = URLDownloadToFile(NULL, TEXT("https://s.cctry.com/images/eagle2.png"),
		TEXT("D:\\cctry.png"), 0, NULL);
// 	if (hRet == S_OK)
// 	{
// 		TCHAR sz[] = _T("���");
// 		MessageBox(NULL, TEXT("���سɹ�"), _T("���Ǳ���"), MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON3); // ���һ������ΪMessageBox�İ�ť
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("����ʧ��"), TEXT("Tip"), MB_OK);
// 	}

	// 1. �򿪸ո����ص�ͼƬ
	SHELLEXECUTEINFO ShellInfo = { 0 };
	ShellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShellInfo.lpVerb = TEXT("open");
	ShellInfo.lpFile = TEXT("D:\\cctry.png");
	ShellInfo.nShow = SW_SHOW;

	BOOL bRet = ShellExecuteEx(&ShellInfo);

	// 2. �򿪼��±�
	//ShellInfo.lpFile = TEXT("C:\\Windows\\notepad.exe");
	// ��Ϊwindows��ϵͳ��������Կ��Բ���·��
	ShellInfo.lpFile = TEXT("notepad.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 3. �򿪼�����
	ShellInfo.lpFile = TEXT("calc.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 4. ��QQ
	ShellInfo.lpFile = TEXT("C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQScLauncher.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 5. ��Ĭ���������ָ����ַ
	ShellInfo.lpFile = TEXT("www.cctry.com");
	bRet = ShellExecuteEx(&ShellInfo);

	return 0;
}