#include <windows.h>
#include <tchar.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
// 	HANDLE hFile =  CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if (hFile != INVALID_HANDLE_VALUE)
// 	{
// 		// 大于4G的需要接住第二位高位，也是一个DWORD类型接收高位
// 		DWORD dwFileSizeHigh = 0;
// 		DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
// 
// 		// 合成64位整型,跟INT64一个东西
// 		LONGLONG lFileSize = ((LONGLONG)dwFileSizeHigh << 32) | dwFileSize;
// 
// 		// 转换成字符串
// 		TCHAR szFileSize[128] = { 0 };
// 		swprintf_s(szFileSize, TEXT("文件的大小是: %lld"), lFileSize);
// 		MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
// 		CloseHandle(hFile);
// 
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("文件打开失败"), TEXT("Tip"), MB_OK);
// 	}
	HANDLE hFile = CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fiseSize = { 0 };
		BOOL bRet =  GetFileSizeEx(hFile, &fiseSize);

		if (bRet)
		{
			TCHAR szFileSize[128] = { 0 };
			swprintf_s(szFileSize, TEXT("文件的大小是: %lld"), fiseSize.QuadPart);
			MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
		}
		CloseHandle(hFile);

	}
	else
	{
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("Tip"), MB_OK);
	}

	
	return 0;
}