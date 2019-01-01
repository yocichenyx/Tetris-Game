#pragma once
#include "Square.h"
#include <vector>
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	//函数
	void createSelf(int pattern);//根据类型生成所需的方块，确定方块位置，调用addSquare()
	void drawSelf(CDC *pDC);
	void changeColor(CDC *pDC, COLORREF color);
	void wipeSelf(CDC *pDC);
	void moveSelf(CDC *pDC,int x, int y);
	void addSquare(Square s);
	void delSquare(int y1, int y2);
	//属性
	vector<Square> squares;
	CPoint location;//位置，做成员绘制基准
	int pattern;//1-7 类型
	COLORREF Color;//颜色值
};

