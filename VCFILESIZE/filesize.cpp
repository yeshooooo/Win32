#include <windows.h>
#include <tchar.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
// 	HANDLE hFile =  CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if (hFile != INVALID_HANDLE_VALUE)
// 	{
// 		// ����4G����Ҫ��ס�ڶ�λ��λ��Ҳ��һ��DWORD���ͽ��ո�λ
// 		DWORD dwFileSizeHigh = 0;
// 		DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
// 
// 		// �ϳ�64λ����,��INT64һ������
// 		LONGLONG lFileSize = ((LONGLONG)dwFileSizeHigh << 32) | dwFileSize;
// 
// 		// ת�����ַ���
// 		TCHAR szFileSize[128] = { 0 };
// 		swprintf_s(szFileSize, TEXT("�ļ��Ĵ�С��: %lld"), lFileSize);
// 		MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
// 		CloseHandle(hFile);
// 
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("�ļ���ʧ��"), TEXT("Tip"), MB_OK);
// 	}
	HANDLE hFile = CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fiseSize = { 0 };
		BOOL bRet =  GetFileSizeEx(hFile, &fiseSize);

		if (bRet)
		{
			TCHAR szFileSize[128] = { 0 };
			swprintf_s(szFileSize, TEXT("�ļ��Ĵ�С��: %lld"), fiseSize.QuadPart);
			MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
		}
		CloseHandle(hFile);

	}
	else
	{
		MessageBox(NULL, TEXT("�ļ���ʧ��"), TEXT("Tip"), MB_OK);
	}

	
	return 0;
}