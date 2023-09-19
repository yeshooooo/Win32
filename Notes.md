# 0. 一些技巧性的东西

#### 0.1 同时产生一个dos窗口调试

每次用MESSAGE太麻烦

首先定义一个全局句柄

```cpp
HANDLE g_hOutput = 0; // 接受标准输出句柄
```

![image-20230824101024825](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241010987.png)

然后再winmain里创建dos

```cpp
	AllocConsole();// 增加dos窗口
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // 拿到标准输出句柄

```

![image-20230824101050691](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241010961.png)

然后具体应用到需要打印调试到控制台的地方

```cpp
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_SIZE: 宽：%d,高：%d\n", nWidth, nHeight);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
```



![image-20230824100913290](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241009501.png)

上面这样可能会中文乱码



方式二：

![image-20230824102455093](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241024196.png)

其他帖子关于使用

```shell
1、#include <conio.h>
 
2、在需要开启控制台窗口的地方调用
AllocConsole();//注意检查返回值
 
3、在需要输出调试的时候调用_cprintf等函数
如_cprintf("i=%d\n", i);
 
4、关闭控制台的时候调用
FreeConsole();
 
注意：上述方法在输出中文时会出现乱码，如果需要输出中文，请使用下面的方法：
AllocConsole();
freopen( "CONOUT$","w",stdout);
printf("i的值为%d\n", i);
FreeConsole();

方法二：
#include <io.h> 
#include <fcntl.h>
 
void InitConsoleWindow() 
{ 
    AllocConsole(); 
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
    int hCrt = _open_osfhandle((long)handle,_O_TEXT); 
    FILE * hf = _fdopen( hCrt, "w" ); 
    *stdout = *hf; 
}
BOOL CHelloMFCDlg::OnInitDialog()
{ 
    CDialog::OnInitDialog();
 
    InitConsoleWindow();  // add
    printf("str = %s\n ", "Debug output goes to terminal\n");
    ...... 
}
```

==个人使用无中文乱码版本方法==

```cpp
void OnSize(HWND hWnd, LPARAM lParam) {
	// 拿到变化后的宽跟高
	short nWidth = LOWORD(lParam);
	short nHeight = HIWORD(lParam);

	char szText[256] = { 0 };
	sprintf_s(szText, "WM_SIZE: 宽：%d,高：%d\n", nWidth, nHeight);
	AllocConsole();
	FILE* stream;
	freopen_s(&stream,"CONOUT$", "w", stdout);

	printf("i的值为%s\n", szText);
	FreeConsole();
	//WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
	
}
```

![image-20230824162139159](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241621504.png)

# 1.MSDN 抄的一些数据类型

### 

### 1.1 整数类型

![image-20230817153159152](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171531270.png)

### 1.2 字符串

![image-20230817153000116](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171530179.png)

### 1.3 windows核心编程中对字符串用法的总结

1. 为了让ansi和unicode都能通过编译，可以定义类型为TCHAR ,TCHAR的具体类型取决于是否声明了宏UNICODE ，字符串常量可以用TEXT()宏来包裹，可以屏蔽L和ansi字符串常量的区别，具体类型由UNICODE宏控制

   一个注意点是msdn上说这俩宏用的少

2.![image-20230819112243144](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308191122260.png)





# 2.WinMain详解

```cpp
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdSHow);
```

* 参数详解

  * HINSTANCE hInstance： 应用程序当前实例句柄。分开看，一个是handle句柄，一个是instance实例
  * HINSTANCE hPrevInstance: 应用程序的先前实例的句柄。同一个程序打开两次，出现两个窗口。第一次打开的窗口就是先前实例的窗口。对于一个32位程序，该参数总为NULL。
  * LPSTR lpCmdLine：命令行的字符串指针。
  * int nCmdSHow：窗口显示方式、默认为SW_SHOWDEFAULT。

* 奇怪的符号

  | 符号      | 原型                                                         | 说明                                           |
  | --------- | ------------------------------------------------------------ | ---------------------------------------------- |
  | WINAPI    | #define WINAPI  __stdcall                                    | 标准调用约定、__stdcall是专门用来调用Win API的 |
  | HINSTANCE | struct HINSTANCE__ {int unused;}; <br />typedef struct HINSTANCE__ * HINSTANCE | 结构体类型指针                                 |
  | LPSTR     | typedef CHAR *LPSTR;                                         | char* 类型                                     |
  | HANDLE    | typedef void * HANDLE;                                       | void类型指针                                   |
  |           |                                                              |                                                |

  HANDLE 句柄是通用情况，HINSTANCE可以看成是他的特殊情况
  
