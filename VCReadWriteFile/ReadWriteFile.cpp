#include <windows.h>

struct Student
{
	int age;
	char sex;
	char name[32];
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nShowCmd)
{
	// 打开
	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 
		//FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, // 第二个是立即刷新缓存到问价用
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
		return -1;
	}
	int num = 123;
	DWORD dwRealWrite = 0;
	BOOL bRet = WriteFile(hFile, &num, sizeof(num), &dwRealWrite, NULL);

	char ch = 'q';
	bRet = WriteFile(hFile, &ch, sizeof(ch), &dwRealWrite, NULL);

	char szText[32] = "我是VC驿站的粉丝！";
	bRet = WriteFile(hFile, szText, sizeof(szText), &dwRealWrite, NULL);
	Student stud;
	stud.age = 20;
	stud.sex = 'm';
	strcpy_s(stud.name, "zhangsan");
	bRet = WriteFile(hFile, &stud, sizeof(Student), &dwRealWrite, NULL);
	if (bRet)
	{
		MessageBox(NULL, TEXT("数据写入成功!"), TEXT("Tip"), MB_OK);

	}
	else
	{
		MessageBox(NULL, TEXT("数据写入失败!"), TEXT("Tip"), MB_OK);
	}

	// 注意上面的数据实际上是写入了缓存，并没有实际的写入文件中
	//FlushFileBuffers(hFile);
	
	// 先关闭文件，再重新打开，因为连续写入的时候，文件指针指到最后了
	CloseHandle(hFile);
	// 打开
	hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
		return -1;
	}
	// 从文件中读数据
// 	int ret_num = 0; // 接收读入的变量
 	DWORD dwRealRead = 0; // 实际读入的变量个数
// 	bRet = ReadFile(hFile, &ret_num, sizeof(ret_num), &dwRealRead, NULL);
// 
// 	// 读字符
// 	char cha = 0;
// 	bRet = ReadFile(hFile, &cha, sizeof(ret_num), &dwRealRead, NULL);
	// 从文件头开始设置
	//SetFilePointer(hFile, sizeof(int) + sizeof(char), NULL, FILE_BEGIN);
	//从文件尾开始设置, 大小计算的时候要写负数（这里有错误）
	SetFilePointer(hFile, (32 + sizeof(Student)), NULL, FILE_END);
	char szTextRet[32] = { 0 };
	bRet = ReadFile(hFile, szTextRet, sizeof(szTextRet), &dwRealRead, NULL);

	Student stud2;
	bRet = ReadFile(hFile, &stud2, sizeof(Student), &dwRealRead, NULL);

	if (bRet)
	{
		MessageBox(NULL, TEXT("数据读取成功!"), TEXT("Tip"), MB_OK);

	}
	else
	{
		MessageBox(NULL, TEXT("数据读取失败!"), TEXT("Tip"), MB_OK);
	}
	CloseHandle(hFile);
	return 0;

}