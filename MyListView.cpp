#include "MyListView.h"

void MyListView::Create(HWND m_hWnd)
{
	CRect myRect{ 0,0,600,400 };
	myListView.Create(m_hWnd, myRect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS);
	myListView.InsertColumn(0, TEXT("Название"), LVCFMT_LEFT, 290);
	myListView.InsertColumn(1, TEXT(".*"), LVCFMT_LEFT, 50);
	myListView.InsertColumn(2, TEXT("Полный путь"), LVCFMT_LEFT, 290);
}

void MyListView::FindFile(CString szPath)
{
	CFindFile F;
	CString S = szPath + TEXT("\\*.*");
	i = 0;
	BOOL bFlag = F.FindFile(S);
	if (!bFlag)
	{
		MessageBox(TEXT("Error"), TEXT("File not found"), 0);
	}
	else
	{
		do
		{
			if (F.IsDirectory() == TRUE && F.IsDots() == FALSE)
			{
				View_List(F.GetFileName(), i);
				i++;
				FindFile(F.GetFilePath());
			}
			else
			{
				View_List(F.GetFileName(), i);
				i++;
			}

		} while (F.FindNextFileW());
		F.Close();
		InitListViewImage(i, S);
	}
}

void MyListView::View_List(CString buf, int i)
{
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.iItem = i;
	lvItem.iImage = i + 2;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPWSTR)buf.GetString();
	lvItem.cchTextMax = wcslen(buf);
	myListView.InsertItem(&lvItem);
}


BOOL MyListView::InitListViewImage(int size, CString path)
{
	CFindFile F;
	HIMAGELIST hSmall;
	SHFILEINFO lp;
	hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK | ILC_COLOR32, size, 1);
	bool hFind = F.FindFile(path);
	if (!hFind)
	{
		MessageBox(TEXT("Error"), TEXT("File not found"), MB_OK | MB_ICONWARNING);
	}
	else
	{
		do
		{
			if (wcscmp(F.GetFileName(), TEXT(".")) == 0)
			{
				SHGetFileInfo(TEXT(""), FILE_ATTRIBUTE_DEVICE, &lp, sizeof(&lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);
				ImageList_AddIcon(hSmall, lp.hIcon);
				DestroyIcon(lp.hIcon);
			}
			if (wcscmp(F.GetFileName(), TEXT("..")) == 0)
			{
				SHGetFileInfo(TEXT(""), FILE_ATTRIBUTE_DIRECTORY, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);
				ImageList_AddIcon(hSmall, lp.hIcon);
				DestroyIcon(lp.hIcon);
			}
			path.Delete(path.GetLength() - 3, 3);
			CString FileInfo = path + F.GetFileName();
			wchar_t buf1[MAX_PATH] = L"C:\\Users\\Огурчик\\Desktop\\";
			wcscat(buf1, F.GetFileName());

			DWORD num = GetFileAttributes(buf1);
			SHGetFileInfoW(buf1, num, &lp, sizeof(lp), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
			ImageList_AddIcon(hSmall, lp.hIcon);
			DestroyIcon(lp.hIcon);
		} while (F.FindNextFileW());
		F.Close();
	}
	myListView.SetImageList(hSmall, LVSIL_SMALL);
	return TRUE;
}
