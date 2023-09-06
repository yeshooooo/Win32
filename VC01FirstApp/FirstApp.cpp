#include <tchar.h>
#include <windows.h>

#pragma comment(lib, "Urlmon.lib")

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	HRESULT hRet = URLDownloadToFile(NULL, TEXT("https://s.cctry.com/images/eagle2.png"),
		TEXT("D:\\cctry.png"), 0, NULL);
// 	if (hRet == S_OK)
// 	{
// 		TCHAR sz[] = _T("你好");
// 		MessageBox(NULL, TEXT("下载成功"), _T("我是标题"), MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON3); // 最后一个参数为MessageBox的按钮
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("下载失败"), TEXT("Tip"), MB_OK);
// 	}

	// 1. 打开刚刚下载的图片
	SHELLEXECUTEINFO ShellInfo = { 0 };
	ShellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShellInfo.lpVerb = TEXT("open");
	ShellInfo.lpFile = TEXT("D:\\cctry.png");
	ShellInfo.nShow = SW_SHOW;

	BOOL bRet = ShellExecuteEx(&ShellInfo);

	// 2. 打开记事本
	//ShellInfo.lpFile = TEXT("C:\\Windows\\notepad.exe");
	// 因为windows在系统变量里，所以可以不加路径
	ShellInfo.lpFile = TEXT("notepad.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 3. 打开计算器
	ShellInfo.lpFile = TEXT("calc.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 4. 打开QQ
	ShellInfo.lpFile = TEXT("C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQScLauncher.exe");
	bRet = ShellExecuteEx(&ShellInfo);

	// 5. 用默认浏览器打开指定网址
	ShellInfo.lpFile = TEXT("www.cctry.com");
	bRet = ShellExecuteEx(&ShellInfo);

	return 0;
}