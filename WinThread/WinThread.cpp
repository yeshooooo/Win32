#include <Windows.h>
#include <stdio.h>
DWORD WINAPI TestProc(LPVOID pParam) {
	// ����ת��char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		printf("%s\n", pszText);
		// ˯1��
		Sleep(1000);
	}
}

DWORD WINAPI TestProc2(LPVOID pParam) {
	// ����ת��char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		printf("%s\n", pszText);
		// ˯1��
		Sleep(1000);
	}
}

int main() {
	DWORD nID = 0;
	char pszText[] = "********";
	HANDLE hThread = CreateThread(NULL, 0, TestProc, pszText, 0, &nID);

	// �ٿ����߳�
	// ���̴߳���ִ���źŵ�ʱ���߳̾��û���źţ����߳̽�����ʱ���߳̾����Ϊ���ź�

	WaitForSingleObject(hThread, INFINITE);
	char pszText2[] = "------";
	// HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // ����������ִ��
	HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // �����󱻹���
	// ���������̺߳����߳�Ҫ��������Ȼ��������ˣ����߳�Ҳû��
	getchar();

	// �س����̶߳Ե�
	SuspendThread(hThread);
	ResumeThread(hThread2);
	getchar();


}