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