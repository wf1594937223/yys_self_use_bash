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
    POINT p, tmpp;
    GetCursorPos(&p);
	tmpp = pos(x, y);
    int startX = tmpp.x, startY = tmpp.y;
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

COLORREF GetColorAt(int x, int y) {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, 1, 1);
    SelectObject(hdcMem, hBitmap);

    // 复制屏幕内容
    BitBlt(hdcMem, 0, 0, 1, 1, hdcScreen, x, y, SRCCOPY | CAPTUREBLT);

    COLORREF color = GetPixel(hdcMem, 0, 0);

    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    return color;
}

bool checkRedPixel(int x, int y) {
	ShowCursor(FALSE); // 隐藏光标，避免遮挡
    // HDC hdc = GetDC(NULL);  // 获取整个屏幕的 DC
    // COLORREF color = GetPixel(hdc, x, y);
    COLORREF color = GetColorAt(x, y);
	// ReleaseDC(NULL, hdc);
	ShowCursor(TRUE);  // 恢复光标显示

    int r = GetRValue(color);
    int g = GetGValue(color);
    int b = GetBValue(color);

    // 简单判断是否为红色像素（阈值可调）
	printf("广告像素检测结果为 R=%d, G=%d, B=%d\n", r, g, b);
    return (r > 200 && g + b <= r);
}

POINT a[1010], help_mission;
int b[1010];
int main()
{
	SetProcessDPIAware(); // 使程序支持高DPI显示
	int i, j, n, m, l, tagn, tagm, tagl, k;
	// 文件名
	const char* savefile = "mouse_data.txt";
	//get the number of positions and the positions
	printf("欢迎使用本软件！每次操作后请按回车\n");
	printf("请输入你要点击的鼠标位置个数（输入0则沿用上次数据）：");
	scanf("%d", &tagn); n = tagn;
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
		scanf("%d", &tagm); m = tagm;
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
	}
	printf("请问是否需要检测广告（协作任务）弹出（负数否，正数是，0沿用上次数据）：\n");
	scanf("%d", &tagl); l = tagl;
	if (tagl > 0)
	{
		printf("请将鼠标移动到协作任务拒绝位置，然后按任意键继续。\n");
		system("pause");
		GetCursorPos(&help_mission);
		printf("协作任务拒绝位置坐标为:(%ld, %ld)\n", help_mission.x, help_mission.y);
	}
	else if (tagl == 0) {
		std::ifstream fin(savefile);
		if (!fin) {
			printf("未找到上次保存的数据，无法沿用！\n");
			return 1;
		}
		int tmpn, tmpm;
		fin >> tmpn;
		for (i = 1; i <= tmpn; i++) { int tx, ty; fin >> tx >> ty; }
		fin >> tmpm;
		for (i = 1; i <= tmpm; i++) { int tb; fin >> tb; }
		fin >> l;
		if (l > 0) fin >> help_mission.x >> help_mission.y;
		fin.close();
		if (l > 0)
			printf("已从文件沿用上次的协作任务位置:(%ld, %ld)\n", help_mission.x, help_mission.y);
		else
			printf("已从文件沿用上次的不检测广告\n");
	}
	else {
		printf("本次不检测广告（协作任务）\n");
	}
	// 保存数据到文件
	std::ofstream fout(savefile);
	fout << n << " ";
	for (i = 1; i <= n; i++) fout << a[i].x << " " << a[i].y << " ";
	fout << m << " ";
	for (i = 1; i <= m; i++) fout << b[i] << " ";
	fout << l << " ";
	if (l > 0) fout << help_mission.x << " " << help_mission.y << " ";
	fout.close();
	printf("已保存本次数据，下次可直接输入0沿用\n");
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
			if(l > 0 && checkRedPixel(help_mission.x, help_mission.y))
				clk(help_mission.x, help_mission.y);
			clk(a[j].x, a[j].y);
			slp(b[j]);
		}
	}
	return 0;
}
