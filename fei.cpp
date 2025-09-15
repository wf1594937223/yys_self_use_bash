// ...existing code...
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <iostream>
# include <fstream>
std::mt19937 ran(time(0));
//random time for not hacking
void slp(int ti)
{
	int sig, ran_ti;
	sig = (ran() % 2) * 2 - 1;
	ran_ti = (ran() % (ti / 100)) * sig;
	Sleep(ti + ran_ti);
	return;
}
//random pos for not hacking
POINT pos(int x, int y)
{
	int sigx, ran_tix, sigy, ran_tiy;
	sigx = (ran() % 2) * 2 - 1;
	sigy = (ran() % 2) * 2 - 1;
	ran_tix = (ran() % 5) * sigx;
	ran_tiy = (ran() % 5) * sigy;
	POINT tmp;
	tmp.x = x + ran_tix;
	tmp.y = y + ran_tiy;
	return tmp;
}
// void clk(int x, int y)
// {
// 	POINT tmp = pos(x, y);
// 	SetCursorPos(tmp.x, tmp.y);
// 	mouse_event(MOUSEEVENTF_LEFTDOWN, tmp.x, tmp.y, 0, 0); // 按下
// 	mouse_event(MOUSEEVENTF_LEFTUP, tmp.x, tmp.y, 0, 0);   // 释放
// 	return;
// }
// 平滑移动鼠标到(x, y)
void smoothMove(int x, int y) {
    POINT p;
    GetCursorPos(&p);
    int startX = p.x, startY = p.y;
    int steps = 30 + rand() % 20; // 随机步数，模拟人手
    for (int i = 1; i <= steps; ++i) {
        double t = (double)i / steps;
        // 可用贝塞尔曲线或简单线性插值
        int curX = startX + (int)((x - startX) * t);
        int curY = startY + (int)((y - startY) * t);
        SetCursorPos(curX, curY);
        Sleep(5 + rand() % 8); // 每步随机延时
    }
    SetCursorPos(x, y); // 最后确保到达目标
}

// 平滑移动并点击
void clk(int x, int y) {
    smoothMove(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(20 + rand() % 30); // 按下延时
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

POINT a[1010];
int b[1010];
int main()
{
	int i, j, n, m, k;
	// 文件名
	const char* savefile = "mouse_data.txt";
	//get the number of positions and the positions
	printf("欢迎使用本软件！每次操作后请按回车\n");
	printf("请输入你要点击的鼠标位置个数（输入0则沿用上次数据）：");
	scanf("%d", &n);
	if (n == 0) {
		std::ifstream fin(savefile);
		if (!fin) {
			printf("未找到上次保存的数据，无法沿用！\n");
			return 1;
		}
		fin >> n;
		for (i = 1; i <= n; i++) fin >> a[i].x >> a[i].y;
		fin >> m;
		for (i = 1; i <= m; i++) fin >> b[i];
		fin.close();
		printf("已从文件沿用上次的%d个坐标和%d个间隔\n", n, m);
	} else {
		printf("请依次设置这些鼠标位置：\n");
		for (i = 1; i <= n; i++)
		{
			printf("请将鼠标移动到第%d个位置，然后按任意键继续。\n", i);
			system("pause");
			GetCursorPos(&a[i]);
			printf("第%d个位置坐标为:(%ld, %ld)\n", i, a[i].x, a[i].y);
		}
		//get the number of intevals and the intevals
		printf("请输入每轮的间隔个数（输入0则沿用上次数据，否则和鼠标位置个数相同）：");
		scanf("%d", &m);
		if (m == 0) {
			std::ifstream fin(savefile);
			if (!fin) {
				printf("未找到上次保存的数据，无法沿用！\n");
				return 1;
			}
			int tmpn;
			fin >> tmpn;
			for (i = 1; i <= tmpn; i++) { int tx, ty; fin >> tx >> ty; }
			fin >> m;
			for (i = 1; i <= m; i++) fin >> b[i];
			fin.close();
			printf("已从文件沿用上次的%d个间隔\n", m);
		} else {
			printf("请依次输入每个间隔（单位：秒）：\n");
			for (i = 1; i <= m; i++)
			{
				printf("请输入第%d个间隔，然后回车继续：\n", i);
				scanf("%d", &b[i]); b[i] *= 1000;
			}
		}
		// 保存数据到文件
		std::ofstream fout(savefile);
		fout << n << " ";
		for (i = 1; i <= n; i++) fout << a[i].x << " " << a[i].y << " ";
		fout << m << " ";
		for (i = 1; i <= m; i++) fout << b[i] << " ";
		fout.close();
		printf("已保存本次数据，下次可直接输入0沿用\n");
	}
	//get the number of cycle
	printf("请输入你要循环的轮数（输入-1为无限循环），然后回车：");
	scanf("%d", &k);
	printf("即将在5秒后开始，请做好准备！\n");
	Sleep(5000);
	while(k)
	{
		k--;
		for (j = 1; j <= n; j++)
		{	
			clk(a[j].x, a[j].y);
			slp(b[j]);
		}
	}
	return 0;
}
