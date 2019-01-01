
// TetrisView.cpp: CTetrisView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"
#include "TetrisView.h"
#include "Panel.h"
#include "Square.h"
#include "Assembly.h"
#include <time.h>
#include <math.h>
#include <windows.h>
//#include <Digitalv.h> //MCI_DGV_PLAY_REPEAT必要头文件
#include <Vfw.h>
#pragma comment( lib, "vfw32.lib" ) //处理音频
#include  <MMsystem.h>   
#pragma comment(lib,"winmm.lib")//播放音乐
#define PI 3.141592625//用于弧度转换

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Panel *panel = Panel::getInstance();//获取单例:全局

HWND hMCI;//音乐句柄
bool gameOn = false;//游戏开始标志
bool gameStop = false;//游戏暂停标志
bool flag_top = false;//到底标志
bool flag_timer = false;//计时器是否开启
bool flag_over = false;//游戏结束标志
bool musicOn = false;
COLORREF settleColor = RGB(255, 204, 204);//落定后的颜色
COLORREF bkColor = RGB(255, 255, 255);//背景色
COLORREF black = RGB(0, 0, 0);//面板背景色

MCI_OPEN_PARMS open, open1;//打开设备
UINT wdd, wdd1;//存储播放设备id
MCI_PLAY_PARMS play, play1;//播放
// CTetrisView

IMPLEMENT_DYNCREATE(CTetrisView, CView)

BEGIN_MESSAGE_MAP(CTetrisView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTetrisView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_BUTTONSTART, OnSTARTDown)//点击start事件
	ON_BN_CLICKED(IDB_BUTTONSTOP, OnSTOPDown)//点击stop事件
	ON_BN_CLICKED(IDB_BUTTONRESTART, OnRESTARTDown)//点击restart事件
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTetrisView 构造/析构

CTetrisView::CTetrisView() noexcept
{
	// TODO: 在此处添加构造代码

}

CTetrisView::~CTetrisView()
{
}

BOOL CTetrisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTetrisView 绘图

void CTetrisView::OnDraw(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//-------------------
	//添加背景图片
	//CPaintDC dc(this);                           //定义CPaint指针
	/*CBitmap bitmap;                            //定义位图
	bitmap.LoadBitmap(IDB_BITMAP1);    //这个IDB_BITMAP1要自己添加
	CBrush brush;
	brush.CreatePatternBrush(&bitmap);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0, 0, 1890, 900);                  //前两个为图片起始位置，后两个为图片终点位置，实际为图片的像素大小
	pDC->SelectObject(pOldBrush);*/
	//------------------------------

	panel->drawSelf(pDC);//绘制面板
}

// CTetrisView 打印


void CTetrisView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTetrisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTetrisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTetrisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTetrisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTetrisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CTetrisView::OnSTARTDown()
{
	if (gameOn == false)
	{
		CDC *pDC = this->GetDC();
		//AfxBeginThread((AFX_THREADPROC)TCP_summary, (LPVOID)this); 
		AfxBeginThread((AFX_THREADPROC)gameOver, (LPVOID)this);//开启监控线程
		CString bkMusic = L"C:\\Users\\陈育鑫\\Music\\tetris.mp3";//bkmusic.wav
		playMusic(bkMusic);//播放音乐
		SetTimer(1, 500, NULL);
		gameOn = true;
	}
}

void CTetrisView::OnSTOPDown()
{
	if (gameOn == true)
	{
		KillTimer(1);
		gameOn = false;
		gameStop = true;
		//同时暂停音乐
		mciSendCommand(wdd, MCI_PAUSE, MCI_FROM, (DWORD)&play);//MCI_PAUSE指定设备暂停播放
	}
}

void CTetrisView::OnRESTARTDown()
{
	CDC *pDC = this->GetDC();
	//杀计时器
	KillTimer(1);
	flag_over == false;//结束标志置否
	//清除panel中的assemblies
	panel->assemblies.clear();
	//重绘面板
	panel->drawSelf(pDC);
	//重新开启线程
	//AfxBeginThread((AFX_THREADPROC)gameOver, (LPVOID)this);//开启监控线程
	//重新设置计时器
	SetTimer(1, 500, NULL);
}


// CTetrisView 诊断

