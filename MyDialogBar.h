#pragma once
#include <atlwin.h>
#include <atlstr.h>
#include <atlfile.h>
#include <vector>
#include <thread>

#include "resource.h"
#include "resource2.h"


class MyDialogBar:public CDialogImpl<MyDialogBar>
{
public:
	enum { IDD = IDD_MYDIALOGBAR };


	BEGIN_MSG_MAP(MyDialogBar)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_COMMAND,OnCommand)
		MESSAGE_HANDLER(WM_CLOSE,OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CString parameter = TEXT("C:\\Users\\Огурчик\\Desktop\\");
		switch (wParam)
		{
		case IDC_BUTTON_EXIT:
			OnCloseCmd(uMsg,wParam,lParam,bHandled);
			return 0;
		case IDC_BUTTON_APPLY:
			if (FindThread.joinable())
			{
				FindThread.detach();
				FindThread = std::thread((&MyDialogBar::FindFile), this, parameter);
			}
			else
			{
				FindThread = std::thread((&MyDialogBar::FindFile), this, parameter);
			}
			return 0;
		case IDC_SEARCH_TEXT_BAR :
			GetDlgItemText(IDC_SEARCH_TEXT_BAR, FileName);
			return 0;
		case IDC_EXTENTION_TEXT_BAR:
			GetDlgItemText(IDC_EXTENTION_TEXT_BAR, FileExtention);
			return 0;
		}
		return 0;
	}


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect myRect{0,0,600,400 };
		CenterWindow(GetParent());
		myListView.Create(m_hWnd,myRect , NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
			LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS);
		myListView.InsertColumn(0, TEXT("Название"), LVCFMT_LEFT, 600);
		myListView.InsertColumn(1, TEXT(".*"), LVCFMT_LEFT,50);
		myListView.InsertColumn(2, TEXT("Полный путь"), LVCFMT_LEFT,290);
		return 0;
	}

	LRESULT OnCloseCmd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (FindThread.joinable())
		{
			FindThread.detach();
		}
		EndDialog(NULL);
		return 0;
	}

	/*void Find(CString szPath)
	{
		CFindFile F;
		CString S = szPath+ TEXT("\\*.*");
		
		BOOL  bFlag = F.FindFile(S);
		while (bFlag)
		{
			bFlag = F.FindNextFile();
			if (F.IsDirectory() == TRUE&&F.IsDots()==FALSE)
			{
				myListView.InsertItem(0, F.GetFileName());
				myListView.SetItemText(0, 2,F.GetFilePath());
				Find(F.GetFilePath());
			}
		}
		F.Close();
	}*/
	void FindFile(CString szPath)
	{
		CFindFile F;
		CString S = szPath+ TEXT("*.*");
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
				View_List(F.GetFileName(), i);
				i++;
			} while (F.FindNextFileW());
			F.Close();
			InitListViewImage(i, S);
		}
	}
	void View_List(CString buf, int i)
	{
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
		lvItem.state = 0;
		lvItem.stateMask = 0;
		lvItem.iItem = i;
		lvItem.iImage = i+2;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)buf.GetString();
		lvItem.cchTextMax = wcslen(buf);
		myListView.InsertItem(&lvItem);
	}

	BOOL InitListViewImage(int size, CString path)
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
				if (wcscmp(F.GetFileName(), TEXT("."))==0)
				{
					SHGetFileInfo(TEXT(""), FILE_ATTRIBUTE_DEVICE, &lp, sizeof(&lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);
					ImageList_AddIcon(hSmall, lp.hIcon);
					DestroyIcon(lp.hIcon);
				}
				if (wcscmp(F.GetFileName(),TEXT("..")) == 0)
				{
					SHGetFileInfo(TEXT(""), FILE_ATTRIBUTE_DIRECTORY, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);
					ImageList_AddIcon(hSmall, lp.hIcon);
					DestroyIcon(lp.hIcon);
				}
				path.Delete(path.GetLength() - 3, 3);
				CString FileInfo = path+ F.GetFileName();
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
private:
	CString FileName;
	CString FileExtention;
	CListViewCtrl myListView;
	std::thread FindThread;
	int i;
};