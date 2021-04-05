#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
#include <atlwin.h>
#include <atlctrls.h>
#include <atlfile.h>
#include <atlstr.h>

#include <tuple>

#include "resource2.h"
class Finder: public CListViewCtrl
{
private:
	LVITEM lvItem;
	CListViewCtrl myListView; 
	CString path;
	int i;
public:
	void GetAttributes(CString path, HIMAGELIST& Icons);
	void Create(HWND m_hWnd);
	void FindFile(CString szPath);
	void View_List(CString buf, int i,CString path);
	BOOL InitListViewImage(int size, CString path);
	std::tuple<CString,CString> Split(CString buf);
	
};