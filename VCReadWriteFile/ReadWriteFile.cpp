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
	// ��
	HANDLE hFile = CreateFile(TEXT("D:\\VC��վok.txt"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 
		//FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, // �ڶ���������ˢ�»��浽�ʼ���
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("�ļ���ʧ��!"), TEXT("Tip"), MB_OK);
		return -1;
	}
	int num = 123;
	DWORD dwRealWrite = 0;
	BOOL bRet = WriteFile(hFile, &num, sizeof(num), &dwRealWrite, NULL);

	char ch = 'q';
	bRet = WriteFile(hFile, &ch, sizeof(ch), &dwRealWrite, NULL);

	char szText[32] = "����VC��վ�ķ�˿��";
	bRet = WriteFile(hFile, szText, sizeof(szText), &dwRealWrite, NULL);
	Student stud;
	stud.age = 20;
	stud.sex = 'm';
	strcpy_s(stud.name, "zhangsan");
	bRet = WriteFile(hFile, &stud, sizeof(Student), &dwRealWrite, NULL);
	if (bRet)
	{
		MessageBox(NULL, TEXT("����д��ɹ�!"), TEXT("Tip"), MB_OK);

	}
	else
	{
		MessageBox(NULL, TEXT("����д��ʧ��!"), TEXT("Tip"), MB_OK);
	}

	// ע�����������ʵ������д���˻��棬��û��ʵ�ʵ�д���ļ���
	//FlushFileBuffers(hFile);
	
	// �ȹر��ļ��������´򿪣���Ϊ����д���ʱ���ļ�ָ��ָ�������
	CloseHandle(hFile);
	// ��
	hFile = CreateFile(TEXT("D:\\VC��վok.txt"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("�ļ���ʧ��!"), TEXT("Tip"), MB_OK);
		return -1;
	}
	// ���ļ��ж�����
// 	int ret_num = 0; // ���ն���ı���
 	DWORD dwRealRead = 0; // ʵ�ʶ���ı�������
// 	bRet = ReadFile(hFile, &ret_num, sizeof(ret_num), &dwRealRead, NULL);
// 
// 	// ���ַ�
// 	char cha = 0;
// 	bRet = ReadFile(hFile, &cha, sizeof(ret_num), &dwRealRead, NULL);
	// ���ļ�ͷ��ʼ����
	//SetFilePointer(hFile, sizeof(int) + sizeof(char), NULL, FILE_BEGIN);
	//���ļ�β��ʼ����, ��С�����ʱ��Ҫд�����������д���
	SetFilePointer(hFile, (32 + sizeof(Student)), NULL, FILE_END);
	char szTextRet[32] = { 0 };
	bRet = ReadFile(hFile, szTextRet, sizeof(szTextRet), &dwRealRead, NULL);

	Student stud2;
	bRet = ReadFile(hFile, &stud2, sizeof(Student), &dwRealRead, NULL);

	if (bRet)
	{
		MessageBox(NULL, TEXT("���ݶ�ȡ�ɹ�!"), TEXT("Tip"), MB_OK);

	}
	else
	{
		MessageBox(NULL, TEXT("���ݶ�ȡʧ��!"), TEXT("Tip"), MB_OK);
	}
	CloseHandle(hFile);
	return 0;

}