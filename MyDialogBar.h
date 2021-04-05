#pragma once

#include <thread>
#include <Commctrl.h>
#include "resource.h"
#include "resource2.h"
#include "Finder.h"

class MyDialogBar:public CDialogImpl<MyDialogBar>
{
public:
	enum { IDD = IDD_MYDIALOGBAR };


	BEGIN_MSG_MAP(MyDialogBar)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_COMMAND,OnCommand)
		NOTIFY_CODE_HANDLER(NM_CLICK,OnNotify)
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
			ListView_DeleteAllItems(this->myListView.m_hWnd);
			if (FindThread.joinable())
			{
				FindThread.detach();
				FindThread = std::thread((&Finder::FindFile),this->myListView, parameter);
			}
			else
			{
				FindThread = std::thread((&Finder::FindFile), this->myListView, parameter);
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
	
	LRESULT OnNotify(int, LPNMHDR pnmh, BOOL&)
	{
		MessageBoxW(TEXT("I am here"), TEXT("Here"), NULL);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		myListView.Create(m_hWnd);
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

	
private:
	CString FileName;
	CString FileExtention;
	Finder myListView;
	std::thread FindThread;
};