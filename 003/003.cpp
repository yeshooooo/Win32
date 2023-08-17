#include <windows.h>
#include <stdio.h>
//#pragma comment(linker,"/subsystem:\"console\"/entry:\"WinMainCRTStartup\"")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 程序实例句柄
	printf("%p ---- %p\n", hInstance, hPrevInstance);
	// 当前应用程序实例句柄
	// HMODULE GetModuleHandle(PCSTRlpModuleName);传NULL时返回自身应用程序句柄。
	printf("当前应用程序实例句柄为: %p\n", GetModuleHandle(NULL));

	// 程序命令行字符串
	printf("%s\n", lpCmdLine);

	// 应用程序窗口的初始显示方式
	printf("%d\n", nShowCmd);

	MessageBox(NULL, L"Hello", L"提示", MB_OK);
	return 0;
}