#include "stdafx.h"
#include "Assembly.h"
#include <vector>
//#include "Panel.h"
using namespace std;
Square s1, s2, s3, s4;//ȫ�ֵ��ĸ�����

//Panel *panel1 = Panel::getInstance();//��ȡ����:ȫ��

Assembly::Assembly()
{
	location = CPoint(680, 50);//Ĭ����ʼλ��
	Color = RGB(255, 255, 0);
}

Assembly::~Assembly()
{
}

void Assembly::createSelf(int pattern)
{
	switch (pattern)
	{
	case 1://�û��Ҫ��ת
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
	case 2://L��������ת
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
	case 3://����������ת
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
	case 4://һ������:������ת
		{
			pattern = 4;
			s1.center = CPoint(location.x + 60, location.y + 20);
			squares.push_back(s1);
		}
		break;
	case 5://һ��������ת
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
	case 6://Z :������ת�� N
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
	//�㲥����Ҷ������Լ�
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].drawSelf(pDC);
	}
}

void Assembly::changeColor(CDC * pDC, COLORREF color)
{	
	//�㲥����Ҷ��ı���ɫ
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].changeColor(pDC, color);
	}
}

void Assembly::wipeSelf(CDC * pDC)
{
	//�㲥����Ҷ������Լ�
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].wipeSelf(pDC);
	}
}

void Assembly::moveSelf(CDC * pDC, int x, int y)
{
	//�㲥����Ҷ��ƶ��Լ�
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i].moveSelf(pDC, x, y);
	}
	//����λ�ã���ת��
	this->location.x += x;
	this->location.y += y;
}

void Assembly::addSquare(Square s)
{
	squares.push_back(s);
}

void Assembly::delSquare(int y1, int y2)
{
	for (vector<Square>::iterator iter = squares.begin(); iter != squares.end(); iter++)//ʹ�õ���������
	{
		if ((*iter).center.y > y1 && (*iter).center.y < y2)//ɾ�������ڵķ������
		{
			squares.erase(iter);
		}
	}
}

