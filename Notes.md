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

### 2.1 应用程序实例句柄

![image-20230817160146245](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171601527.png)

### 2.2 窗口显示方式

int nCmdSHow

![image-20230817163220156](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171632380.png)

### 2.3 设置入口点同时弹出控制台应用程序

![image-20230817171534044](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171715259.png)

```cpp
#pragma comment(linker,"/subsystem:\"console\"/entry:\"WinMainCRTStartup\"")
```



**也可以在配置里设置**

![image-20230817172036419](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171720469.png)

![image-20230817172055431](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202308171720504.png)