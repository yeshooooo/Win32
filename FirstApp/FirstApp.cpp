#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>


// ���尴ť�ؼ�ID��WM_CREATE��WM_COMMAND�¼�ʹ��
UINT IDC_BUTTON_1 = 200; // ����Button��ID������HMENU����
UINT IDC_BUTTON_2 = 201; // ����Button��ID������HMENU����

// ListView ID
UINT IDC_LIST_1 = 301;

// �༭��ID
UINT IDC_EDIT1 = 401;

// ����Ϊȫ�ֱ������Ա��ڰ�ť���ʱ�ܷ���
HWND hEditWnd1 = NULL;
HWND hListCtrl = NULL;
// ��������
static TCHAR szWindowClass[] = TEXT("WindowClass"); 

// ���ڱ���
static TCHAR szTitle[] = TEXT("����VC��վ�ĵ�һ�����ڳ���");

// ���岽�� ���ں����д�����Ϣ����

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = TEXT("VC��վ�ĸ�λ�����ǣ���Һð���");

	switch (uMsg)
	{
	case WM_CREATE:
		{
			// �����ӿؼ�
/*			UINT IDC_BUTTON_1 = 200; // ����Button��ID������HMENU����*/
			HWND hButtonWnd = CreateWindow(WC_BUTTON, TEXT("���ǰ�ť1"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 100,
				100, 50, // ��͸�
				hWnd, // ������
				(HMENU)IDC_BUTTON_1, // ����û�а�ť�����������������ؼ�ID�����ã�
				NULL,
				NULL);
			HWND hButtonWnd2 = CreateWindow(WC_BUTTON, TEXT("���ǰ�ť2"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				100, 200,
				100, 50, // ��͸�
				hWnd, // ������
				(HMENU)IDC_BUTTON_2, // ����û�а�ť�����������������ؼ�ID�����ã�
				NULL,
				NULL);

			// �����༭��
			hEditWnd1 = CreateWindow(WC_EDIT, TEXT("���Ǳ༭��"), WS_CHILD |  WS_VISIBLE | WS_BORDER,
				100, 40,
				100, 50, // ��͸�
				hWnd, // ������
				(HMENU)IDC_BUTTON_2, // ����û�а�ť�����������������ؼ�ID�����ã�
				NULL,
				NULL);

			// �����б�ؼ�
			hListCtrl = CreateWindow(WC_LISTVIEW, TEXT(""),
				// WS ��Window Style ,BS��Button Style
				WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
				220, 50,
				500, 200,
				hWnd, //������
				(HMENU)IDC_LIST_1,
				NULL,
				NULL
				);

			// ��������ѡ��
			ListView_SetExtendedListViewStyle(hListCtrl, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT |
				LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_AUTOCHECKSELECT | LVS_EX_AUTOSIZECOLUMNS);

// 			// Ҳ�����ڴ�����ʱ�򣬾��ڵ�һ����������Style, Ŀǰ������
// 			hListCtrl = CreateWindowEx(
// 				LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ,
// 				WC_LISTVIEW, TEXT(""),
// 
// 				WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
// 				220, 50,
// 				500, 200,
// 				hWnd, //������
// 				(HMENU)IDC_LIST_1,
// 				NULL,
// 				NULL
// 			);
			// �����У�ͨ��������Ϣ
			LVCOLUMN colInfo = { 0 };// lParam �е���Ϣ���Ǹ��ṹ��
			colInfo.mask |= LVCF_WIDTH | LVCF_TEXT | LVCF_FMT; // ���ļ�����Ա��Ч���Ա����������ã����Ⱥ��ı�
			colInfo.cx = 100; // ���
			colInfo.pszText = (LPWSTR)TEXT("��0��");
			colInfo.fmt = LVCFMT_RIGHT; // ���뷽ʽ����Ȼ��0��ʼ��������룬������ʱû��
// 			SendMessage(hListCtrl,
// 				LVM_INSERTCOLUMN, // ͨ��������Ϣ��������
// 				0, // wParam������һ��
// 				(LPARAM)&colInfo
// 				);
			// ΢��Ҳ��װ�˲���ĺ������SendMessage����
			ListView_InsertColumn(hListCtrl, 0, &colInfo);
			// �����2��
			LVCOLUMN colInfo2 = { 0 };
			colInfo2.mask |= LVCF_WIDTH | LVCF_TEXT | LVCF_FMT; 
			colInfo2.cx = 100; // ���
			colInfo2.pszText = (LPWSTR)TEXT("��1��");
			colInfo2.fmt = LVCFMT_RIGHT; 
			SendMessage(hListCtrl,
				LVM_INSERTCOLUMN, 
				1, 
				(LPARAM)&colInfo2
			);

			// ������

			// ��һ�е�0��
			LV_ITEM row_item = { 0 };
			row_item.mask |= LVIF_TEXT;
			row_item.pszText = (LPWSTR)TEXT("aaaaaa");
			// ʹ��InsertItemֻ�ܸ���һ�����ã��ڶ���������Ҫʹ��ListView_setItem����
			ListView_InsertItem(hListCtrl, &row_item); 
			// ��һ�е�1��
#if 0
			ZeroMemory(&row_item, sizeof(row_item));//���
			row_item.mask |= LVIF_TEXT;
			row_item.iItem = 0; // ���õڼ���
			row_item.iSubItem = 1; // ���õڼ���
			row_item.pszText = (LPWSTR)TEXT("bbbbb");
			ListView_SetItem(hListCtrl, &row_item);
#endif

			// Ҳ����ֱ����ListView_SetItemText����ĳ��ĳ��
			ListView_SetItemText(hListCtrl, 0, 1, (LPWSTR)TEXT("xxxx"));

		}

		break;

	// ��������ѡ��ListView��ĳһ��
	case WM_NOTIFY:
	{
		NMHDR* pNmHdr = (NMHDR*)lParam;
		// ͨ�������ID�ж��Ƿ����б�ؼ�
		if (pNmHdr->idFrom == IDC_LIST_1)
		{
			if (pNmHdr->code == NM_CLICK) // �ж��Ƿ��ǵ���¼�
			{
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
				lpnmitem->iItem;// ��ȡ�������
				lpnmitem->iSubItem; // ��ȡ�������

				TCHAR szText[128] = { 0 };
				_stprintf_s(szText, _T("������� %d �У� %d �У�"), lpnmitem->iItem, lpnmitem->iSubItem);
				MessageBox(hWnd, szText, _T("Tip"), MB_OK);
			}


		}
		// �ж��б��м�����ק����¼�
		if (pNmHdr->code == HDN_ENDTRACK)
		{
			HWND hListHeader = ListView_GetHeader(hListCtrl);
			if (hListHeader == pNmHdr->hwndFrom)
			{
				// ��ȡ��һ�п��
				int nColWidth = ListView_GetColumnWidth(hListHeader, 1);
				TCHAR szText[128] = { 0 };
				_stprintf_s(szText, _T("��һ�еĿ���� %d"), nColWidth);
				MessageBox(hWnd, szText, _T("Tip"), MB_OK);
			}
			
		}


	}
		break;
	// �������¼�
	case WM_COMMAND:
	{
		// lParam������ǿؼ��ľ��������ID�;����������һ���Ϳ���
		UINT nCtrlID = LOWORD(wParam); // ���ֽ���ID��
		UINT nCode = HIWORD(wParam); // ���ֽ���֪ͨ�룬��BN_CLICKED

		if (nCode == BN_CLICKED)
		{
			if (nCtrlID == IDC_BUTTON_1)
			{
				//MessageBox(hWnd, TEXT("��ť1�������"), TEXT("Tip"), MB_OK);
				// ʵ�ֵ����ť1,��ȡ�༭������ݣ�������ListView�����һ��
				// GetDlgItemText ��GetWindowText�����Ի�ȡ�༭������
				
				TCHAR szEditText[256] = {0};
				GetWindowText(hEditWnd1,szEditText, 256);
				// ��ȡ�б������
				int iTotalRow = ListView_GetItemCount(hListCtrl);

				LV_ITEM row_item = { 0 };
				row_item.mask |= LVIF_TEXT;
				row_item.pszText = szEditText;
				row_item.iItem = iTotalRow; // ���ò��뵽�ڼ��У�Ĭ���ǵ�0��
				int iRow = ListView_InsertItem(hListCtrl, &row_item); // 
				//SendMessage(hListCtrl, LVM_INSERTITEM,iTotalRow, (LPARAM)&row_item);

				ListView_SetItemText(hListCtrl, iRow, 1, szEditText); // ���õ�һ����Ϣ

			}
			else if (nCtrlID == IDC_BUTTON_2)
			{
				// MessageBox(hWnd, TEXT("��ť2�������"), TEXT("Tip"), MB_OK);
				// ɾ���б���ָ�����У�������к͵�����
				// ���������壬��Ϊ��һ�ε��õ�ʱ��ɾ����4��֮���5�оͱ����4����������Ҫд����4
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
	// ��һ����ע�ᴰ���࣬ ָ��������������봰�ڻص�����
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("������ע��ʧ�ܣ�"), TEXT("Tip"), NULL);
		return 1;
	}

	// �ڶ����� �������ڣ�ָ��ע�ᴰ���࣬���ڱ��⣬���ڵĴ�С
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
		MessageBox(NULL, TEXT("��������ʧ�ܣ�"), TEXT("Tip"), NULL);

	}
	// ʱ��1 �����������ﴴ���Ӵ��ڻ��߿ؼ�
	//CreateWindow �����Ӵ���

	// �������� ��ʾ����
	ShowWindow(hWnd, nCmdSHow);

	// ���Ĳ��� ��ʼ��Ϣѭ��
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}