#include <stdio.h>
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(NULL, TEXT("11111"), TEXT("title"), MB_OK);

	// 删除文件
// 	BOOL bRet =  DeleteFile(TEXT("D:\\789.txt"));
// 	if (bRet)
// 	{
// 		MessageBox(NULL, TEXT("删除成功"), TEXT("Tip"), MB_OK);
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("删除失败"), TEXT("Tip"), MB_OK);
// 	}

// 	// 复制文件
// 	BOOL bCopyRet = CopyFile(TEXT("D:\\456.png"), 
// 		//TEXT("D:\\abc.png"),
// 		TEXT("C:\\Users\\yeshooo\\Desktop\\abc.png"),
// 		FALSE // 文件存在也能拷贝
// 		);
// 	if (bCopyRet)
// 	{
// 		MessageBox(NULL, TEXT("拷贝成功"), TEXT("tip"), MB_OK);
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("拷贝失败"), TEXT("tip"), MB_OK);
// 	}

// 	// 移动文件
// 	BOOL bMoveRet = MoveFile(TEXT("D:\\456.png"), TEXT("D:\\789.png"));
// 	if (bMoveRet)
//  	{
//  		MessageBox(NULL, TEXT("移动成功"), TEXT("tip"), MB_OK);
//  	}
//  	else
//  	{
//  		MessageBox(NULL, TEXT("移动失败"), TEXT("tip"), MB_OK);
//  	}

	// 错误码
	// 这里设置7289为只读类型的故意让他删不掉
	BOOL bDRet = DeleteFile(TEXT("D:\\7289.txt"));
	if (bDRet)
	{
		MessageBox(NULL, TEXT("删除成功"), TEXT("tip"), MB_OK);

	}
	else
	{
		DWORD dwError = GetLastError();
		if (dwError == 2)
		// if (dwError == ERROR_FILE_NOT_FOUND) // 或者内核中的ErrorFormat
		{
			MessageBox(NULL, TEXT("文件不存在，请确认要删除的文件路径是否正确"), TEXT("tip"), MB_OK);
		}
		else if (dwError == 5)
		{
			MessageBox(NULL, TEXT("没有权限,删除失败"), TEXT("tip"), MB_OK);
		}
		else
		{
			MessageBox(NULL, TEXT("删除失败"), TEXT("tip"), MB_OK);
		}
		

	}
	return 0;
}