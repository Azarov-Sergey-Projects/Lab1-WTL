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
		CString parameter = TEXT("C:\\Users\\Огурчик\\Desktop");
		switch (wParam)
		{
		case IDC_BUTTON_EXIT:
			OnCloseCmd(uMsg,wParam,lParam,bHandled);
			return 0;
		case IDC_BUTTON_APPLY:
			FindThread = std::thread((&MyDialogBar::Find),this,parameter);
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
		myListView.InsertColumn(0, TEXT("Название"), LVCFMT_LEFT, 250);
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

	void Find(CString szPath)
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
	}
	HICON GetPathIcon(LPCWSTR sPath)
	{
		SHFILEINFO FileInfo;
		SHGetFileInfo(sPath,GetFileAttributes(sPath),
			&FileInfo, sizeof(FileInfo),SHGFI_SYSICONINDEX | SHGFI_ICON);
		return FileInfo.hIcon;
	}
private:
	CString FileName;
	CString FileExtention;
	CListViewCtrl myListView;
	std::thread FindThread;
};