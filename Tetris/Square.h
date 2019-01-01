#pragma once
class Square
{
public:
	Square();
	~Square();
	void drawSelf(CDC *pDC);//绘制自己
	void wipeSelf(CDC *pDC);//擦除自己
	void changeColor(CDC *pDC, COLORREF _color);
	void moveSelf(CDC *pDC, int x, int y);//移动自己
	CPoint center;//中心点，用于定位
	COLORREF color;//方块颜色
};

