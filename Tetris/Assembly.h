#pragma once
#include "Square.h"
#include <vector>
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	//����
	void createSelf(int pattern);//����������������ķ��飬ȷ������λ�ã�����addSquare()
	void drawSelf(CDC *pDC);
	void changeColor(CDC *pDC, COLORREF color);
	void wipeSelf(CDC *pDC);
	void moveSelf(CDC *pDC,int x, int y);
	void addSquare(Square s);
	void delSquare(int y1, int y2);
	//����
	vector<Square> squares;
	CPoint location;//λ�ã�����Ա���ƻ�׼
	int pattern;//1-7 ����
	COLORREF Color;//��ɫֵ
};

