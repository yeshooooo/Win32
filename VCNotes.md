[VC驿站Windows程序设计win32部分](https://www.bilibili.com/video/BV1Bb411e7av/?p=1&vd_source=c6ca89f75d00cd4da634736edfcca1ae)

API的msdn文档按F1就能跳转到对应的函数文档上

# 1. 不同的程序入口点

[WinMain 应用程序入口点 - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/learnwin32/winmain--the-application-entry-point)

WinMain命令行参数传递作为ANSI字符串，wWinMain命令行参数为Unicode

有些旧版教程中使用_tWinMain，此类定义均位于 tchar.h，从源码来看, _tWinMain就是wWinMain

```cpp
//tchar.h

/* Program */

#define _tmain      wmain
#define _tWinMain   wWinMain
#define _tenviron   _wenviron
#define __targv     __wargv
```

==所以当使用wMain的时候，要包含tchar.h头文件==

```cpp
#include <tchar.h>
#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	MessageBox(NULL, TEXT("程序框文本"), _T("我是标题"), MB_OK); // 最后一个参数为MessageBox的按钮
	return 0;
}
```



[vc驿站MessageBox](https://www.cctry.com/forum.php?mod=viewthread&tid=298028&highlight=MessageBox)

[调用约定](https://www.cctry.com/forum.php?mod=viewthread&tid=288828&highlight=%B5%F7%D3%C3%D4%BC%B6%A8)

# 2. 宽窄字符串

![image-20230904172136171](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309041721212.png)

**vs设置中多字节对应的是窄字节**==多字节的意思是有的地方是一个字节，有的地方是多个字节，数字，字母这些用一个字节表示==

**UNICODE对应的是宽字节**

[实用VC编程之玩转字符串](https://www.cctry.com/forum-153-1.html)

### 2.1 宽窄字符串的区别

**宽字节的功能，即：Unicode，Unicode中规定任意一个字符都占用两个字节的存储空间，即2个char，不管是数字或者字母，还是一个汉字 都占用2个字节。用两个char难免不方面，所以微软直接使用一个新的类型：wchar_t，大家看起来比较陌生，不过他的原型实际上就是 unsigned short，这个大家比较熟悉吧，占用2个字节的存储空间。**

其实 MessageBox 他只是一个宏，他对应的两个版本的函数分别为：MessageBoxA 和 MessageBoxW，你在VS编译器中使用的时候系统会根据是否定义了_UNICODE 宏来进行判断当前工程使用的是宽字节的Unicode编码还是窄字节编码，根据这个来决定该使用哪个版本的函数！如果你的工程没有定义 _UNICODE 宏，也就是非Unicode编码，那么就使用窄字节的 MessageBoxA，如果定义了，那么就使用宽字节的 MessageBoxW

==查看DLL中的导出函数可以使用 depends 这个工具来查看！==[Windows 查看 exe 依赖的 dll 的方法 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/395557318)

[lucasg/Dependencies: A rewrite of the old legacy software "depends.exe" in C# for Windows devs to troubleshoot dll load dependencies issues. (github.com)](https://github.com/lucasg/Dependencies)

MessageBox位于C:\Windows\System32\user32.dll



备注：Windows 2000 及其以后的 Xp、2003、Vista、Win7、Win8、Win10 等系统都是使用Unicode从头进行开发的，如果调用任何一个Windows API 函数并给它传递一个 ANSI 字符串，那么系统首先要将字符串转换成Unicode，然后将Unicode字符串传递给操作系统。如果希望函数返回ANSI字符串，系统就会先将Unicode字符串转换成ANSI字符串，然后将结果返回给你的应用程序。进行这些字符串的转换需要占用系统的时间和内存。通过从头开始用Unicode来开发应用程序，就能够使你的应用程序更加高效的运行

### 2.2 优缺点

一般来说只是涉及到界面，或者是跟字符串操作相关的建议大家使用宽字节，其他地方还是可以用窄字节。因为宽字节的占用空间比窄字节多了一倍，所以如果是单纯在本机的话还好，如果是进行字符串的网络传输，那么传输量就会是窄字节的二倍，所以这块也不是说什么时候都一味的用Unicode宽字节。

### 2.3 复杂的宽窄字节数据类型：

● 窄字节：
char、char * 、const char *
CHAR、(PCHAR、PSTR、LPSTR)、LPCSTR

上下三组相互对应，如PCHAR、PSTR、LPSTR都是char*

● Unicode 宽字节：
wchar_t、wchar_t * 、const wchar_t *
WCHAR、(PWCHAR、PWSTR、LPWSTR)、LPCWSTR

● T 通用类型：
TCHAR、(TCHAR * 、PTCHAR、PTSTR、LPTSTR)、LPCTSTR

其中：P代表指针的意思，STR代表字符串的意思，==L是长指针的意思，在以前16位操作系统上有区别==在WIN32平台下可以忽略，C代表const常量的意思，W代表wide宽字节的意思，T大家可以理解为通用类型的意思，
就是可以根据工程中是否定义_UNICODE 宏，来判断当前工程的编码类型是宽字节还是窄字节，之后分别定义成不同的类型，比如：TCHAR 类型，如果工程中定义了_UNICODE 宏，那么就表明工程是宽字节编码，他最终就被定义成 wchar_t 类型，如果工程中没有定义_UNICODE 宏，就表明工程当前是窄字节编码，那么 TCHAR 被最终定义成 char 类型。

〓※※※〓 其方便性就是修改了工程的编码格式之后不用修改代码，所以还是建议大家在编写程序的时候使用通用类型！



# 3. 宽窄字符串的使用

### **3.1、宽窄字符串类型指针的定义：**

字符串字面量是ANSI类型的，c++中需加const

● 窄字节：const char *p_str = "hello";
● Unicode宽字节：const wchar_t *p_wstr = L"hello";
● 通用类型：const TCHAR *p_tstr = _T("hello"); 或者 TCHAR *p_tstr= _TEXT("hello");
● 动态申请内存：TCHAR *pszBuf = new TCHAR[100];

其中，_TEXT 和 _T 是一样的，定义如下：
\#define _T(x)    __T(x)
\#define _TEXT(x)  __T(x)

来看看 __T 的最终定义：
\#ifdef _UNICODE
\#define __T(x)    L##x
\#else
\#define __T(x)    x
\#endif

其中，##为连接的意思。

### **3.2、常用的宽窄字节字符串处理函数：**

字符串长度：
● Ansi：strlen(const char *str);
● Unicode：wcslen(const wchar_t *str);
● 通用函数：_tcslen(TCHAR *str);

● Ansi：int atoi(const char *str);
● Unicode：int _wtoi(const wchar_t *str);
● 通用函数：_tstoi(const TCHAR *str);

字符串拷贝：
● Ansi：strcpy(char *strDestination, const char *strSource);
● Unicode：wcscpy(wchar_t *strDestination, const wchar_t *strSource);
● 通用函数：_tcscpy(TCHAR *strDestination, const TCHAR *strSource);

以上函数不安全，在vs2003等以上版本的编译器中会有warnning警告提示，以下为安全函数（VC++6.0不支持）：
● Ansi：strcpy_s(char *strDestination, size_t numberOfElements, const char *strSource);
● Unicode：wcscpy_s(wchar_t *strDestination, size_t numberOfElements, const wchar_t *strSource);
● 通用函数：_tcscpy_s(TCHAR *strDestination, size_t numberOfElements, const TCHAR *strSource);

numberOfElements
Size of the destination string buffer. **目的缓冲区的大小，以字节为单位，不是字符！**

size_t unsigned integer，在MSDN中的解释：Result of sizeof operator，也就是说 size_t 是 unsigned integer 即无符号整数。那为什么会有size_t这个类型呢？
因为不同平台的操作系统(32/64)中 int/long 等类型所占的字节并不一样，而 size_t 在不同的平台下有不同的定义。有点类似于TCHAR类型：

```cpp
#ifndef  _SIZE_T_DEFINED
 #ifdef   _WIN64
 typedef  unsigned  __int64      size_t;  //8个字节，64位
 #else
 typedef  _W64  unsigned  int    size_t;  //4个字节，32位
 #endif
 #define  _SIZE_T_DEFINED
#endif
```



### **3.3、sizeof 求宽窄字节字符串的注意事项：**

const char* p_str = "hello";
我想求这个字符串所占用的字节数：sizeof(p_str) 肯定是错误的！
strlen(p_str) + 1，实际上就是 p_str 指针指向的字符串所占用的空间对吧？因为本身1个char占用1个字节的存储空间，所以默认字符串的长度加上字符串的结束符 \0 就是字符串所占用的字节数。

那么宽字节呢？const wchar_t* p_wstr = L"hello";
他占用的字节数应该如何来求？
wcslen(p_str) + 1 这个对吗？当然也是不对的，正确的是：(wcslen(p_str) + 1) * sizeof(wchar_t)

通用类型：const TCHAR* p_tstr = __T("hello");
(_tcslen(p_tstr) + 1) * sizeof(TCHAR)

应用在哪里呢？比如：
TCHAR *pszBuf = new TCHAR[100];
定义完了之后，我要给这个字符串数组清空，于是我应该这么做：
memset(pszBuf, 0, 100 * sizeof(TCHAR)) 这样做是最安全的，不管当前的 TCHAR 是 char 也好，是 wchar_t 也好，都可以满足。

# 4. 宽窄字节字符串的转换

### 4.0 ANSI,UTF-8,UNICODE

[ Windows 记事本的 ANSI、Unicode、UTF-8 这三种编码模式有什么区别](https://www.zhihu.com/question/20650946)

[1] Windows 里说的「ANSI」其实是 Windows code pages，这个模式根据当前 locale 选定具体的编码，比如简中 locale 下是 GBK。把自己这些 code page 称作「ANSI」是 Windows 的臭毛病。在 ASCII 范围内它们应该是和 ASCII 一致的。

[2] 把带有 BOM 的小端序 UTF-16 称作「Unicode」也是 Windows 的臭毛病。Windows 从 Windows 2000 开始就已经支持 [surrogate pair](https://www.zhihu.com/search?q=surrogate pair&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A15745831}) 了，所以已经是 UTF-16 了，「[UCS-2](https://www.zhihu.com/search?q=UCS-2&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A15745831})」这个说法已经不合适了。UCS-2 只能编码 BMP 范围内的字符，从 1996 年起就在 Unicode/ISO 标准中被 UTF-16 取代了（UTF-16 通过蛋疼的 surrogate pair 来编码超出 BMP 的字符）。都十多年了，求求大家别再误称了……

[3] 把带 BOM 的 UTF-8 称作「UTF-8」又是 Windows 的臭毛病。如果忽略 BOM，那么在 ASCII 范围内与 ASCII 一致。另请参见：「带 BOM 的 UTF-8」和「无 BOM 的 UTF-8」有什么区别？

### **4.1 为什么要进行转换？**

既然宽窄字节字符串各自的基本使用都给大家讲解完了，那么本节课开始呢，就给大家讲解下如何进行宽窄字节字符串的转换。
之前跟大家说过，并不是建议程序中所有的字符串都用宽字节的Unicode来表示，比如**网络发送的字符串就可以用窄字节**的，当对方收到之后默认收到的是窄字节的，因为对方的程序可能用宽字节Unicode来写的界面，所以要显示的时候就要转换成Unicode宽字节的字符串。这样就涉及到宽窄字节的转换，类似的情况很常见，所以本节课重点讲解下！

### **4.2 使用微软提供的API函数来实现宽窄字节的转换：**

WideCharToMultiByte 实现宽字节转换到窄字节
MultiByteToWideChar 实现窄字节转换到宽字节

WideCharToMultiByte 的代码页用来标记与新转换的字符串相关的代码页，==窄字节的代码页是可变的，宽字节的代码页是不可变的==；

返回值是字符数，不是字节

![image-20230905112000558](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051120649.png)

MultiByteToWideChar 的代码页用来标记与一个多字节字符串相关的代码页，

[1]、常用的代码页有 CP_ACP 和 CP_UTF8 两个：
使用 CP_ACP 代码页就实现了 ANSI 与 Unicode 之间的转换；--- 我们所用的！
使用 CP_UTF8 代码页就实现了 UTF-8 与 Unicode 之间的转换，在网页中经常会用到UTF8编码的格式。

[2]、dwFlags 参数允许我们进行额外的控制，但是，一般情况下都不使用这个标志，直接传递 0 就行了。

[3]、lpDefaultChar和pfUsedDefaultChar：只有当WideCharToMultiByte函数遇到一个宽字节字符，而该字符在uCodePage参数标识的代码页中并没有它的表示法时，WideCharToMultiByte函数才使用这两个参数。如果宽字节字符不能被转换，该函数便使用lpDefaultChar参数指向的字符。如果该参数是NULL（这是大多数情况下的参数值），那么该函数使用系统的默认字符。该默认字符通常是个问号。这对于文件名来说是危险的，因为问号是个通配符。pfUsedDefaultChar参数指向一个布尔变量，如果Unicode字符串中至少有一个字符不能转换成等价多字节字符，那么函数就将该变量置为TRUE。如果所有字符均被成功地转换，那么该函数就将该变量置为FALSE。当函数返回以便检查宽字节字符串是否被成功地转换后，可以测试该变量。

● 两个转换函数的使用举例：

![image-20230905105007923](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051050055.png)

![image-20230905105023853](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051050982.png)

```cpp
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
```



注意：使用过后千万别忘记释放返回的指针空间。
具体代码的讲解详见视频教程！

### **4.3 使用ATL提供的 CA2W、CW2A、CT2W、CT2A 来更方便的转换：**

之前在《VC++基础班》中曾给大家讲解过 A2W、W2A、T2A、T2W 之类的宏也可以方便的转换，但是这几个宏出现的时间比较早，如果字符串很长的话就会有问题，所以这里面不再建议大家使用了。介绍给大家更为方便和安全的 **CA2W、CW2A、CT2W、CT2A 封装类来给大家使用**。当然设置里要使用MFC

相关头文件#include <atlbase.h> 

```cpp
	CA2W a2wObj(p_a);
	wchar_t* p_ww = (wchar_t*)a2wObj;

```



# 5. **CString的方便之处及优缺点**

**1、CString简介：** 提起CString，相信接触过MFC编程开发的小伙伴都不陌生，相反很熟悉。MFC中对于CString字符串的使用可以说是很频繁的，CString类也可以说是MFC中针对字符串的一个封装类。其实CString归属于ATL，不过在MFC中一样使用，而且被重点使用。

 **2、CString类的方便之处：** 

在 C语言中，对于字符串的操作有字符串数组，字符串指针之类的，比如： 

```cpp
const char* p_str = "hello"; 
char szbuf[100] = {'h', 'e', 'l', 'l', 'o'};
```



 再对字符串进行拷贝、连接、比较 的时候也要借助 strcpy、strcat、strcmp 之类的函数，而且还要考虑目标空间是否够用之列的，很是麻烦，一旦操作不好就会导致数组越界了，造成缓冲区溢出。

有的网友会说了，C语言的字符串处理确实比较麻烦，但是我可以使用C++ STL 中的 string 啊，这样要比C语言的方便很多吧，比如：

```cpp
string str = "hello";
str += " world.";
bool is_equal = (str == "123");
```

这样就不用担心字符串空间是否够用，连接、比较什么的也十分方便，直接 += 或者 == 就可以判断出来了。其实 C++ STL 中的 string 还提供了很多其他的算法，例如：
http://www.cplusplus.com/reference/string/string/

虽然 C++ STL 中的 string 照比C语言中的字符串处理方便很多，但是这里我要说，跟成熟的字符串处理还是差很多，起码跟 CString来说就差了不少。比如：
trim 操作：去除掉首尾的不可见字符，比如回车，制表符，空格之类的；
reverse 操作：进行字符串的首尾颠倒反转；
upper操作：将字符串中的英文全变成大写字母；
lower操作：将字符串中的英文全变成小写字母；
right操作：直接返回字符串中结尾的指定字符；
span_including操作：返回包含指定字符串中任意一个字符的子串
span_excluding操作：返回不包含指定字符串中任意一个字符的子串
format：格式化字符串
replace：替换字符串中的指定字符
stricmp：不区分大小写进行字符串比较

等等吧，这些都没有现成的函数，不可能类似这样简单的操作：
string str = "hello";
str.upper();
直接将字符串变成大写字母的。实际是做不到的，即使能做到也需要好几行代码。但是以上这些操作在 CString 中就相对简单了。可以很轻松的做到，CString早就封装好了这些常用的操作。
例如：
CString str;


**3、CString 对于 TCHAR 的封装：**

针对当前编码，决定当前内部的指针是 char* 还是 wchar_t*，实际上CString 本身是个模板类，实际上有三种：

```cpp
typedef ATL::CStringT< wchar_t, StrTraitMFC< wchar_t > > CStringW;
typedef ATL::CStringT< char, StrTraitMFC< char > > CStringA;
typedef ATL::CStringT< TCHAR, StrTraitMFC< TCHAR > > CString;
```


CStringA 内部使用的是 char*，CStringW 内部使用的是 wchar_t*，CString内部使用的是 TCHAR*，所以 CString 本身会根据当前工程的编码决定使用 char* 还是 wchar_t*，这一点也比较方便。
我们可以先简单使用下 CString，之后改变下工程的编码，看看编译结果就知道了。

**4、CString 类对于宽窄字节的转换：**
当前工程编码是宽字节Unicode的前提下，CString支持从char* 类型的窄字节字符串来构造和赋值。所以，是可以方便的从窄字节转换到宽字节。但是反之则没有办法实现。

**5、使用 CString 类的优缺点：**
先来说说优点：
①、使用方便，包含了很多实现好的操作，包括：trim、reverse、format、replace 等等；
②、无缝兼容MFC；
③、自适应当前工程编码，匹配宽窄字节；
④、==只能能实现从窄字节到宽字节的自动转换工作==

再来说说缺点：
①、在非MFC的工程中无法使用，例如：控制台类型的工程，或者Win32类型的API项目；
②、无法完全实现宽窄字节的任意转换；
③、非C++标准，无法跨平台，在除了VC以外的编译器无法使用，更无法在Linux等其他平台上使用。

# 6. std::string 扩展

略

# 7. Windows编程中常用的数据类型

![image-20230905165410645](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051654572.png)

![image-20230905165716375](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051657193.png)

![image-20230905170658673](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051706716.png)

==32位和64位系统上不区分长短指针了，加不加L已经无所谓了==

![image-20230905170825450](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051708562.png)

![image-20230905171059254](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051711594.png)

![image-20230905171323230](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051713206.png)

![image-20230905171545325](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051715298.png)

![image-20230905171647177](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051716064.png)

![image-20230905171747213](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309051717589.png)

# 8.  URLDownloadFile和 ShellExecuteEx

### 8.1 自动下载文件URLDownloadFile

URLDownloadToFile 函数说明 :

简介：
指从指定URL地址读取内容并将读取到的内容保存到特定的文件里。

```cpp
HRESULT URLDownloadToFile( 
    LPUNKNOWN pCaller,
    LPCTSTR szURL,
    LPCTSTR szFileName,
    DWORD dwReserved,
    LPBINDSTATUSCALLBACK lpfnCB
);
```



**参数：**
pCaller：Pointer to the controlling IUnknown interface of the calling Microsoft ActiveX component (if the caller is an ActiveX component).
//该参数为 ActiveX 控件的接口指针，如果当前程序不是 ActiveX 控件则写 NULL 就可以了。

szURL
Pointer to a string value containing the URL to be downloaded. Cannot be set to NULL.
//该参数为要下载的 url 地址，不能为空。

szFileName
Pointer to a string value containing the name of the file to create for bits that come from the download.
//下载文件后，保存的文件名字，包含文件的具体路径信息。

dwReserved
Reserved. Must be set to 0.
//保留字段，必需为0。

lpfnCB
Pointer to the caller's IBindStatusCallback interface. URLDownloadToFile calls this interface's IBindStatusCallback::OnProgress method on a connection activity, including the arrival of data. IBindStatusCallback::OnDataAvailable is never called.
//下载进度状态回调接口的指针。如果要实时监视下载文件的状态那么就要用到这个参数了。

**返回值：**
S_OK : 成功下载；
E_OUTOFMEMORY：缓冲区长度不够，或者没有足够的内存来完成下载的操作；
INET_E_DOWNLOAD_FAILURE：指定的资源或者回调接口有问题。

**实例**

![image-20230906094740317](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309060947372.png)

#pragma comment(lib, "Urlmon.lib")

```cpp
HRESULT hRet = URLDownloadToFile(NULL, TEXT("https://s.cctry.com/images/eagle2.png"), TEXT("D:\\cctry.png"), 0, NULL);
```

### 8.2 自动执行文件 ShellExecuteEx

==准确的说他是一个Shell API，不是Windows系统的标准API, 是经过微软封装过的一层==

**简介：** 可以使用 ShellExecuteEx 打开文件或执行程序

**语法格式：**

```cpp
BOOL ShellExecuteEx(
  _Inout_ SHELLEXECUTEINFO *pExecInfo
 );
```

**参数：**
输入输出参数都是 SHELLEXECUTEINFO 结构体，其结构定义如下：

```cpp
typedef struct _SHELLEXECUTEINFO
{
  DWORD    cbSize;//结构大小,一般直接写sizeof(SHELLEXECUTEINFO)，这个参数是为以后可能的扩展考虑的 ---重要
  ULONG     fMask;//指定结构成员的有效性，指定哪个有效，比如是想打开程序，网址等等
  HWND      hwnd;//父窗口句柄或出错时显示错误父窗口的句柄，可以为 NULL
  LPCTSTR   lpVerb;//指定该函数的执行动作            ---重要
  LPCTSTR   lpFile;//操作对象路径
  LPCTSTR   lpParameters;//执行参数，可以为 ULL
  LPCTSTR   lpDirectory;//工作目录，可以为 NULL
  int             nShow;//显示方式
  HINSTANCE hInstApp;//如果设置了 SEE_MASK_NOCLOSEPROCESS ,并且调用成功则该值大于32，调用失败者被设置错误值
  LPVOID     lpIDList;//ITEMIDLIST结构的地址，存储成员的特别标识符，当fMask不包括SEE_MASK_IDLIST或SEE_MASK_INVOKEIDLIST时该项被忽略
  LPCTSTR   lpClass;//指明文件类别的名字或GUID，当fMask不包括SEE_MASK_CLASSNAME时该项被忽略
  HKEY        hkeyClass;//获得已在系统注册的文件类型的Handle，当fMask不包括SEE_MASK_HOTKEY时该项被忽略
  DWORD   dwHotKey;//程序的热键关联，低位存储虚拟关键码（Key Code），高位存储修改标志位(HOTKEYF_)，当fmask不包括SEE_MASK_HOTKEY时该项被忽略

  union
  {
    HANDLE hIcon;//取得对应文件类型的图标的Handle，当fMask不包括SEE_MASK_ICON时该项被忽略
    HANDLE hMonitor;//将文档显示在显示器上的Handle，当fMask不包括SEE_MASK_HMONITOR时该项被忽略
  } DUMMYUNIONNAME;

  HANDLE    hProcess;//指向新启动的程序的句柄。若fMask不设为SEE_MASK_NOCLOSEPROCESS则该项值为NULL。
                     //但若程序没有启动，即使fMask设为SEE_MASK_NOCLOSEPROCESS，该值也仍为NULL。
                     //如果没有新创建进程，也会为空

} SHELLEXECUTEINFO, *LPSHELLEXECUTEINFO;
```

**fMask** 用于指定结构成员的内容和有效性，可为下列值的组合：

● SEE_MASK_DEFAULT (0)默认
● SEE_MASK_CLASSNAME 使用 lpClass 参数，如果 SEE_MASK_CLASSKEY 也有效，则用后者
● SEE_MASK_CLASSKEY 使用 hkeyClass 参数
● SEE_MASK_IDLIST 使用 lpIDList 参数
● SEE_MASK_INVOKEIDLIST 使用选定项目的快捷菜单 IContextMenu 接口处理程序
● SEE_MASK_ICON 使用 hIcon 给出的菜单，不能与 SEE_MASK_HMONITOR 共用，Vista之后
● SEE_MASK_HOTKEY 使用 dwHotKey 参数
● SEE_MASK_NOCLOSEPROCESS 如果执行之后需要返回进程句柄，或者等待执行完毕的话，则需要指定该参数，从结构参数意义可以看到 hProcess 和 hInstApp 都依赖该选项
● SEE_MASK_CONNECTNETDRV 验证共享并连接到驱动器号
● SEE_MASK_NOASYNC 不等待操作完成，直接返回，会创建一个后台线程运行。
● SEE_MASK_FLAG_DDEWAIT 弃用，使用 SEE_MASK_NOASYNC
● SEE_MASK_DOENVSUBST 环境变量会被展开
● SEE_MASK_FLAG_NO_UI 出现错误，不显示错误消息框，比如不会弹出找不到文件之类的窗口，直接返回失败
● SEE_MASK_UNICODE UNICODE 程序
● SEE_MASK_NO_CONSOLE 继承父进程的控制台，而不是创建新的控制台，与 CREATE_NEW_CONSOLE 相反
● SEE_MASK_ASYNCOK 执行在后台线程，调用立即返回
● SEE_MASK_NOQUERYCLASSSTORE 弃用
● SEE_MASK_HMONITOR 使用 hmonitor，不能与 SEE_MASK_ICON 共存
● SEE_MASK_NOZONECHECKS 不执行区域检查
● SEE_MASK_WAITFORINPUTIDLE 创建新进程后，等待进程变为空闲状态再返回，超时时间为1分钟
● SEE_MASK_FLAG_LOG_USAGE 跟踪应用程序启动次数
● SEE_MASK_FLAG_HINST_IS_SITE

**lpVerb** 参数与 ShellExecute 的 lpOperation 参数一致：
edit 用编辑器打开 lpFile 指定的文档，如果 lpFile 不是文档，则会失败
explore 浏览 lpFile 指定的文件夹
find 搜索 lpDirectory 指定的目录
open 打开 lpFile 文件，lpFile 可以是文件或文件夹
print 打印 lpFile，如果 lpFile 不是文档，则函数失败
properties 显示属性
runas 请求以管理员权限运行，比如以管理员权限运行某个exe
NULL ==执行默认”open”动作==

**nShow** 与 ShellExecute 的该参数一致：
● SW_HIDE 隐藏窗口，活动状态给令一个窗口
● SW_MINIMIZE 最小化窗口，活动状态给令一个窗口
● SW_RESTORE 用原来的大小和位置显示一个窗口，同时令其进入活动状态
● SW_SHOW 用当前的大小和位置显示一个窗口，同时令其进入活动状态
● SW_SHOWMAXIMIZED 最大化窗口，并将其激活
● SW_SHOWMINIMIZED 最小化窗口，并将其激活
● SW_SHOWMINNOACTIVE 最小化一个窗口，同时不改变活动窗口
● SW_SHOWNA 用当前的大小和位置显示一个窗口，不改变活动窗口
● SW_SHOWNOACTIVATE 用最近的大小和位置显示一个窗口，同时不改变活动窗口
● SW_SHOWNORMAL 与SW_RESTORE相同

如果设置了 SEE_MASK_NOCLOSEPROCESS ，调用成功则 hInstApp 返回大于32的值，调用失败会返回：
● SE_ERR_FNF (2) 文件未找到
● SE_ERR_PNF (3) 路径未找到
● SE_ERR_ACCESSDENIED (5) 拒绝访问
● SE_ERR_OOM (8) 内存不足
● SE_ERR_DLLNOTFOUND (32) 动态库未找到
● SE_ERR_SHARE (26) 无法共享打开的文件
● SE_ERR_ASSOCINCOMPLETE (27) 文件关联信息不完整
● SE_ERR_DDETIMEOUT (28) 操作超时
● SE_ERR_DDEFAIL (29) 操作失败
● SE_ERR_DDEBUSY (30) DDE 操作忙
● SE_ERR_NOASSOC (31) 文件关联不可用

**返回值：**
函数执行成功，返回 TRUE ，否则返回 FALSE ，可使用 GetLastError 获取错误码。

● ERROR_FILE_NOT_FOUND 文件不存在
● ERROR_PATH_NOT_FOUND 路径不存在
● ERROR_DDE_FAIL DDE(动态数据交换)失败
● ERROR_NO_ASSOCIATION 未找到与指定文件拓展名关联的应用
● ERROR_ACCESS_DENIED 拒绝访问
● ERROR_DLL_NOT_FOUND 未找到dll
● ERROR_CANCELLED 功能提示用户提供额外信息，但是用户取消请求。
● ERROR_NOT_ENOUGH_MEMORY 内存不足
● ERROR_SHARING_VIOLATION 发生共享冲突

**实例：**

```cpp
SHELLEXECUTEINFO sei;
ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));//使用前最好清空
sei.cbSize = sizeof(SHELLEXECUTEINFO);//管理员权限执行cmd，最基本的使用与 ShellExecute 类似
sei.lpFile = _T("cmd.exe");
sei.nShow = SW_SHOW;
sei.lpVerb = _T("runas");
ShellExecuteEx(&sei);

ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
sei.cbSize = sizeof(SHELLEXECUTEINFO);
sei.lpFile = _T("cmd.exe");
sei.nShow = SW_SHOW;
sei.fMask = SEE_MASK_NOCLOSEPROCESS;//使用 SEE_MASK_NOCLOSEPROCESS 参数
sei.lpVerb = _T("open");
if (ShellExecuteEx(&sei))//执行成功
{
    if (sei.hProcess)//指定 SEE_MASK_NOCLOSEPROCESS 并其成功执行，则 hProcess 将会返回执行成功的进程句柄
        WaitForSingleObject(sei.hProcess, INFINITE);//等待执行完毕
}
else
{
    CString s;
    s.Format(_T("ShellExecuteEx error,error code:%d"), GetLastError());
    MessageBox(s);
}
```

其他诸如打开文件、打开网页等与 ShellExecute 类似。

#### 8.2.1 实战

1. 打开刚刚下载的图片/程序
2. 打开记事本
3. 打开计算器
4. 打开QQ
5. 用默认浏览器打开指定网址

# 9. 文件的删除、复制与重命名操作

### 9.1 文件的删除： DeleteFile

**简介：** 可以使用 DeleteFile 删除指定存在的文件

```cpp
BOOL WINAPI DeleteFile(
  _In_  LPCTSTR lpFileName
);
```

**参数：**
lpFileName
必选项，指定要删除文件的路径。

**返回值：**
函数执行成功，返回 TRUE ，否则返回 FALSE ，可使用 GetLastError 获取错误码。

备注：如果程序尝试删除一个不存在的文件，GetLastError 返回 ERROR_FILE_NOT_FOUND。如果文件是只读的，则 GetLastError 返回 ERROR_ACCESS_DENIED。

### 9.2 文件的复制: CopyFile

**CopyFile 函数说明**

**简介：** 可以使用 CopyFile 拷贝指定存在的文件到目标路径文件。

```cpp
BOOL CopyFile(
  LPCTSTR lpExistingFileName,
  LPCTSTR lpNewFileName,
  BOOL    bFailIfExists
);
```

**参数：**
lpExistingFileName：要拷贝的源文件的路径
lpNewFileName：要拷贝到的目标文件的路径
bFailIfExists：传递TRUE：如果目标文件已经存在，不拷贝，CopyFile 返回 FALSE，
传递 FALSE，如果目标文件已经存在，覆盖目标文件

**返回值：**
函数执行成功，返回 TRUE ，否则返回 FALSE ，可使用 GetLastError 获取错误码。

### 9.3 文件移动: **MoveFile**

==相同路径下就是改名了==

**简介：** 可以使用 MoveFile 移动一个已存在的文件或者**文件夹**到新的位置

```cpp
BOOL MoveFile(
  LPCTSTR lpExistingFileName,
  LPCTSTR lpNewFileName
);
```

**参数：**
lpExistingFileName：要移动的源文件或者文件夹的路径
lpNewFileName：要移动到的目标文件或者文件夹的路径

**返回值：**
函数执行成功，返回 TRUE ，否则返回 FALSE ，可使用 GetLastError 获取错误码。

# 10. GetLastError

![image-20230906170229197](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309061702037.png)

==函数执行成功了，也可以调用GetLastError，他会返回Error_SUCCESS==

# 11. 文件的创建、打开与关闭操作

### 11.1 文件的打开与创建 CreateFile

![image-20230907084830389](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309070848597.png)

CreateFile 函数说明:

**简介：**
创建或打开文件或 I/O 设备。常用的 I/O 设备有：文件，文件流，目录，物理磁盘，卷，控制台缓冲区，磁带驱动器，通信资源，邮筒和管道。
该函数返回一个句柄，该句柄可用于根据文件或设备以及指定的标志和属性访问文件或设备以获取各种类型的 I/O

```cpp
HANDLE WINAPI CreateFile(
  _In_     LPCTSTR               lpFileName,
  _In_     DWORD                 dwDesiredAccess,
  _In_     DWORD                 dwShareMode,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _In_     DWORD                 dwCreationDisposition,
  _In_     DWORD                 dwFlagsAndAttributes,
  _In_opt_ HANDLE                hTemplateFile
);
```

**参数**
**lpFileName**

==文件路径，有长度限制==  要创建或打开的文件或设备的名称。 可以在这个名字中使用正斜杠（/）或反斜杠（\）【注意/是转义符号】。在该函数的ANSI版本中，该名称的长度仅限于MAX_PATH字符。要将此限制扩展为32,767个宽字符，请调用该函数的Unicode版本并将“\\？\”添加到路径中。

**dwDesiredAccess**

==对他干啥，读、写、还是读写==  所请求的文件或设备访问权限。

| **值**        | **含义**                                                     |
| ------------- | ------------------------------------------------------------ |
| 0             | 不希望从设备读取数据或向设备写入数据。如果只想改变设备的配置(如：修改文件的时间戳)，可以传0 |
| GENERIC_READ  | 允许对设备进行只读访问                                       |
| GENERIC_WRITE | 允许对设备进行只写访问，该选项并没有隐含 GENERIC_READ标志    |

**dwShareMode**

==先打开的可以规定后打开的人的权限，读还是写只读等==  文件或设备的请求共享模式。

| **值**            | **含义**                                                     |
| ----------------- | ------------------------------------------------------------ |
| 0                 | 要求独占对设备的访问。如果设备已经打开，CreateFile 调用会失败；如果成功地打开了设备，后续的 CreateFile 调用会失败 |
| FILE_SHARE_READ   | 如果有其他对象要用该设备，我们要求它们不得修改设备的数据；如果设备已经以写入方式或独占方式打开，那么CreateFile调用会失败 |
| FILE_SHARE_WRITE  | 如果有其他内核对象要使用该设备，则要求它们不得读取设备的数据 |
| FILE_SHARE_DELETE | 当对文件进行操作的时候，我们不关心文件是否被逻辑删除或移动。在Windows内部，系统会先将文件标记为待删除，然后当该文件所有已打开的句柄都被关闭的时候，再将其真正的删除 |

**lpSecurityAttributes**

==安全描述符，大多数是继承相关的内容，大多情况下写NULL使用默认的设置==  指向 SECURITY_ATTRIBUTES 结构的指针，该结构包含两个独立但相关的数据成员：一个可选的安全描述符以及一个布尔值，该值确定返回的句柄是否可以被子进程继承。该参数可以是NULL。
如果此参数为NULL，则由CreateFile返回的句柄不能由应用程序可能创建的任何子进程继承，并且与返回句柄关联的文件或设备将获得默认安全描述符。一般都是直接将该参数设置为NULL。

**dwCreationDisposition**
采取存在或不存在的文件或设备的操作。

| **值**            | **含义**                                                     |
| ----------------- | ------------------------------------------------------------ |
| CREATE_NEW        | 告诉CreateFile创建一个新文件，如果同名文件已经存在，那么 CreateFile调用会失败 |
| CREATE_ALWAYS     | ==告诉CreateFile无论同名是否存在都创建新文件，若文件存在，则覆盖== |
| OPEN_EXISTING     | 告诉CreateFile打开一个已有的文件或设备，如果文件或设备不存在，那么CreateFile调用会失败 |
| OPEN_ALWAYS       | 告诉CreateFile打开一个已有的文件，如果文件存在，那么CreateFile会直接打开文件，如果不存在，则会创建一个新文件 |
| TRUNCATE_EXISTING | 告诉CreateFile打开一个已有的文件并将文件大小截断为0字节(==相当于把文件内容清空了==)，如果文件不存在，那么CreateFile调用会失败 |

**dwFlagsAndAttributes**
该参数的用途：
允许我们设置一些标志来微调与设备之间的通信；
如果设备是一个文件，我们还能够设置文件的属性。

| **值**                             | **含义**                                                     |
| ---------------------------------- | ------------------------------------------------------------ |
| **通信标志–缓存**                  |                                                              |
| FILE_FLAG_NO_BUFFERING             | 该标志表示在访问文件的时候不要使用任何数据缓存               |
| FILE_FLAG_SEQUENTIAL_SCAN          | 指定系统顺序地访问文件，系统从文件读取的数据量会超过我们的要求(减少硬盘访问),指定了FILE_FLAG_NO_BUFFERING标志，该标志不生效 |
| FILE_FLAG_RANDOM_ACCESS            | 该标志表示系统不要提前读取文件数据(指定FILE_FLAG_NO_BUFFERING，则该标志不生效) |
| FILE_FLAG_WRITE_THROUGH            | 禁止写入文件时，将数据缓存在内存中(减少数据丢失的可能性)     |
| **通信标志–其他标志**              |                                                              |
| FILE_FLAG_DELETE_ON_CLOSE          | 文件所有的句柄都被关闭后，删除该文件                         |
| FILE_FLAG_BACKUP_SEMANTICS         | 用于备份和恢复软件。在打开或创建任何文件之前，为了确保视图打开文件或创建文件的进程具有所需的访问特权 |
| FILE_FLAG_POSIX_SEMANTICS          | 让CreateFile在创建文件或打开文件时，以区分大小写的方式来查找文件名 |
| FILE_FLAG_OPEN_REPARSE_POINT       | 告诉系统忽略文件的重解析属性(重解析属性允许一个文件系统过滤器对打开文件、读取文件、写入文件以及关闭文件这些行为进行修改) |
| FILE_FLAG_OPEN_NO_RECALL           | 该标志告诉系统不要将文件内容刚从脱机存储器(offline storage，比如磁带)恢复到联机存储器(即online storage, 如硬盘) |
| FILE_FLAG_OVERLAPPED               | 该标志告诉系统我们想以异步方式来访问设备                     |
| **文件设置**                       |                                                              |
| FILE_ATTRIBUTE_ARCHIVE             | 应用程序用该标志来将文件标记为待备份或待删除。当CreateFile创建一个新文件时，会自动设置该标志 |
| FILE_ATTRIBUTE_ENCRYPTED           | 文件是经过加密的                                             |
| FILE_ATTRIBUTE_HIDDEN              | ==文件是隐藏的。它不会出现在通常的目录清单中==               |
| FILE_ATTRIBUTE_NORMAL              | ==文件没有其他属性。只有单独使用的时候，这个标志才有效== 正常的文件 |
| FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | 内容索引服务(content indexing service)不会对文件进行索引     |
| FILE_ATTRIBUTE_OFFLINE             | 文件虽然存在，但文件内容已经被转移到脱机存储中               |
| FILE_ATTRIBUTE_READONLY            | ==文件只读==                                                 |
| FILE_ATTRIBUTE_SYSTEM              | ==文件是操作系统的一部分，专供操作系统使用==                 |
| FILE_ATTRIBUTE_TEMPORARY           | 文件数据只会使用一小段时间。为了将访问时间降至最低，会尽量将文件数据保存在内存中 |

**hTemplateFile**
具有 GENERIC_READ 访问权限的模板文件的有效句柄。模板文件为正在创建的文件提供文件属性和扩展属性。该参数可以是NULL。

**返回值**
如果函数成功，则返回值的是指定文件、设备、命名管道或邮件插槽的句柄；
如果函数失败，则返回值为 INVALID_HANDLE_VALUE。 ==要获得扩展的错误信息，请调用GetLastError==。

**备注**
CreateFile 最初是专门为文件交互而开发的，但后来被扩展和增强，以包括Windows开发人员可用的大多数其他类型的 I/O 设备和机制。

### 11.2 CreateFile实战

1. 使用CreateFile创建一个新的空白文件

   ```cpp
   	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
   		NULL,
   		CREATE_ALWAYS,
   		FILE_ATTRIBUTE_NORMAL,
   		NULL
   		);
   	if (hFile != INVALID_HANDLE_VALUE)
   	{
   		MessageBox(NULL, TEXT("文件创建成功"), TEXT("Caption"), MB_OK);
   	}
   ```

   

2. 使用CreateFile打开一个已经存在的文件

   ```cpp
   	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
   		NULL,
   		OPEN_EXISTING,
   		FILE_ATTRIBUTE_NORMAL,
   		NULL
   		);
   	if (hFile != INVALID_HANDLE_VALUE)
   	{
   		MessageBox(NULL, TEXT("文件打开成功"), TEXT("Caption"), MB_OK);
   	}
   	else
   	{
   		MessageBox(NULL, TEXT("文件不存在"), TEXT("Caption"), MB_OK);
   	}
   ```

   

3. 使用CreateFile打开一个文件，如果文件存在则正常打开，文件不存在则新创建

   ```cpp
   OPEN_ALWAYS
   ```

   

4. 使用CreateFile打开一个文件，如果文件存在则打开并清空内容，文件不存在则新创建

```cpp
CREATE_ALWAYS 或者 OPEN_ALWAYS再嵌套一个截断的
```

### 11.3 关闭文件句柄：CloseHandle

**简介：**==关闭一个内核对象。其中包括文件、文件映射、进程、线程、安全和同步对象等等。== 在 CreateThread 成功之后会返回一个 hThread 的线程句柄，且内核对象的计数加1，CloseHandle之后，引用计数减1，当变为0时，系统删除该内核对象。 若在线程执行完之后，没有调用CloseHandle，在进程执行期间，将会造成内核对象的泄露，相当于句柄泄露，但不同于内存泄露，这势必会对系统的效率带来一定程度上的负面影响。但当进程结束退出后，系统会自动清理这些资源。

```cpp
BOOL CloseHandle(
  HANDLE hObject
);
```

**参数：**
**hObject**：代表一个已经打开的对象句柄，例如：文件句柄，线程句柄，进程句柄等。

**返回值：**
TRUE：执行成功；
FALSE：执行失败，可以调用 GetLastError() 获得具体的执行失败原因

```cpp
	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("文件打开成功"), TEXT("Caption"), MB_OK);
		CloseHandle(hFile);// 防止内存泄露
	}
	else
	{
		MessageBox(NULL, TEXT("文件不存在"), TEXT("Caption"), MB_OK);
		//INVALID_HANDLE_VALUE 就不需要调用CloseHandle，因为他本身就是个无效的句柄
	}
```



# 12. 获取文件大小

文件夹的大小就是文件大小的和

### 12.1 GetFileSize 

**简介：** 该函数用于获取指定文件的大小（长度），以==字节==为单位

```cpp
DWORD GetFileSize(
  HANDLE  hFile,
  LPDWORD lpFileSizeHigh
);
```

**参数：**
hFile：待获取大小的文件句柄，该文件句柄必须具有 GENERIC_READ 或 GENERIC_WRITE 访问权限。

==该文件句柄可以用CreateFile打开文件获得,第二个参数需要传递GENERIC_READ 或 GENERIC_WRITE==

lpFileSizeHigh：指向一个 DWORD 变量的指针，该变量用于接收文件大小高端（第32-63位）部分的值。若不需获取这部分的值，该参数可以为 NULL 。

**返回值：**
如果函数调用成功，返回值为文件大小的低端（第0-31位）的值，如果 lpFileSizeHigh 参数不为 NULL ，该参数对应的变量包含文件大小的高端（第32-63位）部分的值。
如果函数调用失败，并且 lpFileSizeHigh 参数为 NULL ，则返回值为 INVALID_FILE_SIZE 。要获取更多错误信息，请调用 GetLastError 函数。
如果函数调用失败，并且 lpFileSizeHigh 参数不为 NULL ，返回值为 INVALID_FILE_SIZE ，调用 GetLastError 函数返回的错误代码为 NO_ERROR 以外的值。



// 获得小于4GB的文件大小：

DWORD dwSize = GetFileSize(hFile, NULL);

```cpp
	HANDLE hFile =  CreateFile(TEXT("D:\\789.png"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);

		TCHAR szFileSize[128] = { 0 };
		swprintf_s(szFileSize, TEXT("文件的大小是: %d"), dwFileSize);
		MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
		CloseHandle(hFile);

	}
	else
	{
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("Tip"), MB_OK);
	}
```





// 获得大于4GB的文件大小

```cpp
	HANDLE hFile =  CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// 大于4G的需要接住第二位高位，也是一个DWORD类型接收高位
		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHigh);

		// 合成64位整型,跟INT64一个东西
		LONGLONG lFileSize = ((LONGLONG)dwFileSizeHigh << 32) | dwFileSize;

		// 转换成字符串
		TCHAR szFileSize[128] = { 0 };
		swprintf_s(szFileSize, TEXT("文件的大小是: %lld"), lFileSize);
		MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
		CloseHandle(hFile);

	}
	else
	{
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("Tip"), MB_OK);
	}
```



### 12.2 GetFileSizeEx

**简介：** 和 GetFileSize 函数的功能基本一致，该函数也是用于获取指定文件的大小（长度），以字节为单位！

```cpp
BOOL GetFileSizeEx(
  HANDLE         hFile,
  PLARGE_INTEGER lpFileSize
);
```

**参数：**
lpFileSize: 输出参数，指向储存文件大小的一个 LARGE_INTEGER 联合体。

```cpp
#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER {
#else // MIDL_PASS
typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
#endif //MIDL_PASS
    LONGLONG QuadPart;
} LARGE_INTEGER;
```

相关 LARGE_INTEGER 联合体的解释详见：

LARGE_INTEGER 大整数结构体的解析：

在“WinNT.h”文件中定义了一个结构体 LARGE_INTEGER，定义的十分巧妙！

```cpp
#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER {
#else // MIDL_PASS
typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
#endif //MIDL_PASS
    LONGLONG QuadPart;
} LARGE_INTEGER;
```

解释：根据预编译指令若定义宏 MIDL_PASS，则将 结构体命名为 LARGE_INTEGER，若未定义宏 MIDL_PASS，则将联合体命名为 LARGE_INTEGER。
实际上如果编译器具有内置支持64位整数，使用 QuadPart 成员中存储的64位整数。否则，使用 LowPart 和 HighPart 成员的存储的64位整数。

在不支持内置64位情况下，即，采用联合体结构。联合体有两部分组成，DUMMYSTRUCTNAME 和 u：因为联合的长度取决于最长的数据成员的长度，相当于成员公用内存。

①、在小端的情况下，因为低32位数字在前，高32位在后：

如果将这个64位大整数赋值 0x1234：

```cpp
LARGE_INTEGER data;
data.LowPart = 0x34;
data.HighPart = 0x12;
```

②、在大端的情况下,高32位数字在前,低32位在后：
如果将这个64位大整数赋值 0x1234：

```cpp
LARGE_INTEGER data;
data.u.LowPart = 0x34;
data.u.HighPart = 0x12;
```



**返回值：**
函数执行成功，返回TRUE
函数执行失败，返回FALSE，获取更多错误信息，请调用 GetLastError 函数。

```cpp
	HANDLE hFile = CreateFile(TEXT("C:\\Users\\yeshooo\\Downloads\\ubuntu-22.04.2-desktop-amd64.iso"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fiseSize = { 0 };
		BOOL bRet =  GetFileSizeEx(hFile, &fiseSize);

		if (bRet)
		{
			TCHAR szFileSize[128] = { 0 };
			swprintf_s(szFileSize, TEXT("文件的大小是: %lld"), fiseSize.QuadPart);
			MessageBox(NULL, szFileSize, TEXT("Tip"), MB_OK);
		}
		CloseHandle(hFile);

	}
	else
	{
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("Tip"), MB_OK);
	}
```

# 13. 文件的读写操作

### 13.1 文件的写操作： WriteFile

**简介：** 从文件指针指向的位置开始将数据写入到一个文件中, 且支持同步和异步操作。 如果文件打开方式没有指明 FILE_FLAG_OVERLAPPED，当程序调用成功时，它将实际写入文件的字节数保存到 lpNumberOfBytesWriten 指明的地址空间中。 如果文件要交互使用的话，当函数调用完毕时要记得调整文件指针。

```cpp
BOOL WriteFile(
  HANDLE       hFile,
  LPCVOID      lpBuffer,
  DWORD        nNumberOfBytesToWrite,
  LPDWORD      lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped
);
```

**参数说明：**
hFile：需要写入数据的已打开的文件句柄，这个句柄所==关联的文件必须拥有 GENERIC_WRITE 访问权限属性==的文件；
lpBuffer：要写入到文件的缓冲区首地址；
nNumberOfBytesToWrite：要写入数据的字节数量。如写入零字节，表示什么都不写入，但会更新文件的“上一次修改时间”；
lpNumberOfBytesWritten：实际写入文件的字节数量，此变量是用来==返回==的 ；
lpOverlapped：倘若在指定 FILE_FLAG_OVERLAPPED 标志的前提下打开文件，指针不能为空，这个参数就必须引用一个特殊的结构。那个结构定义了一次异步写操作。否则，该参数应置为空；==异步的话传地址，同步的话传NULL==

**返回值：**
函数执行成功，返回TRUE，否则返回FALSE，更多错误信息还请调用 GetLastError 获取错误码

### 13.2 WriteFile实战

1. 写入数字

   ```cpp
   	// 打开
   	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE, FILE_SHARE_READ,
   		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   	if (hFile == INVALID_HANDLE_VALUE)
   	{
   		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
   		return -1;
   	}
   	int num = 123; // 这里写入的是二进制的 {，而不是字符串
   	DWORD dwRealWrite = 0;
   	BOOL bRet = WriteFile(hFile, &num, sizeof(num), &dwRealWrite, NULL);
   
   	if (bRet)
   	{
   		MessageBox(NULL, TEXT("数据写入成功!"), TEXT("Tip"), MB_OK);
   
   	}
   	else
   	{
   		MessageBox(NULL, TEXT("数据写入失败!"), TEXT("Tip"), MB_OK);
   	}
   	CloseHandle(hFile);
   	return 0;
   ```

   

2. 写入英文字母

   ```cpp
   	// 打开
   	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE, FILE_SHARE_READ,
   		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   	if (hFile == INVALID_HANDLE_VALUE)
   	{
   		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
   		return -1;
   	}
   	int num = 123; // 这里写入的是二进制的 {，而不是字符串
   	DWORD dwRealWrite = 0;
   	//BOOL bRet = WriteFile(hFile, &num, sizeof(num), &dwRealWrite, NULL);
   
   	char ch = 'q';
   	BOOL bRet = WriteFile(hFile, &ch, sizeof(ch), &dwRealWrite, NULL);
   
   	if (bRet)
   	{
   		MessageBox(NULL, TEXT("数据写入成功!"), TEXT("Tip"), MB_OK);
   
   	}
   	else
   	{
   		MessageBox(NULL, TEXT("数据写入失败!"), TEXT("Tip"), MB_OK);
   	}
   	CloseHandle(hFile);
   	return 0;
   ```

   

3. 写入包含中文的字符串

   ```cpp
   	// 打开
   	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE, FILE_SHARE_READ,
   		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   	if (hFile == INVALID_HANDLE_VALUE)
   	{
   		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
   		return -1;
   	}
   	int num = 123; // 这里写入的是二进制的 {，而不是字符串
   	DWORD dwRealWrite = 0;
   	//BOOL bRet = WriteFile(hFile, &num, sizeof(num), &dwRealWrite, NULL);
   
   	char ch = 'q';
   	//BOOL bRet = WriteFile(hFile, &ch, sizeof(ch), &dwRealWrite, NULL);
   
   	char szText[] = "我是VC驿站的粉丝！";
   	BOOL bRet = WriteFile(hFile, szText, sizeof(szText), &dwRealWrite, NULL);
   
   	if (bRet)
   	{
   		MessageBox(NULL, TEXT("数据写入成功!"), TEXT("Tip"), MB_OK);
   
   	}
   	else
   	{
   		MessageBox(NULL, TEXT("数据写入失败!"), TEXT("Tip"), MB_OK);
   	}
   	CloseHandle(hFile);
   	return 0;
   ```

   

4. 写入二进制数据

```cpp
	// 打开
	HANDLE hFile = CreateFile(TEXT("D:\\VC驿站ok.txt"), GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("文件打开失败!"), TEXT("Tip"), MB_OK);
		return -1;
	}
	DWORD dwRealWrite = 0;
	Student stud;
	stud.age = 20;
	stud.sex = 'm';
	strcpy_s(stud.name, "zhangsan");
	BOOL bRet = WriteFile(hFile, &stud, sizeof(stud), &dwRealWrite, NULL);
	if (bRet)
	{
		MessageBox(NULL, TEXT("数据写入成功!"), TEXT("Tip"), MB_OK);

	}
	else
	{
		MessageBox(NULL, TEXT("数据写入失败!"), TEXT("Tip"), MB_OK);
	}
	CloseHandle(hFile);
	return 0;
```

### 13.3 文件的读操作： ReadFile

**简介：** 从文件指针指向的位置开始将文件数据读入内存中， 且支持同步和异步操作。 如果文件打开方式没有指明 FILE_FLAG_OVERLAPPED 的话，当程序调用成功时，它将实际读出文件的字节数保存到 lpNumberOfBytesRead 指明的地址空间中。 从文件中读出数据，与 C语言的 fread函数相比，这个函数要灵活的多，适用的场景也很多。该函数能够操作通信设备、管道、套接字以及邮槽。

```cpp
BOOL ReadFile (
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
);
```


**参数：**
hFile：需要读入数据的文件指针，这个指针指向的文件必须是 GENERIC_READ 访问属性的文件；
lpBuffer：接收读入文件数据的缓冲区；==存放从文件中读取的数据==
nNumberOfBytesToRead：指定要读取的字节数；
lpNumberOfBytesRead：指向一个DWORD类型变量的指针，用来接收实际读取的字节数（实际读取的字节数很可能比要读取的字节数小）；
lpOverlapped：OVERLAPPED 结构体指针，如果文件是以 FILE_FLAG_OVERLAPPED 方式打开的话，那么这个指针就不能为 NULL。FILE_FLAG_OVERLAPPED 允许对文件进行重叠操作，或者说异步操作，以后会讲解到。

**返回值：**
函数调用成功，返回TRUE，
调用失败，返回FALSE，如果要获得更多的错误信息，请调用 GetLastError 函数。

# 14. 文件指针

![image-20230908143944564](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309081439395.png)



### 14.1 设置文件指针的位置： 随机读写 SetFilePointer

**SetFilePointer 函数说明** :

**简介：**
使用 SetFilePointer 函数设置文件当前的读写位置。

```cpp
DWORD SetFilePointer(
  HANDLE hFile,
  LONG   lDistanceToMove,
  PLONG  lpDistanceToMoveHigh,
  DWORD  dwMoveMethod
);
```

**参数：**
hFile：已打开的文件句柄，必须==拥有 GENERIC_READ 或者 GENERIC_WRITE 访问权限==；

（都是小写L,类型LONG的意思）lDistanceToMove：低32位的有符号的（有负有正）的值，指定要移动文件指针的字节数。如果lpDistanceToMoveHigh不为NULL，则lpDistanceToMoveHigh和lDistanceToMove将形成一个指定要移动的距离的单个64位有符号值。 如果lpDistanceToMoveHigh为NULL，则lDistanceToMove是一个32位有符号值。 lDistanceToMove的正值在文件中向前移动文件指针，负值则向后移动文件指针。
lpDistanceToMoveHigh：指向要移动的有符号64位距离的高32位。 如果你不需要高位的32位，这个指针必须设置为NULL。 当非NULL时，该参数还接收文件指针新值的高位DWORD。==文件大小小于4GB的时候，此值可以设置为NULL==
dwMoveMethod：设置文件指针的移动起点的位置：FILE_BEGIN=>从文件头开始往后移动，FILE_CURRENT=>从文件的当前指针位置开始移动，FILE_END=>从文件尾部开始往前移动。 ==设置从后往前移动还是从前往后移动==

**返回值：**
如果SetFilePointer函数成功并且lpDistanceToMoveHigh为NULL，则返回值是新文件指针的低位DWORD。 如果lpDistanceToMoveHigh不为NULL，则函数返回新文件指针的低位DWORD，并将新文件指针的高位DWORD放入该参数指向的LONG中。
如果函数失败并且lpDistanceToMoveHigh为NULL，则返回值为INVALID_SET_FILE_POINTER。 要获得扩展的错误信息，请调用GetLastError。
如果函数失败，并且lpDistanceToMoveHigh非空，则返回值为INVALID_SET_FILE_POINTER。 但是，由于INVALID_SET_FILE_POINTER是新文件指针的低位DWORD的有效值，因此必须检查GetLastError以确定是否发生错误。 如果发生错误，GetLastError返回非NO_ERROR值。
如果新文件指针的值为负值，则函数失败，文件指针不移动，GetLastError返回的代码为ERROR_NEGATIVE_SEEK。



----

**SetFilePointerEx 函数说明**

**简介：** 使用 SetFilePointerEx 函数设置文件当前的读写位置。

  **声明：**

```cpp
BOOL SetFilePointerEx(
        HANDLE    hFile,
        LARGE_INTEGER    liDistanceToMove,
        PLARGE_INTEGER    pliNewFilePointer,
        DWORD    dwMoveMethod
);
```


**参数：**
hFile：已打开的文件句柄，必须拥有 GENERIC_READ 或者 GENERIC_WRITE 访问权限；
liDistanceToMove：标识文件指针要移动多少个字节；
pliNewFilePointer：系统会在pliNewFilePointer参数指向的 LARGE_INTEGER 结构体中保存文件指针的新值；
dwMoveMethod：标识移动文件指针的三种方式，FILE_BEGIN、FILE_CURRENT、FILE_END，解释详见：SetFilePointer 函数说明

**返回值：**
函数执行成功返回TRUE，函数执行失败返回FALSE，获得更多错误信息请调用：GetLastError

# 15. 目录的创建与删除

文件剩下的各种操作省略

# 16. Win32窗口程序的创建流程

**简介：** 创建一个重叠窗口，弹出窗口或子窗口。 它指定窗口类，窗口标题，窗口样式，以及（可选）窗口的初始位置和大小。 该函数还指定窗口的父级或所有者（如果有）以及窗口的菜单。 除了CreateWindow支持的样式之外，要使用扩展的窗口样式，请使用CreateWindowEx函数。

**CreateWindow 函数说明**

```cpp
HWND WINAPI CreateWindow(
  LPCTSTR   lpClassName,
  LPCTSTR   lpWindowName,
  DWORD     dwStyle,
  int       x,
  int       y,
  int       nWidth,
  int       nHeight,
  HWND      hWndParent,
  HMENU     hMenu,
  HINSTANCE hInstance,
  LPVOID    lpParam
);
```

**参数：**
lpClassName：
指向一个空结束的字符串或整型数atom。如果该参数是一个整型量，它是由此前调用theGlobalAddAtom函数产生的全局量。这个小于0xC000的16位数必须是lpClassName参数字的低16位，该参数的高位必须是0。
如果lpClassName是一个字符串，它指定了窗口的类名。**这个类名可以是任何用函数RegisterClass注册的类名，或是任何预定义的控制类名**。请看说明部分的列表。==窗口类名==

lpWindowName：
指向一个指定窗口名的空结束的字符串指针。
如果窗口风格指定了标题条，由lpWindowName指向的窗口标题将显示在标题条上。当使用Createwindow函数来创建控制例如按钮，选择框和静态控制时，可使用lpWindowName来指定控制文本。==标题==

dwStyle：

==底下的窗口可以按照用途进行按位或组合使用==

指定创建窗口的风格。该参数可以是下列窗口风格的组合再加上说明部分的控制风格。风格意义：
WS_BORDER：创建一个带边框的窗口。
WS_CAPTION：创建一个有标题框的窗口（包括WS_BORDER风格）。
WS_CHILD：创建一个子窗口。这个风格不能与WS_POPUP风格合用。
WS_CHILDWINDOW：与WS_CHILD相同。
WS_CLIPCHILDREN：当在父窗口内绘图时，排除子窗口区域。在创建父窗口时使用这个风格。
WS_CLIPSIBLINGS：排除子窗口之间的相对区域，也就是，当一个特定的窗口接收到WM_PAINT消息时，WS_CLIPSIBLINGS 风格将所有层叠窗口排除在绘图之外，只重绘指定的子窗口。如果未指定WS_CLIPSIBLINGS风格，并且子窗口是层叠的，则在重绘子窗口的客户区时，就会重绘邻近的子窗口。
WS_DISABLED：创建一个初始状态为禁止的子窗口。一个禁止状态的窗口不能接受来自用户的输入信息。
WS_DLGFRAME：创建一个带对话框边框风格的窗口。这种风格的窗口不能带标题条。
WS_GROUP：指定一组控制的第一个控制。这个控制组由第一个控制和随后定义的控制组成，自第二个控制开始每个控制，具有WS_GROUP风格，每个组的第一个控制带有WS_TABSTOP风格，从而使用户可以在组间移动。用户随后可以使用光标在组内的控制间改变键盘焦点。
WS_HSCROLL：创建一个有水平滚动条的窗口。
WS_ICONIC：创建一个初始状态为最小化状态的窗口。与WS_MINIMIZE风格相同。
WS_MAXIMIZE：创建一个初始状态为最大化状态的窗口。
WS_MAXIMIZEBOX：创建一个具有最大化按钮的窗口。该风格不能与WS_EX_CONTEXTHELP风格同时出现，同时必须指定WS_SYSMENU风格。
WS_OVERLAPPED：产生一个层叠的窗口。一个层叠的窗口有一个标题条和一个边框。与WS_TILED风格相同。
WS_OVERLAPPEDWINDOW：创建一个具有WS_OVERLAPPED，WS_CAPTION，WS_SYSMENU WS_THICKFRAME，WS_MINIMIZEBOX，WS_MAXIMIZEBOX风格的层叠窗口，与WS_TILEDWINDOW风格相同。
WS_POPUP：创建一个弹出式窗口。该风格不能与WS_CHILD风格同时使用。
WS_POPUPWINDOW：创建一个具有WS_BORDER，WS_POPUP,WS_SYSMENU风格的窗口，WS_CAPTION和WS_POPUPWINDOW必须同时设定才能使窗口某单可见。
WS_SIZEBOX：创建一个可调边框的窗口，与WS_THICKFRAME风格相同。
WS_SYSMENU：创建一个在标题条上带有窗口菜单的窗口，必须同时设定WS_CAPTION风格。
WS_TABSTOP：创建一个控制，这个控制在用户按下Tab键时可以获得键盘焦点。按下Tab键后使键盘焦点转移到下一具有WS_TABSTOP风格的控制。
WS_THICKFRAME：创建一个具有可调边框的窗口，与WS_SIZEBOX风格相同。
WS_TILED：产生一个层叠的窗口。一个层叠的窗口有一个标题和一个边框。与WS_OVERLAPPED风格相同。
WS_TILEDWINDOW:创建一个具有WS_OVERLAPPED，WS_CAPTION，WS_SYSMENU， WS_THICKFRAME，WS_MINIMIZEBOX，WS_MAXIMIZEBOX风格的层叠窗口。与WS_OVERLAPPEDWINDOW风格相同。
WS_VISIBLE：创建一个初始状态为可见的窗口。
WS_VSCROLL：创建一个有垂直滚动条的窗口。

x：
指定窗口的初始水平位置。对一个层叠或弹出式窗口，X参数是屏幕坐标系的窗口的左上角的初始X坐标。对于子窗口，x是子窗口左上角相对父窗口客户区左上角的初始X坐标。如果该参数被设为CW_USEDEFAULT则系统为窗口选择缺省的左上角坐标并忽略Y参数。CW_USEDEFAULT只对层叠窗口有效，如果为弹出式窗口或子窗口设定，则X和y参数被设为零。

y：
指定窗口的初始垂直位置。对一个层叠或弹出式窗口，y参数是屏幕坐标系的窗口的左上角的初始y坐标。对于子窗口，y是子窗口左上角相对父窗口客户区左上角的初始y坐标。对于列表框，y是列表框客户区左上角相对父窗口客户区左上角的初始y坐标。如果层叠窗口是使用WS_VISIBLE风格位创建的并且X参数被设为CW_USEDEFAULT，则系统将忽略y参数。

nWidth：
以设备单元指明窗口的宽度。对于层叠窗口，nWidth或是屏幕坐标的窗口宽度或是CW_USEDEFAULT。若nWidth是CW_USEDEFAULT，则系统为窗口选择一个缺省的高度和宽度：缺省宽度为从初始X坐标开始到屏幕的右边界，缺省高度为从初始Y坐标开始到目标区域的顶部。CW_USEDEFAULT只对层叠窗口有效；如果为弹出式窗口和子窗口设定CW_USEDEFAULT标志则nWidth和nHeight被设为零。

nHeight：
以设备单元指明窗口的高度。对于层叠窗口，nHeight是屏幕坐标的窗口宽度。若nWidth被设为CW_USEDEFAULT，则系统忽略nHeight参数。

hWndParent：
指向被创建窗口的父窗口或所有者窗口的句柄。若要创建一个子窗口或一个被属窗口，需提供一个有效的窗口句柄。这个参数对弹出式窗口是可选的。Windows NT 5.0；创建一个消息窗口，可以提供HWND_MESSAGE或提供一个己存在的消息窗口的句柄。

hMenu：
菜单句柄，或依据窗口风格指明一个子窗口标识。对于层叠或弹出式窗口，hMenu指定窗口使用的菜单：如果使用了菜单类，则hMenu可以为NULL。对于子窗口，hMenu指定了该子窗口标识（一个整型量），一个对话框使用这个整型值将事件通知父类。应用程序确定子窗口标识，这个值对于相同父窗口的所有子窗口必须是唯一的。

hlnstance：
与窗口相关联的模块实例的句柄。

lpParam：
指向一个值的指针，该值传递给窗口WM_CREATE消息。该值通过在IParam参数中的CREATESTRUCT结构传递。如果应用程序调用CreateWindow创建一个MDI客户窗口，则lpParam必须指向一个CLIENTCREATESTRUCT结构。
返回值：如果函数成功，返回值为新窗口的句柄：如果函数失败，返回值为NULL。若想获得更多错误信息，请调用GetLastError函数。

备注：
在返回前，CreateWindow给窗口过程发送一个WM_CREATE消息。对于层叠，弹出式和子窗口，CreateWindow给窗口发送WM_CREATE，WM_GETMINMAXINFO和WM_NCCREATE消息。消息WM_CREATE的IParam参数包含一个指向CREATESTRUCT结构的指针。如果指定了WS_VISIBLE风格，CreateWindow向窗口发送所有需要激活和显示窗口的消息。

**返回值：**
函数执行成功，返回新创建窗口的窗口句柄，函数执行失败返回NULL，要获取更为详细的错误信息，还请调用 GetLastError

### 1. 注册窗口类

 **RegisterClassEx 函数说明**

```cpp
ATOM WINAPI RegisterClassEx(
  const WNDCLASSEX *lpwcx
);
```

**参数：**
Ipwcx：指向一个 WNDCLASSEX 结构的指针。在传递给这个函数之前，必须在结构内填充适当的类的属性。
WNDCLASSEX 属于一个窗口类，WNDCLASSEX 中最重要的成员莫过于lpfnWndProc了。前缀 lpfn 表示该成员是一个指向函数的指针。该函数就是窗口函数，或者说处理窗口消息的回调函数。
每一个窗口类必须有一个窗口函数，当 Windows 把属于特定窗口的消息发送给该窗口时，该窗口的窗口类负责处理所有的消息，如键盘消息或鼠标消息。由于窗口函数差不多智能地处理了所有的窗口消息循环，所以您只要在其中加入消息处理过程即可。

WNDCLASSEX 结构体原型：

```cpp
typedef struct tagWNDCLASSEX {
  UINT      cbSize;
  UINT      style;
  WNDPROC   lpfnWndProc;
  int       cbClsExtra;
  int       cbWndExtra;
  HINSTANCE hInstance;
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCTSTR   lpszMenuName;
  LPCTSTR   lpszClassName;
  HICON     hIconSm;
} WNDCLASSEX, *PWNDCLASSEX;
```

1、cbSize：
WNDCLASSEX 的大小。我们可以用sizeof（WNDCLASSEX）来获得准确的值。==因为不同系统版本这个结构体大小不同，内部成员个数也不一样，这样就可以根据大小确定系统版本==
2、style：
从这个窗口类派生的窗口具有的风格。您可以用“or”操作符来把几个风格或到一起。
3、lpfnWndProc：
窗口处理函数的指针。也可以叫窗口过程。
4、cbClsExtra：
指定紧跟在窗口类结构后的附加字节数。
5、cbWndExtra：
指定紧跟在窗口实例的附加字节数。如果一个应用程序在资源中用CLASS伪指令注册一个对话框类时，则必须把这个成员设成DLGWINDOWEXTRA。
6、hInstance：
本模块的实例句柄。
7、hIcon：
图标的句柄。
8、hCursor：
光标的句柄。
9、hbrBackground：
背景画刷的句柄。
10、lpszMenuName：
指向菜单的指针。
11、lpszClassName：
指向类名称的指针。
12、hIconSm：
和窗口类关联的小图标。如果该值为NULL。则把hIcon中的图标转换成大小合适的小图标。

**返回值：**
如果函数成功，返回这个窗口类型的标识号；如果函数失败，返回值为FALSE。若想获得更多错误信息，请调用GetLastError函数

### 2. 创建窗口，指定注册窗口类，窗口标题，窗口的大小



### 3. 显示窗口

**ShowWindow 函数说明**

**函数简介：** 该函数设置指定窗口的显示状态。

```cpp
BOOL ShowWindow(
  HWND hWnd,
  int  nCmdShow
);
```

**函数参数：**
hWnd：指窗口句柄；
nCmdShow：指定窗口如何被显示。如果发送应用程序的程序提供了 STARTUPINFO 结构，则应用程序第一次调用 ShowWindow 时该参数被忽略。否则，在第一次调用ShowWindow函数时，该值应为在函数WinMain中nCmdShow参数。
在随后的调用中，该参数可以为下列值之一：
SW_FORCEMINIMIZE：在 WindowNT 5.0 中最小化窗口，即使拥有窗口的线程被挂起也会最小化。在从其他线程最小化窗口时才使用这个参数。nCmdShow = 11。
SW_HIDE：隐藏窗口并激活其他窗口。nCmdShow = 0。
SW_MAXIMIZE：最大化指定的窗口。nCmdShow = 3。
SW_MINIMIZE：最小化指定的窗口并且激活在Z序中的下一个顶层窗口。nCmdShow = 6。
SW_RESTORE：激活并显示窗口。如果窗口最小化或最大化，则系统将窗口恢复到原来的尺寸和位置。在恢复最小化窗口时，应用程序应该指定这个标志。nCmdShow = 9。
SW_SHOW：在窗口原来的位置以原来的尺寸激活和显示窗口。nCmdShow = 5。
SW_SHOWDEFAULT：依据在STARTUPINFO结构中指定的SW_FLAG标志设定显示状态，STARTUPINFO 结构是由启动应用程序的程序传递给CreateProcess函数的。nCmdShow = 10。
SW_SHOWMAXIMIZED：激活窗口并将其最大化。nCmdShow = 3。
SW_SHOWMINIMIZED：激活窗口并将其最小化。nCmdShow = 2。
SW_SHOWMINNOACTIVE：窗口最小化，激活窗口仍然维持激活状态。nCmdShow = 7。
SW_SHOWNA：以窗口原来的状态显示窗口。激活窗口仍然维持激活状态。nCmdShow = 8。
SW_SHOWNOACTIVATE：以窗口最近一次的大小和状态显示窗口。激活窗口仍然维持激活状态。nCmdShow = 4。
SW_SHOWNORMAL：激活并显示一个窗口。如果窗口被最小化或最大化，系统将其恢复到原来的尺寸和大小。应用程序在第一次显示窗口的时候应该指定此标志。nCmdShow = 1。

**返回值：**
如果窗口之前可见，则返回值为非零。如果窗口之前被隐藏，则返回值为零。

### 4. 开始消息循环

**GetMessage 函数说明**

**函数简介：**
GetMessage是从调用线程的消息队列里取得一个消息并将其放于指定的结构。此函数可取得与指定窗口联系的消息和由PostThreadMessage寄送的线程消息。此函数接收一定范围的消息值。GetMessage不接收属于其他线程或应用程序的消息。获取消息成功后，线程将从消息队列中删除该消息。函数会一直等待直到有消息到来才有返回值。

```cpp
BOOL GetMessage(
  LPMSG lpMsg,
  HWND  hWnd,
  UINT  wMsgFilterMin,
  UINT  wMsgFilterMax
);
```


**参数说明：**
lpMsg：指向MSG结构的指针，该结构从线程的消息队列里接收消息信息。
hWnd：取得其消息的窗口的句柄。当其值取NULL时，GetMessage为任何属于调用线程的窗口检索消息，线程消息通过PostThreadMessage寄送给调用线程。
wMsgFilterMin：指定被检索的最小消息值的整数。
wMsgFilterMax：指定被检索的最大消息值的整数。

**返回值：**
如果函数取得 WM_QUIT 之外的其他消息，返回非零值。如果函数取得 WM_QUIT 消息，返回值是零。如果出现了错误，返回值是-1。
例如，当hWnd是无效的窗口句柄或lpMsg是无效的指针时。若想获得更多的错误信息，请调用GetLastError函数。

----

**TranslateMessage 函数说明**

```cpp
BOOL TranslateMessage(
  const MSG *lpMsg
);
```

**参数说明：**
IpMsg：指向含有消息的MSG结构的指针，该结构里含有用函数GetMessage或PeekMessage从调用线程的消息队列里取得的消息信息。

**返回值：**
如果消息被转换（即，字符消息被寄送到调用线程的消息队列里），返回非零值。如果消息是WM_KEYDOWN，WM_KEYUP WM_SYSKEYDOWN或WM_SYSKEYUP，返回非零值，不考虑转换。如果消息没被转换（即，字符消息没被寄送到调用线程的消息队列里），返回值是零。

备注：此函数不修改由参数IpMsg指向的消息。
WM_KEYDOWN 和 WM_KEYUP 组合产生一个 WM_CHAR 或 WM_DEADCHAR 消息。
WM_SYSKEYDOWN 和 WM_SYSKEYUP 组合产生一个 WM_SYSCHAR 或 WM_SYSDEADCHAR 消息。TranslateMessage为 那些由键盘驱动器映射为ASCll字符的键产生 WM_CHAR 消息。
如果应用程序为其他用途处理虚拟键消息，不应调用 TranslateMessage。例如，如果调用 TranslateAccelerator 返回一个非零值，应用程序不应调用 TranslateMessage。

TranslateMessage 只能用于转换调用 GetMessage 或 PeekMessage 接收的消息。

----

**DispatchMessage 函数说明**

**函数简介：** 该函数分发一个消息给窗口程序。通常消息从 GetMessage 函数获得或者 TranslateMessage 函数传递的。消息被分发到回调函数（过程函数)，作用是消息传递给操作系统，然后操作系统去调用我们的回调函数，也就是说我们在窗体的过程函数中处理消息。

```cpp
LRESULT DispatchMessage(
  const MSG *lpMsg
);
```

**参数说明：**
lpmsg：指向含有消息的MSG结构的指针。

**返回值：**
返回值是窗口程序返回的值。尽管返回值的含义依赖于被调度的消息，但返回值通常被忽略。
备注：MSG结构必须包含有效的消息值。如果参数lpmsg指向一个WM_TIMER消息，并且WM_TIMER消息的参数IParam不为NULL，则调用IParam指向的函数，而不是调用窗口程序

### 5. 窗口函数中处理窗口消息



### 补充：**Windows消息机制的理解**

**一． 消息的概念** 当用户单击鼠标、改变窗口大小或者按下键盘上的一个键等用户动作时，会触发点击，按键等事件，而windows系统把这些事件转化为特定的消息，并放置在应用程序的消息列队中（windows为当前运行的每一个windows程序维护了一个消息列队）等待应用程序的处理，而应用程序不断从消息列队中取出消息，并进行响应。所以说Windows是基于消息的事件驱动模式。  

**二． 消息如何被处理？（窗口函数）** 按照一般写程序的思维而言，我们希望传给函数正确的参数来调用这个函数。而windows消息可以理解为指定了的函数的参数，例如：用户需要关闭某个窗口，点击了右上角的X后，windows便把这个行为转换为消息加入消息列队，而这个消息可能包含了“需要关闭的是哪一个窗口（窗口句柄）”，“关闭窗口的信息”这样的参数，应用程序从消息列队中读取到了这个消息，并把它交给“关闭窗口”这个函数来进行处理，而这个函数就叫做“窗口函数”，这样听起来不好理解，可以加长的理解为“处理窗口消息的函数过程”。 　　而如果没有对应的函数来处理这个消息呢？例如，拖动窗口。为此，windows会用默认的消息处理函数（DefWindowProc）来进行处理. 

 **三． 消息的组成** 消息结构体长这样：

```cpp
typedef struct tagMSG {
        HWND hwnd; //消息句柄，理解为窗口的唯一标志
        UINT message; //消息类型，分为windows消息和用户自己定义的消息
        WPARAM wParam; //消息附加参数
        LPARAM lParam; //消息附加参数
        DWORD time; //消息产生时间爱你
        POINT pt; //消息被发送时的光标位置
} MSG;
```



**四． 消息发送的方式**

1. 投递消息PostMessage

异步执行，发送到消息列队后立即返回无序等待。
应用程序一般是投递一个消息来通知指定窗口完成某个任务
BOOL PostMessage( HWND hWnd, //目标窗口句柄 UINT Msg, //消息类型 WPARAM wParam, //第一个消息参数 LPARAM lParam //第二个消息参数 );

2. 发送消息 SendMessage

同步执行，需要等到消息的返回码之后在继续。
基本上所有的输入类消息，都是以投递的方式抵达应用的，而其他的消息，则大部分是采取了发送方式。
LRESULT SendMessage( HWND hWnd, //接收消息的目标窗口 UINT Msg, //消息类型 WPARAM wParam, //第一个消息参数 LPARAM lParam //第二个消息参数 );

**五． 消息循环实例**

```cpp
while(GetMessage (&msg, NULL, 0, 0)) //消息循环，不断从消息列队取出消息
{ 
TranslateMessage (&msg) ; //翻译键盘消息
DispatchMessage (&msg) ; //把消息发送给窗口过程
}

//消息过程是一个CALLBACK回调函数，每处理一个消息会返回一个值给windows

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    HDC         hdc;  
    PAINTSTRUCT ps;  
    RECT        rect;  
  
    switch (message)  
    {  
    case WM_CREATE:  
        PlaySound(TEXT("hello.wav"), NULL, SND_FILENAME | SND_ASYNC);  //窗口创建的时候播放音乐
        return 0;  
  
    case WM_DESTROY:  
        PostQuitMessage(0);  //把退出窗口的消息插入消息列队
        return 0;  
    }  
  
    return DefWindowProc(hwnd, message, wParam, lParam);  //默认的消息处理函数
}
```

# 17. 窗口控件的创建与响应

![image-20230909104728684](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091047135.png)

### 17.1 控件的窗口类注册

![image-20230909104947995](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091049984.png)

WC:WINDOW Class ? Windos Control

[(CommCtrl.h) 的窗口类 - 中文文档](https://learn.microsoft.com/zh-cn/windows/win32/controls/common-control-window-classes)

[Window Classes (CommCtrl.h) - 英文文档](https://learn.microsoft.com/en-us/windows/win32/controls/common-control-window-classes)

### 17.2 控件的创建时机：

![image-20230909105724903](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091057939.png)

![image-20230909105859706](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091059024.png)

==建议使用第二种，在主窗口创建成功之后，显示之前，窗口函数会收到WM_CREATE消息，在窗口函数中创建子窗口==

### 17.3 案例1： 创建按钮控件

![image-20230909111721837](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091117555.png)

### 17.4 按钮控件的点击响应：

![image-20230909112920386](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309091129088.png)
