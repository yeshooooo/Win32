#include <windows.h>
#include <stdio.h>
//#pragma comment(linker,"/subsystem:\"console\"/entry:\"WinMainCRTStartup\"")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// ����ʵ�����
	printf("%p ---- %p\n", hInstance, hPrevInstance);
	// ��ǰӦ�ó���ʵ�����
	// HMODULE GetModuleHandle(PCSTRlpModuleName);��NULLʱ��������Ӧ�ó�������
	printf("��ǰӦ�ó���ʵ�����Ϊ: %p\n", GetModuleHandle(NULL));

	// �����������ַ���
	printf("%s\n", lpCmdLine);

	// Ӧ�ó��򴰿ڵĳ�ʼ��ʾ��ʽ
	printf("%d\n", nShowCmd);

	MessageBox(NULL, L"Hello", L"��ʾ", MB_OK);
	return 0;
}