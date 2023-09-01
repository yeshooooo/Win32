#include <Windows.h>
#include <stdio.h>

long g_value = 0;

DWORD CALLBACK TestProc1(LPVOID pParam) {
	for (int i = 0; i < 100000; i++)
	{
		//g_value++;
		// 使用原子锁进行++
		InterlockedIncrement(&g_value);
	}

	return 0;

}

DWORD CALLBACK TestProc2(LPVOID pParam) {
	for (int i = 0; i < 100000; i++)
	{
		//g_value++;
		InterlockedIncrement(&g_value);
	}

	return 0;

}

int main() {
	DWORD nID = 0;
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, TestProc1, NULL, 0, &nID);
	hThread[1] = CreateThread(NULL, 0, TestProc2, NULL, 0, &nID);

	// 阻塞，让主线程不结束
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	printf("等候结束\n");
	printf("最终结果为：%d\n", g_value);
}

