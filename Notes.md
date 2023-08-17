# 1.MSDN 抄的一些数据类型

### 1.1 整数类型

![image-20230817153159152](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171531270.png)

### 1.2 字符串

![image-20230817153000116](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171530179.png)

### 



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