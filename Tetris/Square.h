#pragma once
class Square
{
public:
	Square();
	~Square();
	void drawSelf(CDC *pDC);//�����Լ�
	void wipeSelf(CDC *pDC);//�����Լ�
	void changeColor(CDC *pDC, COLORREF _color);
	void moveSelf(CDC *pDC, int x, int y);//�ƶ��Լ�
	CPoint center;//���ĵ㣬���ڶ�λ
	COLORREF color;//������ɫ
};

