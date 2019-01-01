#include "stdafx.h"
#include "Assembly.h"
#include <vector>
//#include "Panel.h"
using namespace std;
Square s1, s2, s3, s4;//全局的四个方块

//Panel *panel1 = Panel::getInstance();//获取单例:全局

Assembly::Assembly()
{
	location = CPoint(680, 50);//默认起始位置
	Color = RGB(255, 255, 0);
}

Assembly::~Assembly()
{
}

void Assembly::createSelf(int pattern)
{
	switch (pattern)
	{
	case 1://田：没必要旋转
		{
		pattern = 1;
			s1.center = CPoint(location.x + 20, location.y + 20);
			s1.color = Color;
			s2.center = CPoint(location.x + 60, location.y + 20);
			s2.color = Color;
			s3.center = CPoint(location.x + 20, location.y + 60);
			s3.color = Color;
			s4.center = CPoint(location.x + 60, location.y + 60);
			s4.color = Color;

			squares.push_back(s1);
			squares.push_back(s2);
			squares.push_back(s3);
			squares.push_back(s4);
		}
		break;
	case 2://L：可以旋转
		{
		pattern = 2;
			s1.center = CPoint(location.x + 20, location.y + 20);
			s1.color = Color;
			s2.center = CPoint(location.x + 20, location.y + 60);
			s2.color = Color;
			s3.center = CPoint(location.x + 60, location.y + 60);
			s3.color = Color;
			s4.center = CPoint(location.x + 100, location.y + 60);
			s4.color = Color;

			squares.push_back(s1);
			squares.push_back(s2);
			squares.push_back(s3);
			squares.push_back(s4);
		}
		break;
	case 3://土：可以旋转
		{
		pattern = 3;
			s1.center = CPoint(location.x + 60, location.y + 20);
			s1.color = Color;
			s2.center = CPoint(location.x + 20, location.y + 60);
			s2.color = Color;
			s3.center = CPoint(location.x + 60, location.y + 60);
			s3.color = Color;
			s4.center = CPoint(location.x + 100, location.y + 60);
			s4.color = Color;

			squares.push_back(s1);
			squares.push_back(s2);
			squares.push_back(s3);
			squares.push_back(s4);
		}
		break;
	case 4://一个方块:无需旋转
		{
			pattern = 4;
			s1.center = CPoint(location.x + 60, location.y + 20);
			squares.push_back(s1);
		}
		break;
	case 5://一：可以旋转
		{
			pattern = 5;
			s1.center = CPoint(location.x + 20, location.y + 20);
			s1.color = Color;
			s2.center = CPoint(location.x + 60, location.y + 20);
			s2.color = Color;
			s3.center = CPoint(location.x + 100, location.y + 20);
			s3.color = Color;
			s4.center = CPoint(location.x - 20, location.y + 20);
			s4.color = Color;

			squares.push_back(s1);
			squares.push_back(s2);
			squares.push_back(s3);
			squares.push_back(s4);
		}
		break;
	case 6://Z :可以旋转成 N
	{
		pattern = 6;
		s1.center = CPoint(location.x + 20, location.y + 20);
		s1.color = Color;
		s2.center = CPoint(location.x + 20, location.y + 60);
		s2.color = Color;
		s3.center = CPoint(location.x + 60, location.y + 60);
		s3.color = Color;
		s4.center = CPoint(location.x - 20, location.y + 20);
		s4.color = Color;

		squares.push_back(s1);
		squares.push_back(s2);
		squares.push_back(s3);
		squares.push_back(s4);
	}
	break;
	default:
		break;
	}
	
}

void Assembly::drawSelf(CDC * pDC)
{
	//广播：大家都绘制自己
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].drawSelf(pDC);
	}
}

void Assembly::changeColor(CDC * pDC, COLORREF color)
{	
	//广播：大家都改变颜色
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].changeColor(pDC, color);
	}
}

void Assembly::wipeSelf(CDC * pDC)
{
	//广播：大家都擦除自己
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].wipeSelf(pDC);
	}
}

void Assembly::moveSelf(CDC * pDC, int x, int y)
{
	//广播：大家都移动自己
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].moveSelf(pDC, x, y);
	}
	//更新位置，旋转用
	this->location.x += x;
	this->location.y += y;
}

void Assembly::addSquare(Square s)
{
	squares.push_back(s);
}

void Assembly::delSquare(int y1, int y2)
{
	for (vector<Square>::iterator iter = squares.begin(); iter != squares.end(); iter++)//使用迭代器遍历
	{
		if ((*iter).center.y > y1 && (*iter).center.y < y2)//删除区间内的方块对象
		{
			squares.erase(iter);
		}
	}
}

