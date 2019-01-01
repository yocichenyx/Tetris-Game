#include "stdafx.h"
#include "Panel.h"
#include <time.h>
Panel* Panel::panel = new Panel();
Panel::Panel()
{
	p0.x = 600;
	p1.x = 1000;
	p0.y = 50;
	p1.y = 850;
	rect_size = 40;
	mark = 0;
	//初始化top[]
	for (int i = 0; i < 12; i++)
	{
		topp[i] = p1.y;//最高值
	}
	start = clock();
	clearedLine = 0;
}

Panel::~Panel()
{
}

void Panel::drawSelf(CDC *pDC)
{
	//画边界框
	pDC->Rectangle(p0.x, p0.y, p1.x, p1.y);
	//填充动画区域
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	CRect rect(p0.x, p0.y, p1.x, p1.y);
	pDC->FillRect(&rect, &brush);
	//绘制网格
	drawGrid(pDC);
	//得分板
	drawMarkBoard(pDC);
	
}

//返回单件
Panel * Panel::getInstance()
{
	return panel;
}

void Panel::addAssembly(Assembly a)
{
	assemblies.push_back(a);
}


// 绘制网格
int Panel::drawGrid(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	CPen NewPen, *oldPen;
	NewPen.CreatePen(PS_SOLID, 2, RGB(88, 87, 86));
	oldPen = pDC->SelectObject(&NewPen);
	//画竖线
	for (int i = p0.x; i < p1.x; i += rect_size)
	{
		pDC->MoveTo(i, p0.y);
		pDC->LineTo(i, p1.y);
	}
	//画横线
	for (int j = p0.y; j < p1.y; j += rect_size)
	{
		pDC->MoveTo(p0.x, j);
		pDC->LineTo(p1.x, j);
	}
	pDC->SelectObject(oldPen);
	return 0;
}


// 绘制得分板
int Panel::drawMarkBoard(CDC *pDC)
{
	// TODO: 在此处添加实现代码.
	CRect markBoard(panel->p1.x + 10, panel->p0.y, panel->p1.x + 210, panel->p0.y + 230);
	pDC->FillSolidRect(&markBoard, RGB(100, 100, 100));

	//成绩栏
	CRect mark(panel->p1.x + 20, panel->p0.y + 20, panel->p1.x + 200, panel->p0.y + 60);
	pDC->FillSolidRect(&mark, RGB(255, 218, 185));
	pDC->TextOutW(panel->p1.x + 40, panel->p0.y + 30, L"Mark:   ");
	//用时栏
	CRect time(panel->p1.x + 20, panel->p0.y + 70, panel->p1.x + 200, panel->p0.y + 110);
	pDC->FillSolidRect(&time, RGB(255, 218, 185));
	pDC->TextOutW(panel->p1.x + 40, panel->p0.y + 80, L"Time:                        s");

	//消行数
	CRect line(panel->p1.x + 20, panel->p0.y + 120, panel->p1.x + 200, panel->p0.y + 160);
	pDC->FillSolidRect(&line, RGB(255, 218, 185));
	pDC->TextOutW(panel->p1.x + 40, panel->p0.y + 130, L"Cleared Line:");

	//等级
	CRect level(panel->p1.x + 20, panel->p0.y + 170, panel->p1.x + 200, panel->p0.y + 210);
	pDC->FillSolidRect(&level, RGB(255, 218, 185));
	pDC->TextOutW(panel->p1.x + 40, panel->p0.y + 180, L"Level:  ");
	
	//下一个和作者
	CRect author(panel->p1.x + 10, panel->p0.y + 240, panel->p1.x + 210, panel->p0.y + 580);
	pDC->FillSolidRect(&author, RGB(238, 173, 14));
	
	//button区域
	CRect button_area(panel->p1.x + 10, panel->p0.y + 590, panel->p1.x + 210, panel->p0.y + 800);
	pDC->FillSolidRect(&button_area, RGB(255, 222, 173));

	//作者学号姓名
	CFont font, *oldFont;
	VERIFY(font.CreatePointFont(150, _T("楷体"), pDC));//创建字体宋体格式 ，40为字高
	oldFont = pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);//设置字体背景为透明
	pDC->DrawText(L"\n\n  陈育鑫\n\n  yocichen \n\n  \n\n  2016240206\n\n  2016905144", &author, 0);
	pDC->SelectObject(oldFont);//Copyright ©2018 yocichen

	pDC->TextOutW((panel->p1.x + panel->p0.x) / 2, 900, L"Copyright ©2018 yocichen");

	return 0;
}
