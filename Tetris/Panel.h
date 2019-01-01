#pragma once
#include "Assembly.h"
#include <vector>
#include <time.h>
using namespace std;
//面板类：单件模式
class Panel
{
public:
	void drawSelf(CDC *pDC);//绘制面板
	static Panel* getInstance();
	void addAssembly(Assembly a);
	//其他函数
	CPoint p0;//左上角点 p0.x 左界 p1.x 右界
	CPoint p1;//右下角点 p0.y 下界 p1.y 上界
	int rect_size;//小方格大小/边长
	int topp[10 + 2];//顶端数组
	vector<Assembly> assemblies;//掌握组合块的引用
	CPoint rotatelocation[4+6];//存储预旋转后的焦点Assembly各Square的位置

 private:
	static Panel* panel;//单例
	Panel();
	~Panel();
public:
	// 得分
	int mark;
	// 绘制网格
	int drawGrid(CDC *pDC);
	// 绘制得分板
	int drawMarkBoard(CDC *pDC);
	// 开始时间
	clock_t start;
	//结束之间
	clock_t finish;
	// 消除的行数
	int clearedLine;
};

