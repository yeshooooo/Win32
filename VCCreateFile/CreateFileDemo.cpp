#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// ʹ��CreateFile����һ���µĿհ��ļ�
	HANDLE hFile = CreateFile(TEXT("D:\\VC��վok.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("�ļ��򿪳ɹ�"), TEXT("Caption"), MB_OK);
		CloseHandle(hFile);// ��ֹ�ڴ�й¶
	}
	else
	{
		MessageBox(NULL, TEXT("�ļ�������"), TEXT("Caption"), MB_OK);
		//INVALID_HANDLE_VALUE �Ͳ���Ҫ����CloseHandle����Ϊ��������Ǹ���Ч�ľ��
	}
	return 0;
}