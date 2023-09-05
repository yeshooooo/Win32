// 宽窄字节字符串的使用
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>

// 窄字节转换成宽字节
wchar_t* cctry_a2w(const char* p_a)
{
	// 先调用一遍MultiByteToWideChar返回宽字节需要的
	int need_w_char = MultiByteToWideChar(CP_ACP, 0, p_a, -1, NULL, 0);
	if (need_w_char <= 0) return NULL;
	
	wchar_t* p_w = new wchar_t[need_w_char];
	// 清空内存
	wmemset(p_w, 0, need_w_char);

	// 再次调用MultiByteToWideChar就可以成功的转换了
	MultiByteToWideChar(CP_ACP, 0, p_a, -1, p_w, need_w_char);
	return p_w;

	
}

// 宽字节转换成窄字节
char* cctry_w2a(const wchar_t* p_w)
{
	int need_a_char = WideCharToMultiByte(CP_ACP, 0, p_w, -1, NULL, 0, NULL, NULL);
	if (need_a_char <= 0) return NULL;

	char* p_a = new char[need_a_char];
	WideCharToMultiByte(CP_ACP, 0, p_w, -1, p_a, need_a_char, NULL, NULL);
	return p_a;

}
int main1() {
	const char* p_a = "123456中国";
	int a = strlen(p_a);
	CA2W a2wObj(p_a);
	wchar_t* p_ww = (wchar_t*)a2wObj;

	wchar_t* p_www = cctry_a2w(p_a);
	char* p_aaa = cctry_w2a(p_www);
	CW2A w2aObj(p_www);
	char* p_aa = (char*)w2aObj;

	// 用完了之后要记得释放内存，因为在函数中都是new申请的
	delete[] p_www;
	delete[] p_aaa;
	const wchar_t* p_w = L"123";
	int w = wcslen(p_w);
	const TCHAR* p_t = TEXT("789");
	int t = _tcslen(p_t);
	TCHAR* pszBuf = new TCHAR[100];

	//// 字符串的拷贝
	//char sz_a[6] = { 0 };
	//strcpy_s(sz_a, "123456");
	return 0;
}
int main()
{
	CString str(TEXT("123456"));
	str += TEXT("abc ");
	str.Trim();
	str.MakeUpper();
	str.MakeLower();


	CString strRight = str.Right(2);

	CString str2(_T("cabbagc"));
	CString strSub =  str2.SpanIncluding(_T("abc"));
	CString strT;
	strT.Format(_T("hello %s%d"), _T("你好"), 123);
	strT.Replace('l', '#');
	strT.Replace(_T("##o"), _T("666"));

	// CString 默认支持从const char* 类型进行构造
	const char* ppp = "你好123";
	CString strPPP;
	strPPP = ppp; // 




	return 0;

}