#ifdef _DEBUG
void CTetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CTetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTetrisDoc* CTetrisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTetrisDoc)));
	return (CTetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CTetrisView 消息处理程序

//计时器
void CTetrisView::OnTimer(UINT_PTR nIDEvent)
{
	CDC *pDC = this->GetDC();//获取设备上下文对象
	updateMarkBoard(pDC);//更新得分板
	panel->drawGrid(pDC);//绘制网格1
	//如果panel中没有组合块，创建一个
	if (panel->assemblies.size() == 0 && gameOn == true)
	{
		srand((unsigned)time(NULL));
		int pattern = rand() % 6 + 1;
		Assembly m;
		m.createSelf(pattern);
		panel->addAssembly(m);
		m.drawSelf(pDC);

		panel->drawGrid(pDC);//绘制网格2
	}
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	if (isUnderBoundary()==false && gameOn == true)//未到达底部
	{
		p->drawSelf(pDC);//绘制自己
		p->wipeSelf(pDC);//擦除自己
		p->moveSelf(pDC, 0, 40);//移动自己
		panel->drawGrid(pDC);//绘制网格3
	}
	else if(gameOn == true)//到底
	{
		UpdateTopp(pDC);//更新顶部值，做结束判断
		if (flag_over == true)//结束音效处理
		{
			//结束停止背景音乐
			mciSendCommand(wdd, MCI_PAUSE, MCI_FROM, (DWORD)&play);//MCI_STOP指定设备停止播放
		
		}
		p->wipeSelf(pDC);//擦除自己
		p->changeColor(pDC, settleColor);//修改颜色
		p->drawSelf(pDC);//重绘自己
		filledLineClear(pDC);//消除满行
		CreateAssembly(pDC);//产生组合体，加入panel
		panel->drawGrid(pDC);//绘制网格4
	}
	CView::OnTimer(nIDEvent);
}

// 用于检测该组合是否到达面板底部
bool CTetrisView::isTop()
{
	// TODO: 在此处添加实现代码.
	bool flag = false;
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;
	for (int i = 0; i < p->squares.size(); i++)//普通遍历
	{
		if (p->squares[i].center.y+panel->rect_size/2 >= panel->topp[(p->squares[i].center.x -panel->p0.x)/ panel->rect_size])
		{
			flag = true;
			break;
		}
	}
	return flag;
}

// 产生新的组合对象，加入panel中
int CTetrisView::CreateAssembly(CDC *pDC)
{
	//生成随机数
	srand((unsigned)time(NULL));
	int pattern = rand() % 6 + 1;
	//创建组合
	Assembly a;//定义一个组合
	a.createSelf(pattern);
	a.drawSelf(pDC);//初始化完成
	//加入panel
	panel->addAssembly(a);//加入panel中
	return 0;
}

// 更新panel的顶部值（topp[]数组值）
int CTetrisView::UpdateTopp(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	int count;
	int begin = panel->p0.x + 20;
	int end = panel->p1.x;
	for (int i = begin; i < end; i+=panel->rect_size)
	{
		count = panel->p0.y + panel->rect_size/2;
		while (pDC->GetPixel(i, count) == RGB(0, 0, 0))
		{
			count += panel->rect_size;
		}
		//特殊处理count
		panel->topp[(i - panel->p0.x) / panel->rect_size] = count;

		if (count <= (panel->p0.y+panel->rect_size))
		{
			flag_over = true;
			break;
		}
	}
	return 0;
}

//键盘响应函数
BOOL CTetrisView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN)    //捕捉到键盘
	{
		switch (pMsg->wParam)
		{
		case VK_UP: //方向键↑
		{
			funcTop();
		}
		break;
		case VK_DOWN:  //方向键↓
		{
			funcBottom();
		}
		break;
		case VK_RIGHT:  //方向键→
		{
			funcRight();
		}
		break;
		case VK_LEFT:  //方向键←
		{
			funcLeft();
		}
		break;
		case 'W':
		{
			funcTop();
		}
		break;
		case 'A':
		{
			funcLeft();
		}
		break;
		case 'S':
		{
			funcBottom();
		}
		break;
		case 'D':
		{
			funcRight();
		}
		break;
		default:
			break;
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

// 判断是否到达右边界，到达return true，否则return false
bool CTetrisView::isRightBoundary()
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = this->GetDC();
	bool flag_rb = false;
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	for (int i = 0; i < p->squares.size(); i++)
	{
		//右边框
		if ((p->squares[i].center.x + panel->rect_size / 2) == panel->p1.x)
		{
			flag_rb = true;
			break;
		}
		//右碰撞:如果每个方块右侧颜色不等于自己颜色 且 不为黑色，说明碰到了障碍物
		if ((p->squares[i].color != pDC->GetPixel(p->squares[i].center.x + panel->rect_size, p->squares[i].center.y)) && (pDC->GetPixel(p->squares[i].center.x + panel->rect_size, p->squares[i].center.y) != RGB(0, 0, 0)))
		{
			flag_rb = true;
			break;
		}
	}
	return flag_rb;
}

//判断是否到达左边界，到达return true，否则return false
bool CTetrisView::isLeftBoundary()
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = this->GetDC();
	bool flag_lb = false;
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	for (int i = 0; i < p->squares.size(); i++)
	{
		//碰到左边框
		if ((p->squares[i].center.x - panel->rect_size / 2) == panel->p0.x)
		{
			flag_lb = true;
			break;
		}
		//左碰撞:如果存在方块左侧颜色不等于自己颜色 且 不为黑色，说明碰到了障碍物
		if ((p->squares[i].color != pDC->GetPixel(p->squares[i].center.x - panel->rect_size, p->squares[i].center.y)) && (pDC->GetPixel(p->squares[i].center.x - panel->rect_size, p->squares[i].center.y) != RGB(0, 0, 0)))
		{
			flag_lb = true;
			break;
		}
	}
	return flag_lb;
}