* 其他注意点

  ```cpp
  //小注意点：
   // __stdcall 的宏定义，叫CALLBACK WINAPI  APIENTRY APIPRIVATE PASCAL 都行 WINAPIV 是另一个__cdecl的调用约定
  // 调用约定的作用 1. 决定函数名字编译方式 2. 参数入栈顺序 3. 函数的调用时间
  //自己写的winmain 不写_In_的话，winmain也能编译，但是会有下划线，显示批注不一致，这里是用vx自动填充的，右键转到声明或者定义直接复制过来就行
  //这几个小小的红色的技术叫做sal技术，这是给编译器看的注释，我们自己写的注释是给人看的，编译器不认识。 全称： source-code annotation language 叫做 microsoft源代码注释语言 ，gcc，cfree，dev等中可能就不好使了
  //功能 本质就是辅助我们避免野指针
  //官方文档地质：https://docs.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=msvc-160
  //一共8条
  //1. __In__ 功能 ： 表示该参数由我们调用者进行传递
  //			要求 ： 调用者必须提供缓冲区并对其进行初始化
  //			不能是空指针NULL
  //            注意 ： 允许使用__in__ 但是分析器对非指针变量上忽略__In  也就是专门给指针准备的
  
  // 2. _In_opt_ 功能： 同上 特点： 指针可以传递NULL
  //	 注意： 表示可选的意思  既然一个指针参数传递了一个NULL ，说明此参数可忽略，当然不管传递的什么，我们使用指针之前一定要线先检测NULL
  
  // 3. _Out_ 功能： 表示传入指向缓冲区的非NULL指针，函数内部对其赋值 ---即传址调用
  //	特点： 不可为NULL 一定要有合法空间				调用者不必再调用之前初始化缓冲区		被调用的函数承诺在返回之前初始化它
  // 
  // 
  // 4. _Out_opt_ 同上，但是可以是NULL 说明可用可不用  不管可否是NULL，我们在函数内部使用之前一定要判断是否是NULL 
  ```

  

### 2.1 应用程序实例句柄

==句柄有很多种，如窗口句柄，菜单句柄，实例句柄，光标句柄，位图句柄==

![](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308172311062.png)

![image-20230817160146245](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171601527.png)

### 2.2 窗口显示方式

int nCmdSHow

![image-20230817163220156](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171632380.png)

### 2.3 设置入口点同时弹出控制台应用程序

![image-20230817171534044](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171715259.png)

```cpp
#pragma comment(linker,"/subsystem:\"console\"/entry:\"WinMainCRTStartup\"")
或者
#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")
```

常见入口点

```shell
main：

mainCRTStartup(exe_main.cpp)->__scrt_common_main(exe_common.inl)->__scrt_common_main_seh(exe_common.inl)->invoke_main(exe_common.inl)->main

WinMain:

WinMainCRTStartup(exe_main.cpp)->__scrt_common_main(exe_common.inl)->__scrt_common_main_seh(exe_common.inl)->invoke_main(exe_common.inl)->WinMain

DllMain:

DllMainCRTStartup(dll.dllmain.cpp)->dllmain_dispatch(dll.dllmain.cpp)->DllMain

```

