#include <Windows.h>
#include <stdio.h>
DWORD WINAPI TestProc(LPVOID pParam) {
	// 类型转回char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		printf("%s\n", pszText);
		// 睡1秒
		Sleep(1000);
	}
}

DWORD WINAPI TestProc2(LPVOID pParam) {
	// 类型转回char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		printf("%s\n", pszText);
		// 睡1秒
		Sleep(1000);
	}
}

int main() {
	DWORD nID = 0;
	char pszText[] = "********";
	HANDLE hThread = CreateThread(NULL, 0, TestProc, pszText, 0, &nID);

	// 再开个线程
	// 当线程处于执行信号的时候，线程句柄没有信号，当线程结束的时候，线程句柄变为有信号

	WaitForSingleObject(hThread, INFINITE);
	char pszText2[] = "------";
	// HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // 创建后立即执行
	HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // 创建后被挂起
	// 创建完子线程后，主线程要阻塞，不然立马就完了，子线程也没了
	getchar();

	// 回车后线程对调
	SuspendThread(hThread);
	ResumeThread(hThread2);
	getchar();


}