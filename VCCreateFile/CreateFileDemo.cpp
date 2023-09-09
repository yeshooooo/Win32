#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// 使用CreateFile创建一个新的空白文件
	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("文件打开成功"), TEXT("Caption"), MB_OK);
		CloseHandle(hFile);// 防止内存泄露
	}
	else
	{
		MessageBox(NULL, TEXT("文件不存在"), TEXT("Caption"), MB_OK);
		//INVALID_HANDLE_VALUE 就不需要调用CloseHandle，因为他本身就是个无效的句柄
	}
	return 0;
}