// 判断下方是否碰撞:各方块的下面一个方块颜色是白色或者不是本方块颜色，就到达底部
bool CTetrisView::isUnderBoundary()
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = this->GetDC();
	bool flag_ub = false;
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	for (int i = 0; i < p->squares.size(); i++)
	{
		//到达底部边框:再下一个方块颜色为 白色
		if (pDC->GetPixel(p->squares[i].center.x, p->squares[i].center.y + panel->rect_size) == RGB(255, 255, 255))
		{
			flag_ub = true;
			break;
		}
		//右碰撞:如果每个方块下侧颜色不等于自己颜色 且 不为黑色，说明碰到了障碍物
		if ((p->squares[i].color != pDC->GetPixel(p->squares[i].center.x, p->squares[i].center.y+panel->rect_size))&& (pDC->GetPixel(p->squares[i].center.x, p->squares[i].center.y + panel->rect_size)!=RGB(0, 0, 0)))
		{
			flag_ub = true;
			break;
		}
	}
	return flag_ub;
}


// 判断是否满行
void CTetrisView::filledLineClear(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	for (int i = 0; i < p->squares.size(); i++)
	{
		//遍历方块所在行
		bool flag_fill = true;
		COLORREF firstColor = pDC->GetPixel(panel->p0.x + panel->rect_size/2, p->squares[i].center.y);
		for (int j = panel->p0.x + panel->rect_size / 2; j < panel->p1.x; j += panel->rect_size)
		{
			//如果有颜色不相同的方块，结束
			if (pDC->GetPixel(j, p->squares[i].center.y) != firstColor)
			{
				flag_fill = false;
				break;
			}
		}
		//如果满行，进行消除（当前方块所在行）
		if (flag_fill == true)
		{
			MessageBeep(MB_OK);//满行消除音效
			panel->mark += 100;//加分
			panel->clearedLine += 1;
			CRect fillRect(panel->p0.x, p->squares[i].center.y - panel->rect_size / 2, panel->p1.x, p->squares[i].center.y + panel->rect_size / 2);
			//pDC->FillSolidRect(&fillRect, RGB(255, 204, 204));
			//Sleep(1000);
			//可以考虑做一下闪烁消除效果
			pDC->FillSolidRect(&fillRect, RGB(0, 0, 0));
			//然后下移上方的部分
			dropTopPart(p->squares[i].center.y);
		}
	}
}


// 满行消除后，下移上方的部分
int CTetrisView::dropTopPart(int baseline)
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = this->GetDC();
	CRect rect;
	int det = panel->rect_size / 2;
	for (int i = panel->p0.x + panel->rect_size / 2; i < panel->p1.x; i += panel->rect_size)
	{
		for (int j = baseline; j > panel->p0.y; j -= panel->rect_size)
		{
			//如果以及被涂色，那么下移
			if (pDC->GetPixel(i, j) == settleColor)
			{
				//消除原来位置
				rect = CRect(i-det, j-det, i+det, j+det);
				pDC->FillSolidRect(&rect, RGB(0, 0, 0));
				//填充现在位置
				rect = CRect(i - det, j+det, i + det, j + 3*det);
				pDC->FillSolidRect(&rect, settleColor);
			}
		}
	}
	return 0;
}

