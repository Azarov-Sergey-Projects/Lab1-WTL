#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
#include <atlwin.h>
#include <atlctrls.h>
#include <atlfile.h>
#include <atlstr.h>



class MyListView:public CListViewCtrl
{
private:
	LVITEM lvItem;
	CListViewCtrl myListView; 
	CString path;
	int i;
public:
	void Create(HWND m_hWnd);
	void FindFile(CString szPath);
	void View_List(CString buf, int i);
	BOOL InitListViewImage(int size, CString path);
};