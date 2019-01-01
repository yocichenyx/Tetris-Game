
// TetrisView.h: CTetrisView 类的接口
//

#pragma once
#include "Assembly.h"

class CTetrisView : public CView
{
protected: // 仅从序列化创建
	CTetrisView() noexcept;
	DECLARE_DYNCREATE(CTetrisView)

// 特性
public:
	CTetrisDoc* GetDocument() const;

// 操作
public:
	CButton start;
	CButton stop;
	CButton restart;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSTARTDown();//开始按钮按下
	afx_msg void OnSTOPDown();//暂停按钮按下
	afx_msg void OnRESTARTDown();//暂停按钮按下
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 用于检测该组合是否到达 面板 底部
	bool isTop();
	// 产生新的组合对象，加入panel中
	int CreateAssembly(CDC *pDC);
	// 更新panel的顶部值（topp[]数组值）
	int UpdateTopp(CDC *pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 判断是否到达右边界，到达return true，否则return false
	bool isRightBoundary();
	bool isLeftBoundary();
	// 判断下方是否碰撞
	bool isUnderBoundary();
	// 判断是否满行
	void filledLineClear(CDC *pDC);
	// 满行消除后，下移上方的部分
	int dropTopPart(int baseLine);
	// 直接旋转对象
	int rotatAssembly(CDC *pDC);
	// 用于播放背景音乐
	int playMusic(CString musicName);
	// 处理 上键/w键
	int funcTop();
	// 处理下键/s键
	int funcBottom();
	// 处理右键/d键
	int funcRight();
	// 处理左键/a键
	int funcLeft();
	
	// 更新得分板
	int updateMarkBoard(CDC *pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//线程函数
	static UINT gameOver(LPVOID pParam);
	// 重新开始函数
	//int OnRESTARTDown();
};

#ifndef _DEBUG  // TetrisView.cpp 中的调试版本
inline CTetrisDoc* CTetrisView::GetDocument() const
   { return reinterpret_cast<CTetrisDoc*>(m_pDocument); }
#endif