[关于链接器入口点的介绍](https://blog.csdn.net/iBliBiliBelieve/article/details/83614820?spm=1001.2014.3001.5502)

![image-20230817231631388](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308172316530.png)

**也可以在配置里设置**

![image-20230817172036419](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171720469.png)

![image-20230817172055431](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171720504.png)

# 3.Windows窗口

### 1. 步骤

![image-20230818151759898](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181518032.png)

### 2. 窗口类

![image-20230818151915520](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181519769.png)

##### 2.1 注册窗口类

![image-20230818152602187](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181526404.png)

##### 2.2 窗口类样式

![image-20230818153235996](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181532279.png)

![image-20230818153304354](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181533495.png)

##### 2.3 创建窗口

![image-20230818153511242](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181535430.png)

##### ![image-20230818154251221](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181542414.png)

##### 2.4 显示窗口

![image-20230818154758147](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181547329.png)

##### 2.5 更新窗口

![image-20230818154952317](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181549467.png)

##### 2.6 窗口过程函数回调

![image-20230818155239168](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181552313.png)

![image-20230818161736414](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181617690.png)

![image-20230818161937796](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181619059.png)

##### 2.7 消息循环

固定写法

![image-20230818162156925](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181621109.png)

![image-20230818162308411](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181623652.png)

![image-20230818162508357](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181625568.png)

![image-20230818162536521](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308181625653.png)

# 4. WindowsApi常见数据类型

![](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221046060.png)

![image-20230822105036246](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221050425.png)

![image-20230822105241033](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221052293.png)

![image-20230822105411306](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221054551.png)

![image-20230822105504353](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221055557.png)

# 5.句柄的概念

![image-20230822105647244](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221056426.png)

![image-20230822110324170](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221103424.png)

![image-20230822110846204](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221108356.png)

# 6. 符号定义

![image-20230822110928211](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221109298.png)

![image-20230822110950022](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221109241.png)

# 7. 单字节字符集SBCS、多字节字符集MBCS、UNICODE字符集

![image-20230822111638838](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221116067.png)

![image-20230822111759931](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221118185.png)

![image-20230822112012204](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221120553.png)

![image-20230822112240019](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221122145.png)

![image-20230822112503723](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221125072.png)

![image-20230822142423837](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221424186.png)

![image-20230822142755045](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221427209.png)

### 7.1特别说明，ANSI为国际标准的多字节编码，可能会使用gb2312等具体是什么还没确定

![image-20230822153318171](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221533416.png)

![image-20230822155603708](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221556937.png)

### 7.2 解决中文字符串乱码

#include <locale.h>

​	_wsetlocale(LC_ALL, L"chs"); // 设置输出得本地化为中文

```cpp
#include <stdio.h>
#include <string.h>

// 必须包含这个头文件，否则控制台不支持宽字节字符串
#include <locale.h>
int main() {
	const char* str1 = "A中";
	printf("%s %d\n", str1, strlen(str1));

	_wsetlocale(LC_ALL, L"chs"); // 设置输出得本地化为中文
	const wchar_t* str2 = L"A中"; // 必须加L表示字符串是unicode
	wprintf(L"%s %d\n", str2, wcslen(str2));
}
```

# 8.Win32 API的A/W函数

![image-20230822161208940](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221612110.png)

# 9. 消息驱动机制简介简介

![image-20230822171715521](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221717638.png)

![image-20230822171935308](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221719566.png)

![image-20230822172245041](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221722238.png)

### 8.1 消息结构体

![image-20230822172344932](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221723128.png)

### 8.2 windows消息循环

![image-20230822172439925](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221724128.png)

![image-20230822172608143](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221726413.png)

![image-20230822172807852](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221728067.png)

![image-20230822172910496](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221729747.png)

![image-20230822173041832](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221730119.png)

![image-20230822173211355](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308221732598.png)

# 10.达内消息相关

### 10.1 消息基础

#### 10.1.1 消息循环

![image-20230823102713303](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231027561.png)

linux系统消息由几个部分构成是由程序员设定，而windos下是固定的

**派发就是DispatchMessage(&Msg)来派发，派发给窗口的窗口处理函数**==派发其实就是调用我们自己定义的窗口处理函数(回调)==

==每个窗口都应该有窗口处理函数==

==DispatchMessage怎么找到窗口的呢？==

![image-20230823105425670](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231054803.png)



#### 

#### 10.1.2 窗口处理函数的限制

窗口处理函数是有原型限制的

![image-20230823110043849](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231100234.png)

#### 10.1.3 GetMessage()和translatemessage浅谈

GetMessage是去系统某个地方抓本进程的消息

==同一个进程可以包含很多窗口，如果指定第二个参数的话，他只会抓指定窗口的消息，本进程内其他窗口的消息他是不管的,所以一般写NULL，意味着进程内所有的消息都会抓取==

![image-20230823111243995](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231112206.png)

getmessage的返回值

![image-20230823114028071](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231140132.png)



==翻译消息==

翻译消息不是什么消息都翻译，他只翻译键盘消息，而且只翻译键盘的可见字符，上下左右f1这种不翻译，只翻译a-z，因为a-z有大小写之分



![image-20230823114748238](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231147458.png)

### 10.2 常见消息

#### 10.2.1 WM_DESTROY

不是点击关闭按钮的时候产生的消息

![image-20230823160333556](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231603803.png)

#### 10.2.2 WM_SYSCOMMAND

==点击关闭按钮，真正产生的消息是WM_SYSCOMMAND==

==wParam就能看到点了哪产生的消息了==

// lParam低位字节传递的是水平坐标，高位字节传递的是垂直坐标

微软已经提供了取高低位的宏



![image-20230823163852808](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231638103.png)

```cpp
LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY: {
		PostQuitMessage(WM_QUIT); 
		//exit(0);
		return 0;
	}
				   // 点击最大化，最小化，关闭等都会直接产生这个消息
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet =  MessageBox(hWnd, TEXT("是否退出？"), TEXT("Info"), MB_YESNO);
			if (nRet ==IDYES)
			{
				// 什么都不写就会穿透到default下最终退出
			}
			else
			{
				return 0; // return 啥都行，目的是不让DefWindowProc执行
			}
		}
		
	default:
		// 这俩会再处理一遍WM_SYSCOMMAND，产生WM_DESTROY消息，再回上面的
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}
```



#### 10.2.3 WM_CREATE

传过来的lParam要强转为CREATESTRUCT类型

![image-20230823170156116](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231701408.png)

```cpp
void OnCreate(HWND hWnd, LPARAM lParam)
{
	MessageBox(NULL, TEXT("WM_CREATE"), TEXT("Info"), MB_OK);

	CreateWindow(L"Edit", L"hello", WS_CHILD|WS_VISIBLE|WS_BORDER, 0, 0, 200, 200, hWnd, NULL, 0, NULL);
}
```

强转

![image-20230824091421195](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308240914275.png)

#### 10.2.4 WM_SIZE

窗口从无到有这种广义上的大小变化，也会产生WM_SIZE消息

![image-20230824094359388](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308240943605.png)

#### 10.2.5 WM_QUIT

WM_QUIT不用处理，但是他到哪都特殊



![image-20230824104901593](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241049819.png)

### 10.3 消息循环原理

#### 10.3.1 消息循环的阻塞

![image-20230824154632260](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241546709.png)

GetMessage经常阻塞，效率很低

更科学的消息循环写法

```cpp
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// 有消息
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// GetMessage(&msg, NULL, 0, 0)的天敌WM_QUIT
			else
			{
				return 0;
			}
		}
		else
		{
			// 没消息
			// 体面的说法叫空闲处理

		}
	}
```



#### 10.3.2 发送消息（消息是怎么造出来的）

![image-20230824162523231](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241625522.png)

消息的后两个参数这俩参数内部会补充

==试验证明，PostQuitMessage是PostMessage发送的==

```cpp
	case WM_DESTROY: 
		//PostQuitMessage(0); 
		// 
		//exit(0);
		// 试验PostQuitMessage底层调用的是SendMessage还是PostMessage
		//SendMessage(hWnd, WM_QUIT, 0, 0);
		PostMessage(hWnd, WM_QUIT, 0, 0);
		return 0;
```

==SendMessage造的消息没有放到消息队列里==

#### 10.3.3 消息分类

系统消息：1024个==系统定制好的消息，程序员只负责一头，要么发送，要么处理==





![image-20230824172003479](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241720763.png)

```shell
// 自定义消息
// WM_USER就是0x400因为一般不直接写0x400
#define WM_MYMESSAGE WM_USER+1001
```

发送，爱在哪发送在哪发送，爱携带啥信息携带啥信息

send跟post都能发

![image-20230824173137983](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241731056.png)

处理，到自己定义的窗口处理函数中处理，爱咋处理咋处理

![image-20230824173226666](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241732757.png)

![image-20230824173414586](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308241734716.png)

### 10.4 消息队列

#### 10.4.1 消息队列的概念

![image-20230825101542280](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251015460.png)

#### 10.4.2 消息队列的分类

![image-20230825104658595](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251046826.png)

![image-20230825145725098](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251457193.png)

#### 10.4.3 消息和队列的关系

![image-20230825144243395](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251442741.png)

![image-20230825144731517](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251447969.png)



==WM_QUIT==是必须进队列的，其他大多数进不进都行

==WM_CREATE==必须不能进队列，因为他是创建完成之后显示之前的时候，这时候没人能把他抓出来

==消息本身没有队列还是非队列的属性==

#### 10.4.4 深谈GetMessage原理

==GetMessage只能在本进程的消息队列里抓消息==

![image-20230825152127858](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251521585.png)

#### 10.4.5 WM_PAINT消息

![image-20230825152349238](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251523402.png)

![image-20230825161016692](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251610887.png)

![image-20230825161958270](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251619568.png)

==// 绘图代码，必须放在处理WM_PAINT消息时调用。==

### 10.5 键盘消息

#### 10.5.1 键盘消息分类

![image-20230828103055460](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281030703.png)

大小写的a的键码值都是65，所以才需要翻译消息，不然分不出来

#### 10.5.2 WM_CHAR 字符消息

![image-20230828105932795](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281059090.png)

TranslateMessage内部执行过程

![image-20230828110843214](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281108539.png)



### 10.6 鼠标消息

#### 10.6.1 鼠标消息分类

![image-20230825171947940](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308251719313.png)

#### 10.6.2 鼠标基本消息

![image-20230828112138897](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281121260.png)

#### 10.6.3 鼠标双击消息

![image-20230828151407142](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281514557.png)

#### 10.6.4 鼠标滚轮消息

![image-20230828160754629](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281607988.png)

### 10.7 定时器消息

每隔一段炸一次的炸弹

#### 10.7.1 定时器消息介绍

==定时器消息是GetMessage在空闲的时候抓来发送WM_TIMER出去的，有几毫秒的误差，不是特别准==



![image-20230828162404544](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281624985.png)

#### 10.7.2 创建销毁定时器

第一个参数hWnd的作用是找这个窗口的窗口处理函数处理

==定时器埋在哪里都可以==



![image-20230828164701488](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281647803.png)

![image-20230828165603012](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308281656142.png)

# 11.达内资源相关

### 11.1 菜单资源

#### 11.1.1 菜单分类

弹出式菜单主要有两种，一种是下拉菜单，一种是右键菜单

![image-20230829090304260](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308290903411.png)

#### 11.1.2 资源相关

![image-20230829091221619](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308290912800.png)

#### 11.1.3 菜单资源使用

![image-20230829091639378](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308290916665.png)

有三种方法可以挂载菜单

* 注册窗口类时设置菜单

  ![image-20230829105626568](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291056638.png)

* ==创建窗口传参设置菜单（个人更喜欢用）==

  ![image-20230829110727971](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291107100.png)

* 在主窗口WM_CREATE消息中利用SetMenu函数设置菜单

  ![image-20230829111221720](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291112999.png)

  ![image-20230829111235785](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291112966.png)

#### 11.1.4 命令消息处理

![image-20230829111333142](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291113335.png)



#### 11.1.5 菜单项状态



#### 11.1.6 上下文菜单



### 11.2 图标资源

![image-20230829112605892](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308291126218.png)



### 11.3 光标资源

![image-20230830091716582](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308300917958.png)

光标更改比较频繁的时候可以使用SetCursor

==此函数必须在WM_SETCURSOR中处理，只要光标移动，就会产生该消息==

![image-20230830092837414](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308300928681.png)





### 11.4 字符串资源

==字符串资源能非常方便的解决中英文两版的问题==

![image-20230830095402930](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308300954240.png)

字符串表中的每一行都是一个id对应一个字符串

![image-20230830110124937](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301101977.png)



### 11.5 加速键资源

加速键就是快捷键，一般快捷键在菜单栏上有一个一个对应的功能，虽然他们干的事情相同，但是两者在原理上没有一毛钱关系



![image-20230830151218073](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301512437.png)

执行逻辑伪代码

==按加速键也能产生WM_COMMAND消息，但是不是按出来的，是TranslateAccelerator给发送出来的==

![image-20230830152323540](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301523872.png)

![image-20230830152933031](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301529441.png)

![image-20230830153016703](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301530743.png)

这里id故意写成跟菜单项的一致，就不用再去回调函数中处理command消息了，跟menu的公用一个

![image-20230830153215617](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301532799.png)

==加速键id和菜单项id统称为命令id==

也可以用0和1将两者进行区分，不过不会这么用，因为区分的话就没必要绑定id名字一样

![image-20230830153646123](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301536474.png)

# 12. 达内线程相关

### 12.1 线程

==windows不同于linux很多多进程场景，微软主要是应用层，提倡基于线程的开发==

进程开启意味着分内存，不意味着程序执行，线程开启才意味着程序执行





#### 12.1.1 线程基础



![image-20230830161409943](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301614093.png)



#### 12.1.2 线程创建

第一个属性安全属性， ==所有看到安全属性这个参数的地方，通通置空，这是一个废弃的属性==

第二个参数栈空间大小，永远是按1M对齐，如果小于1M，按1M处理，向上补齐

第三个参数写线程处理函数的名字，xxx处理函数这个函数由程序员定义，但是程序员不调用，由系统调用，==回调范畴==

第四个参数是传递给线程处理函数的参数，因为是void* 填啥都行，填啥收到啥

第五个参数是线程的创建方式，==线程的创建方式只有两种，一种是立即执行模式，一种是挂起模式（休眠方式）==

第六个参数是接受操作系统返回的线程id

返回值是线程句柄

线程id和线程句柄都能代表一个线程

![image-20230830162953925](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301629083.png)

线程处理函数中的参数，是在创建线程的时候传递过来的



#### 12.1.3 线程挂起/销毁

CREATE_SUSPENDED参数设置创建后的状态

HANDLE hThread2 = CreateThread(NULL, 0, TestProc2, pszText2, CREATE_SUSPENDED, &nID); // 创建后被挂起

![image-20230830165827667](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301658873.png)



![image-20230830172141594](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301721878.png)

#### 12.1.4 线程相关操作



![image-20230830172228130](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301722432.png)

==可等候的句柄必须具备有信号和无信号的两种状态，目前只有线程句柄，后面还有互斥句柄，事件句柄，信号量句柄==

第二个参数是最大等候时间，时间到了就直接走了不等了，填INFINITE表示永远不会到时间，只能有信号才行



* 等候多个

  ==当线程处于执行信号的时候，线程句柄没有信号，当线程结束的时候，线程句柄变为有信号==

  
  
  ![image-20230830173118077](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308301731439.png)
  
  
  
  * 1
  
  
  
  
  
  
  
### 12.2 线程同步

  有很多种对临界资源加锁的机制，四种加锁机制每一个使用场景不同

  #### 12.2.1 原子锁

  ==原子锁是所有加锁机制中最难用的一个，因为要记大量的函数，每一个操作符都有一个原子锁函数，然后原子锁局限性很大，他只能对运算符加锁==

  ==唯一优点是效率高，所有的加锁机制中，原子锁是效率最高的==

  ![image-20230831084916668](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308310849152.png)

  原子锁是对数据加锁，而不是对cpu加锁

  原子锁是牺牲了效率，换来的正确率

  其实结合汇编很好理解，这里的写需要保户

  ![image-20230831094113601](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308310941719.png)



#### 12.2.2 互斥

==互斥也是一种加锁技术==

windows的互斥和linux的互斥锁解决的问题是一样的，但是实现的方式有差异

==原子锁能解决的问题，互斥也能解决，但是互斥能解决的问题，原子锁不一定能解决，互斥的试用范围更大一些==

原子锁主要解决的是操作符的操作

互斥虽然功能强大，但是没有原子锁效率高



互斥的特性：

**==1. 在任何时间点上，只能有一个线程拥有某个互斥，其他线程只能等，互斥具有独占性和排他性==**

**==2. 当任何线程都不拥有这个互斥句柄的时候，互斥句柄有信号，一旦有线程拥有他的时候，互斥句柄无信号==**

![image-20230831103755044](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311037381.png)

![image-20230831110818516](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311108720.png)

创建互斥的参数

```shell
第一个参数，安全属性已废弃

第二个参数，如果为TRUE，则哪个线程创建互斥，哪个线程就拥有互斥。如果为FALSE，则创建互斥的线程，并不拥有互斥

第三个参数，可以给互斥起名字，不愿意起可以置空
```

等候互斥跟过去排队买火车票很像

#### 12.2.3 事件

==原子锁和互斥都是加锁机制，多个线程之间有排斥的关系，抢占资源==

==事件和信号量实现的是线程之间的协调工作关系，不是排斥关系==

事件句柄也具备有信号和无信号两种状态，也是可等候句柄，==事件句柄程序员可以自己控制什么时候有信号==

![image-20230831144155102](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311441455.png)

![image-20230831155555950](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311555239.png)

事件死锁

解决方案： 与技术无关，靠个人头脑清醒，因为这个是人为代码造成得



![image-20230831164132863](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311641026.png)



#### 12.2.4 信号量

==信号量得作用跟事件类似，都是协调多个线程得工作，但是原理完全不同==

信号量句柄也是可等候句柄

==信号量句柄的计数值不为0的时候，信号量句柄有信号，递减到0的时候，信号量句柄无信号==

![image-20230831165027670](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311650069.png)

![image-20230831165616020](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308311656383.png)

# 13. VC驿站多线程篇

[教程地址](https://www.bilibili.com/video/BV1ft411B7SY/?spm_id_from=333.999.0.0&vd_source=c6ca89f75d00cd4da634736edfcca1ae)

### 13.1 认识与创建线程

#### 13.1.1 使用Spy++工具查看系统中运行的进程与线程

![image-20230904101203400](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309041012466.png)

![image-20230904101328627](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309041013689.png)

展开一个进程可以看见他里面的线程

![image-20230904101430012](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309041014057.png)

有的线程里面还会创建窗口

==线程依附于进程中的环境执行，创建窗口的工作是在线程中执行的，不可能是在进程中创建窗口==

#### 13.1.2 创建线程的三种方式

1. ##### **CreateThread**

   ==标准WIN32函数==

   **需要手动关闭线程句柄**

   [CreateThread 函数 (processthreadsapi.h) - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread)

   ```cpp
   // 线程函数
   DWORD WINAPI ThreadProc(
   	_In_ LPVOID lpParameter
   )
   {
   	int tipMsg = (int)lpParameter;
   	CString strMsg;
   	strMsg.Format(TEXT("%d"), tipMsg);
   	AfxMessageBox(strMsg);
   	return 0;
   }
   
   void CVCThread01Dlg::OnBnClickedBtn()
   {
   	// TODO: Add your control notification handler code here
   
   	// 方式1： 调用CreateThread
   	DWORD dwThreadId = 0;
   	HANDLE tThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)456, 0, &dwThreadId);
   
   	// 引用计数-1，释放线程资源
   	CloseHandle(tThread);
   }
   ```

   

2. ##### **AfxBeginThread**

   ==AfxBeginThread中所需的回调函数跟CreateThread的函数签名不同，调用约定也不同==

   ==MFC中==

   **会自动释放，无需手动管理**

   AfxBeginThread返回值是CWinThread的类型

   [CWinThread 类 | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/mfc/reference/cwinthread-class?view=msvc-170)

   ==AfxBeginThread有两种方式，一种是工作线程，一种是界面线程==

   不建议在工作线程上操作界面和按钮,工作线程和界面线程的上下文不一样

   [应用程序信息和管理 | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/mfc/reference/application-information-and-management?view=msvc-170#afxbeginthread)

   两个创建工作线程的函数

   ```cpp
   // 一般情况下，后四个参数有默认值，不用写
   CWinThread* AfxBeginThread(
       AFX_THREADPROC pfnThreadProc,
       LPVOID pParam,
       int nPriority = THREAD_PRIORITY_NORMAL,
       UINT nStackSize = 0,
       DWORD dwCreateFlags = 0,
       LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
   
   CWinThread* AfxBeginThread(
       CRuntimeClass* pThreadClass,
       int nPriority = THREAD_PRIORITY_NORMAL,
       UINT nStackSize = 0,
       DWORD dwCreateFlags = 0,
       LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
   ```

   ![image-20230904141615911](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309041416230.png)

   因为返回的CWinThread类型的指针中，有默认自动回收的属性

   ```cpp
   
   UINT __cdecl MyControllingFunction(LPVOID pParam)
   {
   	MessageBox(NULL, TEXT("AfxBeginThread创建的线程"), TEXT("Tip"), MB_OK);
   	return 0;
   }
   void CVCThread01Dlg::OnBnClickedButton1()
   {
   	// TODO: Add your control notification handler code here
   
   	CWinThread* pThread = AfxBeginThread(MyControllingFunction, (LPVOID)456);
   }
   ```

   

3. ##### **_beginthreadex**

   ==C和C++中的==

   **需要手动关闭线程句柄**

   [_beginthread、_beginthreadex | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/beginthread-beginthreadex?view=msvc-170)

==方法二和方法三都是间接的调用方式一==

### 13.2 一般情况下用什么

1. WIN32 SDK 一般使用CreateThread

2. 基于MFC的推荐使用AfxBeginThread，因为有默认值，提供基本两个参数就可以，而且不用回收资源

AfxBeginThread不仅可以创建工作线程，也可以创建界面线程

3. 第三种用的不多，是微软封装的在c和c++的，记得关闭返回值线程句柄

### 13.3 线程的运行状态

#### 13.3.1 微软开发工具集合

[Sysinternals 实用工具 - Sysinternals | Microsoft Learn](https://learn.microsoft.com/zh-cn/sysinternals/downloads/)

开发工具包里包含很多小工具，包括TCP View 查看哪些程序开了哪些端口

Autoruns查看系统中的启动项

#### 13.3.2 DebugView查看调试信息

DebugView可以单独下载，也可以下载整个开发工具集

==DebugView可以用于Win32程序的调试，也可用于驱动程序的调试，驱动程序的调试类似dll,不能下断点也不是很方便，==

捕获Win32 勾上

Capture Kernel 是指捕获驱动类程序的输出

![image-20230912155153936](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309121551018.png)

配合OutputDebugString进行输出调试

```cpp
// 线程回调
UINT __cdecl ThreadProc2(LPVOID pParam)
{
	int tipMsg = (int)pParam;
	CString strTipMsg;
	while (TRUE)
	{
		strTipMsg.Format(TEXT("%d"), tipMsg++);
		OutputDebugString(strTipMsg);
		Sleep(50);

	}
	return 0;
}
```

#### 13.3.3 分线程如何调用主线程的变量和函数等

==不能把线程函数作为MFC中主对话框类的普通成员函数==

[成员函数做为线程函数](https://www.cctry.com/thread-19591-1-1.html)

### 																																											13.4 MFC中用户界面线程的相关操作

#### 1. 工作线程和界面线程的区别：消息循环

==默认工作线程是没有消息循环的，界面线程有消息循环==

不建议在工作线程中进行模态对话框的弹出，因为可能引发异常，界面线程有自己的消息循环，跟工作线程的上下文不一样

![image-20230913105945762](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309131059932.png)

AfxBeginThread有两种重载方式，一种是创建工作线程，一种是创建界面线程

![image-20230913105504092](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309131055228.png)

==不能在工作线程中创建与使用界面相关的代码==

#### 2. 界面操作最好在界面线程中进行处理

![image-20230913114445966](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309131144047.png)

#### 3. 界面线程的创建：

##### 3.1 从CWinThread类派生自己的子类：CUIThreadApp(自定义的名字，叫啥都行)



##### 3.2 重载InitInstance（必须重载）与ExitInstance(可选重装)函数

重装方法

![image-20230914114356840](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141143966.png)

没有重载的，下拉选择add即可

![image-20230914114510924](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141145991.png)

##### 3.3 再InitInstance函数中进行界面的创建

##### 3.4 调用AfxBeginThread函数开启界面线程: AfxBeginThread(RUNTIME_CLASS(CUIThreadApp));

![image-20230914115107342](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141151433.png)

```cpp
	// 使用界面线程
	AfxBeginThread(RUNTIME_CLASS(CUIThreadApp));
```

```cpp
// 创建模态对话框
BOOL CUIThreadApp::InitInstance()
{
	CTestDlg dlg;
	dlg.DoModal();
	// TODO:  perform and per-thread initialization here
	return FALSE; // return TRUE后不会调用析构和ExitInstance，而是跳到.Run上执行
}
```

模板创建的Dlg退出的是FALSE,MFC类向导创建的CWinThread子类默认返回TRUE，要改成FALSE，除非你想要对话框关闭后还要执行消息循环

```cpp
// 创建非模态对话框
BOOL CUIThreadApp::InitInstance()
{
	//CTestDlg dlg;
	//dlg.DoModal();
	// TODO:  perform and per-thread initialization here

	//return FALSE; // return TRUE后不会调用析构和ExitInstance，而是跳到.Run上执行

	// 创建非模态对话框
	CTestDlg* pTestDlg = new CTestDlg();
	pTestDlg->Create(IDD_DIALOG1);
	pTestDlg->ShowWindow(SW_SHOW);

	return TRUE; // 这时候要返回TRUE，因为要运行线程自带的消息循环
}
```

==返回TRUE的话，要手动在某个地方发送一个WM_QUIT消息，才会退出界面线程==

也可以返回FALSE但是手动加一个RunModalLoop

![image-20230914144804193](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141448276.png)

如在dlg的cancel按钮中使用PostQuitMessage发送

![image-20230914144633036](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141446306.png)

### 13.5 线程的相关操作

#### 13.5.1 线程的挂起与恢复：SuspendThread、ResumeThread

![image-20230914162636535](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141626619.png)

#### 13.5.2 线程的优先级：

![image-20230914163119885](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141631056.png)

```cpp

UINT __cdecl ThreadProc1(LPVOID lpParameter)
{
	CStdioFile mFile;
	mFile.Open(_T("D:\\123.txt"), CFile::modeCreate | CFile::modeReadWrite);
	int tipMsg = (int)lpParameter;
	CString strTipMsg;
	while(TRUE) {
		strTipMsg.Format(_T("%d\r"), tipMsg);
		mFile.WriteString(strTipMsg);
	}

	mFile.Close();

	return 0;
}

UINT __cdecl ThreadProc2(LPVOID lpParameter)
{
	CStdioFile mFile;
	mFile.Open(_T("D:\\456.txt"), CFile::modeCreate | CFile::modeReadWrite);
	int tipMsg = (int)lpParameter;
	CString strTipMsg;
	while(TRUE) {
		strTipMsg.Format(_T("%d\r"), tipMsg);
		mFile.WriteString(strTipMsg);
	}

	mFile.Close();

	return 0;
}

void CThreadTestDlg::OnBnClickedBtn()
{
	CWinThread *pThread = AfxBeginThread(ThreadProc1, (LPVOID)111, THREAD_PRIORITY_LOWEST);
	//SetThreadPriority(pThread->m_hThread, THREAD_PRIORITY_LOWEST);

	pThread = AfxBeginThread(ThreadProc2, (LPVOID)789, THREAD_PRIORITY_HIGHEST);
	//SetThreadPriority(pThread->m_hThread, THREAD_PRIORITY_HIGHEST);
}
```

一般情况下使用默认的Normal优先级就行

==线程与进程的优先级共同作用==

![image-20230914164754681](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141647884.png)

#### **13.5.3 线程的退出与终结：**

1、最好的方式：让线程函数主动退出，或者 return；
==可以保证线程函数里面对象的析构函数被调用，以及线程申请的相关空间被释放==；

2、线程自己主动退出，可以调用 ExitThread（MFC中使用 AfxEndThread）；
==线程函数里面对象的析构函数不会被调用，线程申请的相关空间被释放；==
所以，在C语言里面可以使用该函数退出线程，但在==C++里面不建议==，因为C++里面有类！

3、其他程序强行结束目标线程：可以调用 TerminateThread
此函数非常危险，被结束的线程不会得到任何通知，线程申请的相关空间也不会被释放！
所以，==离他远点！==

4、线程退出码的获取：GetExitCodeThread
前提：句柄有效，不被关闭！

![image-20230914172828311](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309141728484.png)

### 13.6 线程间通信

#### 13.6.1 全局变量方式或者多个线程都能看到的东西

==全局变量的声明方式==

**1.在某个cpp文件中定义一个全局变量**

在其他的cpp文件中要使用他的话，在某个.h文件中，把他声明成全局变量

如：

.h -------> extern int g_Num; // 声明

.cpp --------> int g_Num = 100; // 定义



```cpp
//.h
extern int g_Num;

//.cpp
// 全局变量方式：线程间通信
int g_Num = 100;
UINT __cdecl ThreadWriteProc(LPVOID lpParameter)
{
	while (TRUE)
	{
		++g_Num;
		Sleep(50);
	}
	return 100;

}

UINT __cdecl ThreadReadProc(LPVOID lpParameter)
{
	CString strTipMsg;
	while (TRUE)
	{
		strTipMsg.Format(TEXT("%d"), g_Num);
		OutputDebugString(strTipMsg);
		Sleep(50);
	}
	return 100;
}

void CVCThread01Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	CWinThread* pThread = AfxBeginThread(ThreadWriteProc, NULL);
	AfxBeginThread(ThreadReadProc, NULL);
}
```



**2.多个线程都能看到的东西**

如：主对话框类，他只有一个，如可以在主对话框类中声明一个变量

==然后创建线程传递参数的时候，把当前主对话框的指针传递过去==

```cpp
//.h
	// 各个线程都能看到的
	int m_Num;
//.cpp
UINT __cdecl ThreadWriteProc(LPVOID lpParameter)
{
	CVCThread01Dlg* pThis = (CVCThread01Dlg*)lpParameter;
	while (TRUE)
	{
		++(pThis->m_Num);
		Sleep(50);
	}
	return 100;

}

UINT __cdecl ThreadReadProc(LPVOID lpParameter)
{
	CVCThread01Dlg* pThis = (CVCThread01Dlg*)lpParameter;
	CString strTipMsg;
	while (TRUE)
	{
		strTipMsg.Format(TEXT("%d"), pThis->m_Num);
		OutputDebugString(strTipMsg);
		Sleep(50);
	}
	return 100;
}

void CVCThread01Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_Num = 99;
	CWinThread* pThread = AfxBeginThread(ThreadWriteProc, this);
	AfxBeginThread(ThreadReadProc, this);
}
```



#### 13.6.2 发送消息的方式：PostThreadMessage

```cpp
// 发送消息的方式
#define MY_THREAD_MSG (WM_USER+100)
UINT __cdecl ThreadWriteProc(LPVOID lpParameter)
{
	int nCount = 0;
	DWORD dwThreadReadID = (DWORD)lpParameter;
	while (TRUE)
	{
		PostThreadMessage(dwThreadReadID, MY_THREAD_MSG, nCount++, NULL);
		Sleep(50);

	}
	return 0;
}

UINT __cdecl ThreadReadProc(LPVOID lpParameter)
{
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0))
	{
		switch (msg.message)
		{
		case MY_THREAD_MSG:
		{
			int nCount = (int)msg.wParam;
			CString strText;
			strText.Format(_T("%d"), nCount);
			OutputDebugString(strText);
		}
		break;
		default:
			break;
		}
	}
	return 0;
}

void CVCThread01Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	CWinThread* pThreadRead = AfxBeginThread(ThreadReadProc, NULL);
	CWinThread* pThreadWrite = AfxBeginThread(ThreadWriteProc, (LPVOID)pThreadRead->m_nThreadID);
}
```

#### 13.6.3 与界面线程的联系

**1.创建界面线程的返回值CWinThread类型指针，就是新线程的指针**

![image-20230918113240811](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181132936.png)

**2.在新界面线程中调用AfxGetApp()；获取到的是程序主线程的指针**

![image-20230918113459947](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181135027.png)

==通过以上两种方法中的任意一种，将指针进行强转类型转换之后，可以轻松的实现线程间的通信==





![image-20230918113615374](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181136437.png)

![image-20230918113747940](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181137392.png)

### 13.7 线程同步

#### 13.7.1 原子互锁家族

注意参数是long*类型的指针

==1. InterlockedIncrement: 加1操作==

==2. InterlockedDecrement: 减1操作==

==3. InterlockedExchangeAdd: 加上“指定”的值，可以加上一个负数==

==4. InterlockedExchange、InterlockedExchangePointer： 能够以原子操作的方式用第二个参数的值来取代第一个参数的值==



#### 13.7.2 Critical Sections(关键代码段、关键区域、临界区域)

**使用方法：**

==这几个函数需要一个共同的类型，这是一个结构体，一般情况下要声明为全局类型的变量==

![image-20230918162547450](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181625533.png)

1. 初始化： InitializeCriticalSection

2. 删除：DeleteCriticalSection

3. 进入：EnterCriticalSection(可能造成阻塞)

4. 尝试进入：TryEnterCriticalSection(不会造成阻塞)

   他有返回值，反或者为TRUE表面当前线程进去了，返回值是FALSE表面当前线程没进去，他不会傻等

5. 离开：LeaveCriticalSection



==优点和缺点==

![image-20230918160835807](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309181608904.png)

```cpp
CStringArray g_ArrString;
CRITICAL_SECTION g_CS;

UINT __cdecl ThreadProcCS(LPVOID lpParameter)
{
	int startIdx = (int)lpParameter;
	for (int idx = startIdx; idx < startIdx + 100; ++idx)
	{
		CString str;
		str.Format(_T("%d"), idx);

		//进入临界区
		EnterCriticalSection(&g_CS);
		g_ArrString.Add(str);
		// 离开临界区
		LeaveCriticalSection(&g_CS);
	}
	return 0;
}

//
void CVCThreadSyncDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	// 创建线程之前进行初始化临界区
	InitializeCriticalSection(&g_CS);
	for (int idx = 1; idx <=50; ++idx)
	{
		AfxBeginThread(ThreadProcCS, (LPVOID)(idx * 10));
	}

}


// 临界区查看结果
void CVCThreadSyncDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString strCount;
	INT_PTR nCount = g_ArrString.GetCount();
	strCount.Format(TEXT("%d"), nCount);
	MessageBox(strCount);

	for (INT_PTR idx = 0; idx < nCount; ++idx)
	{
		//OutputDebugString(g_ArrString.GetAt(nCount)); //win10 vs2019这里会异常
		nCount = nCount; //自己随便加的
	}
	DeleteCriticalSection(&g_CS);
}

```

#### 13.7.3 复杂的线程同步后三种用得更多

#### 13.7.4 Mutex(互斥器)

使用方法：

1. 创建一个互斥器： CreateMutex
2. 打开一个已经存在的互斥器： OpenMutex
3. 获得互斥器的拥有权: WaitForSingleObject、WaitForMultipleObjects等一类等待的函数......(可能造成阻塞)
4. 释放互斥器的拥有权： ReleaseMutex
5. 关闭互斥器：CloseHandle

# 14. 线程和窗口的关系

![image-20230823100657414](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231006591.png)













