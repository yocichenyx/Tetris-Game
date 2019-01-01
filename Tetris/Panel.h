#pragma once
#include "Assembly.h"
#include <vector>
#include <time.h>
using namespace std;
//����ࣺ����ģʽ
class Panel
{
public:
	void drawSelf(CDC *pDC);//�������
	static Panel* getInstance();
	void addAssembly(Assembly a);
	//��������
	CPoint p0;//���Ͻǵ� p0.x ��� p1.x �ҽ�
	CPoint p1;//���½ǵ� p0.y �½� p1.y �Ͻ�
	int rect_size;//С�����С/�߳�
	int topp[10 + 2];//��������
	vector<Assembly> assemblies;//������Ͽ������
	CPoint rotatelocation[4+6];//�洢Ԥ��ת��Ľ���Assembly��Square��λ��

 private:
	static Panel* panel;//����
	Panel();
	~Panel();
public:
	// �÷�
	int mark;
	// ��������
	int drawGrid(CDC *pDC);
	// ���Ƶ÷ְ�
	int drawMarkBoard(CDC *pDC);
	// ��ʼʱ��
	clock_t start;
	//����֮��
	clock_t finish;
	// ����������
	int clearedLine;
};

