#pragma once
#include <atlwin.h>
#include <atlstr.h>
#include <atlfile.h>
#include <vector>

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
		switch (wParam)
		{
		case IDC_BUTTON_EXIT:
			EndDialog(NULL);
			return 0;
		case IDC_BUTTON_APPLY:
			Find(TEXT("D:"));
			//DrawFileView();
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
			LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS,
			WS_EX_CLIENTEDGE);
		myListView.InsertColumn(0, TEXT("Название"), LVCFMT_LEFT, 250);
		myListView.InsertColumn(1, TEXT("Расширение"), LVCFMT_LEFT,150);
		myListView.InsertColumn(2, TEXT("Полный путь"), LVCFMT_LEFT,190);
		return 0;
	}

	LRESULT OnCloseCmd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		EndDialog(NULL);
		return 0;
	}

	void Find(CString szPath)
	{
		CFindFile F;
		CString S = szPath + "\\*.*";
		BOOL    bFlag = F.FindFile(S);
		while (bFlag)
		{
			bFlag = F.FindNextFile();
			if (F.IsDirectory() == TRUE && F.IsDots() == FALSE)
			{
				/*ArrayFileNames.push_back(F.GetFileName());
				ArrayFilePath.push_back(F.GetFilePath());*/
				myListView.InsertItem(0, F.GetFilePath());
				Find(F.GetFilePath());
			}
		}
		F.Close();
	}

	/*void DrawFileView()
	{
		for (int i = 0; i < ArrayFileNames.size(); i++)
		{
			myListView.InsertItem(0, ArrayFileNames[i]);
			//myListView.InsertItem(3, ArrayFilePath[i]);
		}
	}*/


private:
	CString FileName;
	CString FileExtention;
	LVCOLUMNW Columns;
	CListViewCtrl myListView;
	std::vector<CString> ArrayFileNames;
	std::vector<CString> ArrayFilePath;
};