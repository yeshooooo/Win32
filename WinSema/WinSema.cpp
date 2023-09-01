#include <windows.h>
#include <stdio.h>
HANDLE g_hSema = 0;
// 只弄一个子线程，也可以协调主线程和子线程的工作

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
	ReleaseSemaphore(g_hSema, 5, NULL); // 按下回车，增加5次信号量计数值

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(g_hSema);
}