#pragma once
#include <afxdockablepane.h>
#include "CMySideDlg.h"

//���������
class CMyDlg :
	public CDockablePane
{
public:
	CMyDlg();
	~CMyDlg();
	CMySideDlg sidedlg;//������ĶԻ���
};

