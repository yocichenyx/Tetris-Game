// CMySideDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Tetris.h"
#include "CMySideDlg.h"
#include "afxdialogex.h"


// CMySideDlg 对话框

IMPLEMENT_DYNAMIC(CMySideDlg, CDialogEx)

CMySideDlg::CMySideDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CMySideDlg::~CMySideDlg()
{
}

void CMySideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMySideDlg, CDialogEx)
END_MESSAGE_MAP()


// CMySideDlg 消息处理程序
