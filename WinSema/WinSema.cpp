#include <windows.h>
#include <stdio.h>
HANDLE g_hSema = 0;
// ֻŪһ�����̣߳�Ҳ����Э�����̺߳����̵߳Ĺ���

DWORD CALLBACK TestProc(LPVOID pParam) {
	while (1)
	{
		WaitForSingleObject(g_hSema, INFINITE);
		printf("***************\n");
	}

}

int main() {

	g_hSema = CreateSemaphore(NULL, 3, 10, NULL);
	DWORD nId = 0;
	HANDLE hThread = CreateThread(NULL, 0, TestProc, NULL, 0, &nId);
	getchar();
	ReleaseSemaphore(g_hSema, 5, NULL); // ���»س�������5���ź�������ֵ

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(g_hSema);
}