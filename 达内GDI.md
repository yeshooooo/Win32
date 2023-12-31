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



# 4. 文本绘制

==能绘制字符串的函数并不止TextOut一个，还有很多函数可以绘制字符串==

TextOut是功能最弱的一个(在固定坐标绘制，只能单行绘制)，DrawText功能要强的很多（在矩形范围内绘制，可以多行绘制）

![image-20230912002515797](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309120025998.png)

==画任何东西，都应该在绘图消息里画==



### 4.2 更改文字颜色和背景

![image-20230912004726050](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309120047206.png)

```cpp
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);

	//设置颜色和背景
	SetTextColor(hdc, RGB(255, 0, 255));
	SetBkColor(hdc, RGB(0, 255, 0));
	// 通过设置背景模式设置背景色透明的，SetBkColor在TRANSPARENT模式下失效，只适用于OPAQUE不透明模式
	SetBkMode(hdc, TRANSPARENT); //颜色跟背景一体
```

### 4.3 设置字体

==字体是GDI绘图对象的一种，和画笔画刷一个道理==

![image-20230912013255987](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309120132062.png)

TrueType格式的字体文件：系统中Fonts目录下的字体文件才可以创建，这些字体文件内部是TrueType文件，里面保存着每个字的点阵字型，就是每一个字的外观形状都在这个字体文件中保存着，打开后看到的仅仅是预览，不是真实内容

==字体名不是字体的文件名，双击打开字体文件后，看他第一行==

### 4.4 字体使用

CreateFont中使用的字体，系统中必须要有，他会去系统内部寻找这个字体文件

倾斜角度以0.1度为单位

旋转角度在二维中看不出效果

斜体： 1 、 0

下划线： 1、0

字符集： 定死用GB2312_CHARSET

输出精度，剪切精度，输出质量，匹配字体这四个都是废弃的，写0

![image-20230912013801668](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309120138883.png)

![image-20230912013821094](https://yeshooonotes.oss-cn-shenzhen.aliyuncs.com/notespic/202309120138217.png)

```cpp
void OnPaint(HWND hWnd) 
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);

	//设置颜色和背景
	SetTextColor(hdc, RGB(255, 0, 255));
	SetBkColor(hdc, RGB(0, 255, 0));
	// 通过设置背景模式设置背景色透明的，SetBkColor在TRANSPARENT模式下失效，只适用于OPAQUE不透明模式
	SetBkMode(hdc, TRANSPARENT); //颜色跟背景一体

	// 创建字体
	HFONT hFont = CreateFont(
		30, //高度给30
		0, // 宽度给0，会按高度匹配宽度
		45, // 倾斜角度
		0, // 选择角度，二维没有
		900, // 字体粗细
		1, // 是不是斜体
		1, //要不要下划线
		1, // 要不要删除线
		GB2312_CHARSET, // 字符集
		0, 0, 0, 0, //废弃参数
		"黑体" //字体名字，双击字体文件第一行看

	);
	HGDIOBJ nOldFont =  SelectObject(hdc, hFont); //送给画家，并接受原来的字体
    // 画东西
    // ......



	// 画完后把自己要回来
	SelectObject(hdc, nOldFont);
	DeleteObject(hFont); // 销毁字体内存
	EndPaint(hWnd, &ps);


}
```

