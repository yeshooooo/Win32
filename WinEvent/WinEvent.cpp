/*
* 使用事件使两个线程协同处理工作
*/
#include <Windows.h>
#include <stdio.h>

HANDLE g_hEvent = 0; // 接受事件句柄

DWORD CALLBACK PrintProc(LPVOID pParam) {
	while (1)
	{
		// 事件句柄有信号才能打印
		WaitForSingleObject(g_hEvent, INFINITE);
		printf("**********************\n");
		// 打印完后要重置为无信号状态，不然只能控制一次,在print之上设置也是一回事，不过逻辑上需要理解一下，因为ResetEvent本身是不阻塞得
		ResetEvent(g_hEvent); // 当然也可以在创建事件得时候，设置为不能手动复位，这时候就不用加这一句了
	}
	return 0;
}

DWORD CALLBACK CtrlProc(LPVOID pParam) {
	while (1)
	{
		// 先睡一秒再去解救线程1
		Sleep(1000);
		SetEvent(g_hEvent);


	}
	return 0;
}
int main() {
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD nId = 0;

	// 第一个线程负责打印，第二个线程负责控制打印第一个线程打印频率，他俩没有竞争关系
	HANDLE hThread[2] = { 0 };
	hThread[0] = CreateThread(NULL, 0, PrintProc, NULL, 0, &nId);
	hThread[1] = CreateThread(NULL, 0, CtrlProc, NULL, 0, &nId);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// 
}