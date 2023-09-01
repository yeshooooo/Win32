#include <Windows.h>
#include <stdio.h>

HANDLE g_hMutex = 0; // 接收互斥句柄
DWORD WINAPI TestProc(LPVOID pParam) {
	// 类型转回char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		// 等互斥
		WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = 0; i < strlen(pszText); i++)
		{
			printf("%c", pszText[i]);
			
			Sleep(125);
		}
		printf("\n");
		ReleaseMutex(g_hMutex); // 这时候又没有线程拥有他了，互斥句柄又有信号了

	}
}

DWORD WINAPI TestProc2(LPVOID pParam) {
	// 类型转回char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = 0; i < strlen(pszText); i++)
		{
			printf("%c", pszText[i]);

			Sleep(125);
		}
		printf("\n");
		ReleaseMutex(g_hMutex);
	}
}

int main() {

	// 这里主线程不能拥有互斥的所有权，否则主线程和子线程排斥，不能同时工作, 很明显不可以
    // 一般只有子线程相互排斥
	g_hMutex = CreateMutex(NULL, FALSE, NULL); // 创建出来的时候，这里任何线程都不拥有他，他是有信号的

	DWORD nID = 0;
	char pszText[] = "********";
	HANDLE hThread = CreateThread(NULL, 0, TestProc, pszText, 0, &nID);

	// 再开个线程
	// 当线程处于执行信号的时候，线程句柄没有信号，当线程结束的时候，线程句柄变为有信号


	char pszText2[] = "--------";
	HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // 创建后立即执行

	// 创建完子线程后，主线程要阻塞，不然立马就完了，子线程也没了
	getchar();


	CloseHandle(g_hMutex); // 销毁

}