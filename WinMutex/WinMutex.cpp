#include <Windows.h>
#include <stdio.h>

HANDLE g_hMutex = 0; // ���ջ�����
DWORD WINAPI TestProc(LPVOID pParam) {
	// ����ת��char* 
	char* pszText = (char*)pParam;
	while (1)
	{
		// �Ȼ���
		WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = 0; i < strlen(pszText); i++)
		{
			printf("%c", pszText[i]);
			
			Sleep(125);
		}
		printf("\n");
		ReleaseMutex(g_hMutex); // ��ʱ����û���߳�ӵ�����ˣ������������ź���

	}
}

DWORD WINAPI TestProc2(LPVOID pParam) {
	// ����ת��char* 
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

	// �������̲߳���ӵ�л��������Ȩ���������̺߳����߳��ų⣬����ͬʱ����, �����Բ�����
    // һ��ֻ�����߳��໥�ų�
	g_hMutex = CreateMutex(NULL, FALSE, NULL); // ����������ʱ�������κ��̶߳���ӵ�������������źŵ�

	DWORD nID = 0;
	char pszText[] = "********";
	HANDLE hThread = CreateThread(NULL, 0, TestProc, pszText, 0, &nID);

	// �ٿ����߳�
	// ���̴߳���ִ���źŵ�ʱ���߳̾��û���źţ����߳̽�����ʱ���߳̾����Ϊ���ź�


	char pszText2[] = "--------";
	HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, 0, &nID); // ����������ִ��

	// ���������̺߳����߳�Ҫ��������Ȼ��������ˣ����߳�Ҳû��
	getchar();


	CloseHandle(g_hMutex); // ����

}