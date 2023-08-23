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



# 11. 线程相关

# 12. 线程和窗口的关系

![image-20230823100657414](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308231006591.png)