// 判断相撞/出界，否则直接旋转对象
int CTetrisView::rotatAssembly(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;//获取焦点对象
	//x0 = (x - rx0)*cos(a) - (y - ry0)*sin(a) + rx0 ;
	//y0 = (x - rx0)*sin(a) + (y - ry0)*cos(a) + ry0;
	if (p->pattern != 1 && p->pattern != 4)
	{
		int preX[10], preY[10], x, y, i;
		int rx = p->location.x;
		int ry = p->location.y;
		int count = 0;
		p->wipeSelf(pDC);
		//遍历计算
		bool flag_bouondary = false;//判断是否出界，是->不旋转
		for (i = 0; i < p->squares.size(); i++)
		{
			x = p->squares[i].center.x;
			y = p->squares[i].center.y;
			//绕location旋转
			preX[i] = (x - rx)*cos(-PI * 90 / 180) - (y - ry)*sin(-PI * 90 / 180) + rx;
			preY[i] = (x - rx)*sin(-PI * 90 / 180) + (y - ry)*cos(-PI * 90 / 180) + ry;
			//消除误差,即归位
			preX[i] = ((preX[i] - panel->p0.x) / panel->rect_size)*panel->rect_size + panel->rect_size / 2 + panel->p0.x;
			preY[i] = ((preY[i] - panel->p0.y) / panel->rect_size)*panel->rect_size + panel->rect_size / 2 + panel->p0.y;
			//赋值
			COLORREF c = pDC->GetPixel(preX[i], preY[i]);
			if ((c == black) || (c == p->Color))
			{
				count++;
			}
			if ((preY[i] <= (panel->p0.y + panel->rect_size / 2)) ||(preX[i] <= (panel->p0.x + panel->rect_size / 2)))
			{
				flag_bouondary = true;
			}
		}
		//如果可以旋转，那么赋值
		if (count == p->squares.size() && flag_bouondary == false)
		{
			for (i = 0; i < p->squares.size(); i++)
			{
				p->squares[i].center.x = preX[i];
				p->squares[i].center.y = preY[i];
			}
		}
	}
	return 0;
}

// 用于播放背景音乐
int CTetrisView::playMusic(CString musicName)
{
	// TODO: 在此处添加实现代码.
	//PlaySound((LPCTSTR)"C:\\Users\\陈育鑫\\Music\\tetris.mp3", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//MessageBeep(MB_OK);
	//HWND hMCI;定义成全局的
	//CString choose = _T("C:\\Users\\陈育鑫\\Music\\tetris.mp3"); //音乐文件的路径  
	//hMCI = MCIWndCreate(NULL, NULL, WS_POPUP | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, choose);
	//MCIWndPlay(hMCI); //播放音乐

	//打开设备
	open.lpstrDeviceType = NULL;
	open.lpstrElementName = musicName; //音乐名字;
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&open);
	
	//播放音乐
	play.dwFrom = 0;//指定开始播放的位置，单位ms
	wdd = open.wDeviceID;//wdd储存设备id
	mciSendCommand(wdd, MCI_PLAY, MCI_FROM, (DWORD)&play);

	return 0;
}


// 处理 上键/w键
int CTetrisView::funcTop()
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = this->GetDC();//获取设备上下文对象
	vector<Assembly>::iterator p = panel->assemblies.end() - 1;
	if (p->pattern != 1 && p->pattern != 4)
	{
		rotatAssembly(pDC);
	}
	return 0;
}


// 处理 下键/s键
int CTetrisView::funcBottom()
{
	// TODO: 在此处添加实现代码.
	if (isUnderBoundary() == false)//未到底
	{
		CDC *pDC = this->GetDC();//获取设备上下文对象
		vector<Assembly>::iterator p = panel->assemblies.end() - 1;
		p->wipeSelf(pDC);
		p->moveSelf(pDC, 0, panel->rect_size);
	}
	return 0;
}


// 处理 右键/d键
int CTetrisView::funcRight()
{
	// TODO: 在此处添加实现代码.
	//判断是否到右边界&右边是否有拦阻
	if (isRightBoundary() == false)
	{
		CDC *pDC = this->GetDC();//获取设备上下文对象
		vector<Assembly>::iterator p = panel->assemblies.end() - 1;
		p->wipeSelf(pDC);
		p->moveSelf(pDC, panel->rect_size, 0);
	}
	return 0;
}


