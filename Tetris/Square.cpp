#include "stdafx.h"
#include "Square.h"


Square::Square()
{
	color = RGB(255, 255, 0);
}


Square::~Square()
{
}

void Square::drawSelf(CDC * pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(color);//Ìî³äÑÕÉ«
	CRect rect(center.x - 20, center.y + 20, center.x + 20, center.y - 20);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();
}

void Square::wipeSelf(CDC * pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));//Ìî³äÑÕÉ«
	CRect rect(center.x - 20, center.y + 20, center.x + 20, center.y - 20);
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();
}

void Square::changeColor(CDC * pDC, COLORREF _color)
{
	this->color = _color;
}

void Square::moveSelf(CDC * pDC,int x, int y)
{
	center.x += x;
	center.y += y;
	drawSelf(pDC);
}

