#pragma once


// CMySideDlg 对话框

class CMySideDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMySideDlg)

public:
	CMySideDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMySideDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