// 处理 左键/a键
int CTetrisView::funcLeft()
{
	// TODO: 在此处添加实现代码.
	if (isLeftBoundary() == false)
	{
		CDC *pDC = this->GetDC();//获取设备上下文对象
		vector<Assembly>::iterator p = panel->assemblies.end() - 1;
		p->wipeSelf(pDC);
		p->moveSelf(pDC, -panel->rect_size, 0);
	}
	return 0;
}

// 更新得分板
int CTetrisView::updateMarkBoard(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	CString str;
	str.Format(L"%d", panel->mark);
	pDC->TextOutW(panel->p1.x + 130, panel->p0.y + 30, str);
	panel->finish = clock();
	str.Format(L"%d", (panel->finish-panel->start)/ CLOCKS_PER_SEC);
	pDC->TextOutW(panel->p1.x + 130, panel->p0.y + 80, str);
	str.Format(L"%d", panel->clearedLine);
	pDC->TextOutW(panel->p1.x + 160, panel->p0.y + 130, str);
	int level = (panel->finish - panel->start) / CLOCKS_PER_SEC/60;
	if (level <= 1)
	{
		pDC->TextOutW(panel->p1.x + 110, panel->p0.y + 180, L"        ");
		pDC->TextOutW(panel->p1.x + 110, panel->p0.y + 180, L"黄金菜鸟");
	}
	else
	{
		pDC->TextOutW(panel->p1.x + 110, panel->p0.y + 180, L"        ");
		pDC->TextOutW(panel->p1.x + 110, panel->p0.y + 180, L"荣耀大师");
	}
	
	return 0;
}

int CTetrisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//开始按钮
	CRect rect_button(panel->p1.x + 20, panel->p0.y + 600, panel->p1.x + 200, panel->p0.y + 690);//控制按钮大小、位置       
	start.Create(L"开始游戏", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_button, this, IDB_BUTTONSTART);
	start.ShowWindow(SW_SHOWNORMAL);

	//暂停按钮
	CRect rect_button1(panel->p1.x + 20, panel->p0.y + 700, panel->p1.x + 200, panel->p0.y + 790);//控制按钮大小、位置       
	stop.Create(L"暂停游戏", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_button1, this, IDB_BUTTONSTOP);
	stop.ShowWindow(SW_SHOWNORMAL);

	//暂停按钮：存在问题，舍弃
	/*CRect rect_button2(panel->p1.x + 20, panel->p0.y + 760, panel->p1.x + 200, panel->p0.y + 800);//控制按钮大小、位置       
	restart.Create(L"重新开始", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_button2, this, IDB_BUTTONRESTART);
	restart.ShowWindow(SW_SHOWNORMAL);*/

	return 0;
}

UINT CTetrisView::gameOver(LPVOID lparam)
{
	CTetrisView *pThis = (CTetrisView *)lparam;//新建对象，否则无法使用各种函数
	//一直检测是否结束
	CString mark, time;
	while (true)
	{
		//结束跳出循环
		if (flag_over == true)
		{
			//停止定时器
			pThis->KillTimer(1);
			flag_over == false;
			break;
		}
	}

	//标红已经下落的方块
	CDC *pDC = pThis->GetDC();
	int det = panel->rect_size / 2;
	CRect rect;
	for (int i = panel->p0.x+panel->rect_size/2; i < panel->p1.x; i += panel->rect_size)
	{
		for (int j = panel->p0.y+ panel->rect_size / 2; j < panel->p1.y; j += panel->rect_size)
		{
			if (pDC->GetPixel(i, j) != black)
			{
				rect = CRect(i-det, j-det, i+det, j+det);
				pDC->FillSolidRect(&rect, RGB(255, 0, 0));
			}
		}
	}
	panel->drawGrid(pDC);

	//播放游戏结束音乐
	//打开设备
	open1.lpstrDeviceType = NULL;
	open1.lpstrElementName = L"C:\\Users\\陈育鑫\\Music\\die.mp3"; //音乐名字;
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&open1);
	//播放音乐
	play1.dwFrom = 0;//指定开始播放的位置，单位ms
	wdd1 = open1.wDeviceID;//wdd储存设备id
	mciSendCommand(wdd1, MCI_PLAY, MCI_FROM, (DWORD)&play1);

	//获取结束分数时间
	mark.Format(L"得分：    %d  分          \n", panel->mark);
	time.Format(L"时长：    %d  秒          \n", (panel->finish - panel->start) / 1000);
	pThis->MessageBox(L"Game Over\n" + mark + time, L"Game Over", MB_OK);
	return 0;
}


