/*
* ʹ���¼�ʹ�����߳�Эͬ������
*/
#include <Windows.h>
#include <stdio.h>

HANDLE g_hEvent = 0; // �����¼����

DWORD CALLBACK PrintProc(LPVOID pParam) {
	while (1)
	{
		// �¼�������źŲ��ܴ�ӡ
		WaitForSingleObject(g_hEvent, INFINITE);
		printf("**********************\n");
		// ��ӡ���Ҫ����Ϊ���ź�״̬����Ȼֻ�ܿ���һ��,��print֮������Ҳ��һ���£������߼�����Ҫ���һ�£���ΪResetEvent�����ǲ�������
		ResetEvent(g_hEvent); // ��ȻҲ�����ڴ����¼���ʱ������Ϊ�����ֶ���λ����ʱ��Ͳ��ü���һ����
	}
	return 0;
}

DWORD CALLBACK CtrlProc(LPVOID pParam) {
	while (1)
	{
		// ��˯һ����ȥ����߳�1
		Sleep(1000);
		SetEvent(g_hEvent);


	}
	return 0;
}
int main() {
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD nId = 0;

	// ��һ���̸߳����ӡ���ڶ����̸߳�����ƴ�ӡ��һ���̴߳�ӡƵ�ʣ�����û�о�����ϵ
	HANDLE hThread[2] = { 0 };
	hThread[0] = CreateThread(NULL, 0, PrintProc, NULL, 0, &nId);
	hThread[1] = CreateThread(NULL, 0, CtrlProc, NULL, 0, &nId);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// 
}