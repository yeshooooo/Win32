#include <stdio.h>
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(NULL, TEXT("11111"), TEXT("title"), MB_OK);

	// ɾ���ļ�
// 	BOOL bRet =  DeleteFile(TEXT("D:\\789.txt"));
// 	if (bRet)
// 	{
// 		MessageBox(NULL, TEXT("ɾ���ɹ�"), TEXT("Tip"), MB_OK);
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("ɾ��ʧ��"), TEXT("Tip"), MB_OK);
// 	}

// 	// �����ļ�
// 	BOOL bCopyRet = CopyFile(TEXT("D:\\456.png"), 
// 		//TEXT("D:\\abc.png"),
// 		TEXT("C:\\Users\\yeshooo\\Desktop\\abc.png"),
// 		FALSE // �ļ�����Ҳ�ܿ���
// 		);
// 	if (bCopyRet)
// 	{
// 		MessageBox(NULL, TEXT("�����ɹ�"), TEXT("tip"), MB_OK);
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("����ʧ��"), TEXT("tip"), MB_OK);
// 	}

// 	// �ƶ��ļ�
// 	BOOL bMoveRet = MoveFile(TEXT("D:\\456.png"), TEXT("D:\\789.png"));
// 	if (bMoveRet)
//  	{
//  		MessageBox(NULL, TEXT("�ƶ��ɹ�"), TEXT("tip"), MB_OK);
//  	}
//  	else
//  	{
//  		MessageBox(NULL, TEXT("�ƶ�ʧ��"), TEXT("tip"), MB_OK);
//  	}

	// ������
	// ��������7289Ϊֻ�����͵Ĺ�������ɾ����
	BOOL bDRet = DeleteFile(TEXT("D:\\7289.txt"));
	if (bDRet)
	{
		MessageBox(NULL, TEXT("ɾ���ɹ�"), TEXT("tip"), MB_OK);

	}
	else
	{
		DWORD dwError = GetLastError();
		if (dwError == 2)
		// if (dwError == ERROR_FILE_NOT_FOUND) // �����ں��е�ErrorFormat
		{
			MessageBox(NULL, TEXT("�ļ������ڣ���ȷ��Ҫɾ�����ļ�·���Ƿ���ȷ"), TEXT("tip"), MB_OK);
		}
		else if (dwError == 5)
		{
			MessageBox(NULL, TEXT("û��Ȩ��,ɾ��ʧ��"), TEXT("tip"), MB_OK);
		}
		else
		{
			MessageBox(NULL, TEXT("ɾ��ʧ��"), TEXT("tip"), MB_OK);
		}
		

	}
	return 0;
}