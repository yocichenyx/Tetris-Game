#pragma once
#include <afxdockablepane.h>
#include "CMySideDlg.h"

//侧边栏的类
class CMyDlg :
	public CDockablePane
{
public:
	CMyDlg();
	~CMyDlg();
	CMySideDlg sidedlg;//侧边栏的对话框
};

