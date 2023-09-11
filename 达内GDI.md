# 1. 绘图编程

### 1.1 绘图基础

绘图设备--画家

三个步骤

HDC -- 绘图设备句柄 hdc = BeginPaint--抓绘图设备这个画家(hWnd--到这个窗口画画，。。。)

TextOut(hdc-- 告诉画家画字符串， 100, 100 --在主窗口100，100这个位置画字符串， "hello" --画字符串hello) -- 给画家发指定画字符串

...

EndPaint(参数跟BeginPaint一样)---放画家回去操作系统

![image-20230911141310588](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111413916.png)

==微软定义的颜色数据类型COLORREF==

![image-20230911142433459](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111424692.png)

### 1.2 基本图形的绘制

#### 1.2.1 画点

![image-20230911142834253](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111428445.png)

==绘制图形一定要在绘图消息里绘制==

```cpp
		//绘制图形一定要在绘图消息里绘制
	case WM_PAINT:
		OnPaint(hWnd);
		break;

//
// 画点的函数
void DrawPit(HDC hdc)
{
	//SetPixel(hdc, 100, 100, RGB(255, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			SetPixel(hdc, i, j, RGB(i, j, 0));
		}
	}
}
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 }; 
	// ps结构体里的成员是系统api内部使用的
	HDC hdc =  BeginPaint(hWnd, &ps);
	DrawPit(hdc);
	EndPaint(hWnd, &ps);


}
```

#### 1.2.2 画线

==画线需要两个函数配合使用==

MoveToEx参数指定的点为==窗口当前点==

​	**窗口当前点是一个专有名词，默认在0,0点**

LineTo: 从MoveToEx指定的点到LineTo指定的点绘制一条直线



![image-20230911144608721](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111446951.png)

#### 1.2.2 封闭图形

微软给出的定义是：==能够用画刷填充的图形==

视觉上封闭的图形，不一定是封闭图形，如自己用线画一个三角形，并不是封闭图形

Rectangle： 直角矩形

Ellipse: 圆形

# 2. GDI 绘图对象

绘图设备手里只有一根黑色的铅笔，画出来只有一个黑色的线条

GDI有很多绘图对象，如画笔，画刷，字体，调色板等

### 2.1 画笔

线型： 虚线，实线等

![image-20230911151847527](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111518863.png)

![image-20230911152514443](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111525803.png)



### 2.2 画刷



![image-20230911153739507](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111537774.png)

### 2.3 透明画刷

NULL_BRUSH就是借了一个透明画刷

![image-20230911155238572](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111552798.png)

# 3.GDI绘图对象----位图

bmp后缀的图片

### 3.1 位图绘制

==位图的使用是兼顾资源和GDI绘图对象==

![image-20230911155838670](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111558022.png)

![image-20230911163700851](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111637085.png)

#### 1. 添加位图资源

![image-20230911160558760](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111605904.png)

然后往rc文件中添加bitmap

![image-20230911160644571](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111606730.png)

右键空白区域可以看的图片的大小，一会会用到

![image-20230911160953895](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111609037.png)

硬盘上，保存着猪八戒头像每一个像素点的颜色值，==会随进行启动一起被加载到进程内存中==

![image-20230911161159968](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111612024.png)

#### 2. 加载位图资源--LoadBitmap



#### 3. 创建和当前DC相匹配的内存DC

==当前DC在窗口上画画，内存DC在内存上虚拟区域画画==

CreateCompatibleDC创建虚拟区域和返回内存中的虚拟DC

和之前的不一样

![image-20230911161810770](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111618926.png)

![image-20230911163539371](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111635516.png)



#### 4. 利用SelectObject 将位图放入内存中的虚拟DC中

如将猪头送给内存dc后，他会立即在虚拟区域中画出来（眼睛还看不到）

#### 5. 成像 

##### 1：1 成像

==把虚拟区域中画好的在窗口中成像==

![image-20230911163858337](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111638528.png)

如果不做二维图像转换开发的画，成像方法一般写SRCCOPY(原样成像，在成像过程中不更改点的颜色)就够了



##### 缩放成像

![image-20230911171537834](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309111715209.png)

#### 6. 把位图数据要回来(之前给谁了问谁要)

在第四步中给了内存中虚拟DC

SelectObject

#### 7. 释放位图

DeleteObject

#### 8. 释放匹配的DC

就是释放内存中的虚拟DC

DeleteDC

##### 9.源码

==8个步骤缺一不可==

```cpp


// 绘制位图
void DrawBmp(HDC hdc)
{
	// 1. 添加位图资源(猪头)

	// 2. 加载位图(到本进程的内存资源中去找
	// 不得已设置全局变量
	HBITMAP hBmp =  LoadBitmap(g_hInstance, (LPCTSTR)IDB_BITMAP1);
	// 3. 创建和当前DC匹配的虚拟内存DC,并且构建一个虚拟区域，并且内存DC在虚拟区域中绘图
	HDC hMemdc = CreateCompatibleDC(hdc);
	// 4. SelectObject 将位图数据送给内存DC
	// 内存DC在虚拟区域中将位图绘制出来
	// 返回的是老的位图，这里是假的，因为原来没有，为了后面将真的换回来
	HGDIOBJ nOldBmp =  SelectObject(hMemdc, hBmp);

	// 5. 成像
	// 1:1 成像
	BitBlt(
		hdc, // 目的dc
		100, 100, // 目的左上角
		48, 48, // 目的大小
		hMemdc, // 源DC
		0, 0, // 从源的什么位置开始拷贝
		SRCCOPY // 成像方式，原样成像
	);

	// 缩放成像
	StretchBlt(
		hdc, // 目的dc
		200, 200, // 窗口成像位置
		24, 24, // 成像区域大小，也能放大成像，改变成像目标区域大小即可
		hMemdc, // 源dc
		0, 0, //从源的什么位置开始拷贝
		48, 48, // 拷贝多少成像
		SRCCOPY // 成像方式，原样成像
	);


	// 6. 把位图图像要回来
	SelectObject(hMemdc, nOldBmp);
	// 7-8 释放内存
	DeleteObject(hBmp);
	DeleteDC(hMemdc);

}



```



