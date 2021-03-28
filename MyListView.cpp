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
				View_List(F.GetFileName(), i,F.GetFilePath());
				i++;
				FindFile(F.GetFilePath());
			}
			else
			{
				View_List(F.GetFileName(), i,F.GetFilePath());
				i++;
			}

		} while (F.FindNextFileW());
		F.Close();
		InitListViewImage(i, S);
	}
}

void MyListView::View_List(CString buf, int i,CString path)
{
	CString buf1 = buf;
	buf1.Delete(buf1.ReverseFind((wchar_t)TEXT(".")),buf1.Find(TEXT("."),0 ));
	buf.Delete(buf.FindOneOf(TEXT(".")),buf.GetLength());
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.iItem = i;
	lvItem.iImage = i + 2;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPWSTR)buf.GetString();
	lvItem.cchTextMax = wcslen(buf);
	myListView.InsertItem(&lvItem);
	myListView.SetItemText(i, 1, buf1.GetString());
	myListView.SetItemText(i, 2, path.GetString());
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
				SHGetFileInfo(F.GetFilePath(), FILE_ATTRIBUTE_DIRECTORY, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);
				ImageList_AddIcon(hSmall, lp.hIcon);
				DestroyIcon(lp.hIcon);
			}
			path.Delete(path.GetLength() - 3, 3);
			DWORD num = GetFileAttributes(F.GetFilePath());
			SHGetFileInfoW(F.GetFilePath(), num, &lp, sizeof(lp), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
			ImageList_AddIcon(hSmall, lp.hIcon);
			DestroyIcon(lp.hIcon);
		} while (F.FindNextFileW());
		F.Close();
	}
	myListView.SetImageList(hSmall, LVSIL_SMALL);
	return TRUE;
}
