#include <stdio.h>
#include <string.h>
#include <windows.h>
// ����������ͷ�ļ����������̨��֧�ֿ��ֽ��ַ���
#include <locale.h>
int main() {
	const char* str1 = "A��";
	printf("%s %d\n", str1, strlen(str1));

	_wsetlocale(LC_ALL, L"chs"); // ��������ñ��ػ�Ϊ����
	const wchar_t* str2 = L"A��"; // �����L��ʾ�ַ�����unicode
	wprintf(L"%s %d\n", str2, wcslen(str2));

	//MessageBox();
}