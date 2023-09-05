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