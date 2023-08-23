#include <stdio.h>
#include <string.h>
#include <windows.h>
// 必须包含这个头文件，否则控制台不支持宽字节字符串
#include <locale.h>
int main() {
	const char* str1 = "A中";
	printf("%s %d\n", str1, strlen(str1));

	_wsetlocale(LC_ALL, L"chs"); // 设置输出得本地化为中文
	const wchar_t* str2 = L"A中"; // 必须加L表示字符串是unicode
	wprintf(L"%s %d\n", str2, wcslen(str2));

	//MessageBox();
}