#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

//初始化屏幕
void Lcdinit();

//画点
void display(int x, int y, unsigned int color);

//加载相册
void LoadBmp();

//显示一个图片
void DisplayBmp(int order);

//显示图片
void OpenBmpAndDisplay(const char *pathame);

//画一个矩形
void DisplaySql(int w, int h, int x0, int y0, unsigned int color);

//显示字模
void DisplayZimo(unsigned char *zimo, int w, int h, int x0, int y0, unsigned int color);

//显示数字在屏幕
void DisplayData(int value, int i);

//画菜单
void Menu();

//销毁屏幕
void LcdDestory();
