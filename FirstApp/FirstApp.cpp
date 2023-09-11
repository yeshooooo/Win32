#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>


// 定义按钮控件ID供WM_CREATE和WM_COMMAND事件使用
UINT IDC_BUTTON_1 = 200; // 定义Button的ID，传入HMENU参数
UINT IDC_BUTTON_2 = 201; // 定义Button的ID，传入HMENU参数

// ListView ID
UINT IDC_LIST_1 = 301;

// 编辑框ID
UINT IDC_EDIT1 = 401;

// 设置为全局变量，以便在按钮点击时能访问
HWND hEditWnd1 = NULL;
HWND hListCtrl = NULL;
// 窗口类名
static TCHAR szWindowClass[] = TEXT("WindowClass"); 

// 窗口标题
static TCHAR szTitle[] = TEXT("我在VC驿站的第一个窗口程序");

// 第五步： 窗口函数中处理消息窗口

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = TEXT("VC驿站的各位网友们，大家好啊！");

	switch (uMsg)
	{
	case WM_CREATE:
		{
			// 创建子控件
/*			UINT IDC_BUTTON_1 = 200; // 定义Button的ID，传入HMENU参数*/
			HWND hButtonWnd = CreateWindow(WC_BUTTON, TEXT("我是按钮1"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 100,
				100, 50, // 宽和高
				hWnd, // 父窗口
				(HMENU)IDC_BUTTON_1, // 这里没有按钮，但是他可以用作控件ID的作用，
				NULL,
				NULL);
			HWND hButtonWnd2 = CreateWindow(WC_BUTTON, TEXT("我是按钮2"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 200,
				100, 50, // 宽和高
				hWnd, // 父窗口
				(HMENU)IDC_BUTTON_2, // 这里没有按钮，但是他可以用作控件ID的作用，
				NULL,
				NULL);

			// 创建编辑框
			hEditWnd1 = CreateWindow(WC_EDIT, TEXT("我是编辑框"), WS_CHILD |  WS_VISIBLE | WS_BORDER,
				100, 40,
				100, 50, // 宽和高
				hWnd, // 父窗口
				(HMENU)IDC_BUTTON_2, // 这里没有按钮，但是他可以用作控件ID的作用，
				NULL,
				NULL);

			// 创建列表控件
			hListCtrl = CreateWindow(WC_LISTVIEW, TEXT(""),
				// WS 是Window Style ,BS是Button Style
				WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
				220, 50,
				500, 200,
				hWnd, //父窗口
				(HMENU)IDC_LIST_1,
				NULL,
				NULL
				);

			// 设置整行选择
			ListView_SetExtendedListViewStyle(hListCtrl, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT |
				LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_AUTOCHECKSELECT | LVS_EX_AUTOSIZECOLUMNS);

// 			// 也可以在创建的时候，就在第一个参数设置Style, 目前有问题
// 			hListCtrl = CreateWindowEx(
// 				LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ,
// 				WC_LISTVIEW, TEXT(""),
// 
// 				WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
// 				220, 50,
// 				500, 200,
// 				hWnd, //父窗口
// 				(HMENU)IDC_LIST_1,
// 				NULL,
// 				NULL
// 			);
			// 插入列，通过发送消息
			LVCOLUMN colInfo = { 0 };// lParam 列的信息，是个结构体
			colInfo.mask |= LVCF_WIDTH | LVCF_TEXT | LVCF_FMT; // 让哪几个成员生效，以便在下面设置，如宽度和文本
			colInfo.cx = 100; // 宽度
			colInfo.pszText = (LPWSTR)TEXT("第0列");
			colInfo.fmt = LVCFMT_RIGHT; // 对齐方式，当然第0列始终是左对齐，这里暂时没用
// 			SendMessage(hListCtrl,
// 				LVM_INSERTCOLUMN, // 通过发送消息来插入列
// 				0, // wParam插入哪一列
// 				(LPARAM)&colInfo
// 				);
			// 微软也封装了插入的宏来替代SendMessage，如
			ListView_InsertColumn(hListCtrl, 0, &colInfo);
			// 插入第2列
			LVCOLUMN colInfo2 = { 0 };
			colInfo2.mask |= LVCF_WIDTH | LVCF_TEXT | LVCF_FMT; 
			colInfo2.cx = 100; // 宽度
			colInfo2.pszText = (LPWSTR)TEXT("第1列");
			colInfo2.fmt = LVCFMT_RIGHT; 
			SendMessage(hListCtrl,
				LVM_INSERTCOLUMN, 
				1, 
				(LPARAM)&colInfo2
			);

			// 插入行

			// 第一行第0列
			LV_ITEM row_item = { 0 };
			row_item.mask |= LVIF_TEXT;
			row_item.pszText = (LPWSTR)TEXT("aaaaaa");
			// 使用InsertItem只能给第一列设置，第二列这种需要使用ListView_setItem设置
			ListView_InsertItem(hListCtrl, &row_item); 
			// 第一行第1列
#if 0
			ZeroMemory(&row_item, sizeof(row_item));//清空
			row_item.mask |= LVIF_TEXT;
			row_item.iItem = 0; // 设置第几行
			row_item.iSubItem = 1; // 设置第几列
			row_item.pszText = (LPWSTR)TEXT("bbbbb");
			ListView_SetItem(hListCtrl, &row_item);
#endif

			// 也可以直接用ListView_SetItemText设置某行某列
			ListView_SetItemText(hListCtrl, 0, 1, (LPWSTR)TEXT("xxxx"));

		}

		break;

	// 被动处理选中ListView中某一行
	case WM_NOTIFY:
	{
		NMHDR* pNmHdr = (NMHDR*)lParam;
		// 通过基类的ID判断是否是列表控件
		if (pNmHdr->idFrom == IDC_LIST_1)
		{
			if (pNmHdr->code == NM_CLICK) // 判断是否是点击事件
			{
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
				lpnmitem->iItem;// 获取点击的行
				lpnmitem->iSubItem; // 获取点击的列

				TCHAR szText[128] = { 0 };
				_stprintf_s(szText, _T("点击的是 %d 行， %d 列！"), lpnmitem->iItem, lpnmitem->iSubItem);
				MessageBox(hWnd, szText, _T("Tip"), MB_OK);
			}


		}
		// 判断列表中间框框拖拽完毕事件
		if (pNmHdr->code == HDN_ENDTRACK)
		{
			HWND hListHeader = ListView_GetHeader(hListCtrl);
			if (hListHeader == pNmHdr->hwndFrom)
			{
				// 获取第一列宽度
				int nColWidth = ListView_GetColumnWidth(hListHeader, 1);
				TCHAR szText[128] = { 0 };
				_stprintf_s(szText, _T("第一列的宽度是 %d"), nColWidth);
				MessageBox(hWnd, szText, _T("Tip"), MB_OK);
			}
			
		}


	}
		break;
	// 处理点击事件
	case WM_COMMAND:
	{
		// lParam保存的是控件的句柄，我们ID和句柄保存任意一个就可以
		UINT nCtrlID = LOWORD(wParam); // 低字节是ID号
		UINT nCode = HIWORD(wParam); // 高字节是通知码，如BN_CLICKED

		if (nCode == BN_CLICKED)
		{
			if (nCtrlID == IDC_BUTTON_1)
			{
				//MessageBox(hWnd, TEXT("按钮1被点击了"), TEXT("Tip"), MB_OK);
				// 实现点击按钮1,获取编辑框的内容，并且在ListView中添加一行
				// GetDlgItemText 和GetWindowText均可以获取编辑框内容
				
				TCHAR szEditText[256] = {0};
				GetWindowText(hEditWnd1,szEditText, 256);
				// 获取列表的行数
				int iTotalRow = ListView_GetItemCount(hListCtrl);

				LV_ITEM row_item = { 0 };
				row_item.mask |= LVIF_TEXT;
				row_item.pszText = szEditText;
				row_item.iItem = iTotalRow; // 设置插入到第几行，默认是第0行
				int iRow = ListView_InsertItem(hListCtrl, &row_item); // 
				//SendMessage(hListCtrl, LVM_INSERTITEM,iTotalRow, (LPARAM)&row_item);

				ListView_SetItemText(hListCtrl, iRow, 1, szEditText); // 设置第一列信息

			}
			else if (nCtrlID == IDC_BUTTON_2)
			{
				// MessageBox(hWnd, TEXT("按钮2被点击了"), TEXT("Tip"), MB_OK);
				// 删除列表中指定的行，如第四行和第五行
				// 这里有陷阱，因为第一次调用的时候，删掉了4，之后第5行就变成了4，所以这里要写两次4
				ListView_DeleteItem(hListCtrl, 4);
				ListView_DeleteItem(hListCtrl, 4);
			}
			
		}
	}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdSHow)
{
	// 第一步：注册窗口类， 指定窗口类的名称与窗口回调函数
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("窗口类注册失败！"), TEXT("Tip"), NULL);
		return 1;
	}

	// 第二步： 创建窗口，指定注册窗口类，窗口标题，窗口的大小
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("创建窗口失败！"), TEXT("Tip"), NULL);

	}
	// 时机1 ：可以在这里创建子窗口或者控件
	//CreateWindow 创建子窗口

	// 第三步： 显示窗口
	ShowWindow(hWnd, nCmdSHow);

	// 第四步： 开始消息循环
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}