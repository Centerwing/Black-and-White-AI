#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define VP 5000
#define VA -100
#define VB -2
#define VR -3
#define VT -2
#define VY 0
#define VI -3
#define VF -3
#define VC -1
#define VD -1
#define VE -4
#define VX -120
int map[16][16], valid[16][16];
int value[16][16] = {
	{ VP,VA,VC,VB,VB,VB,VD,VD,VD,VD,VB,VB,VB,VC,VA,VP },
	{ VA,VX,VE,VF,VF,VF,VF,VF,VF,VF,VF,VF,VF,VE,VX,VA },
	{ VC,VE,VF,VR,VR,VR,VR,VR,VR,VR,VR,VR,VR,VF,VE,VC },
	{ VB,VF,VR,VR,VR,VR,VT,VT,VT,VT,VR,VR,VR,VR,VF,VB },
	{ VB,VF,VR,VR,VT,VT,VT,VT,VT,VT,VT,VT,VR,VR,VF,VB },
	{ VB,VF,VR,VR,VT,VT,VY,VY,VY,VY,VT,VT,VR,VR,VF,VB },
	{ VD,VF,VR,VT,VT,VY,VY,VY,VY,VY,VY,VT,VT,VR,VF,VD },
	{ VD,VF,VR,VT,VT,VY,VY,VI,VI,VY,VY,VT,VT,VR,VF,VD },
	{ VD,VF,VR,VT,VT,VY,VY,VI,VI,VY,VY,VT,VT,VR,VF,VD },
	{ VD,VF,VR,VT,VT,VY,VY,VY,VY,VY,VY,VT,VT,VR,VF,VD },
	{ VB,VF,VR,VR,VT,VT,VY,VY,VY,VY,VT,VT,VR,VR,VF,VB },
	{ VB,VF,VR,VR,VT,VT,VT,VT,VT,VT,VT,VT,VR,VR,VF,VB },
	{ VB,VF,VR,VR,VR,VR,VT,VT,VT,VT,VR,VR,VR,VR,VF,VB },
	{ VC,VE,VF,VR,VR,VR,VR,VR,VR,VR,VR,VR,VR,VF,VE,VC },
	{ VA,VX,VE,VF,VF,VF,VF,VF,VF,VF,VF,VF,VF,VE,VX,VA },
	{ VP,VA,VC,VB,VB,VB,VD,VD,VD,VD,VB,VB,VB,VC,VA,VP }, };//价值数组
int color, bestx, besty, count;
void change(int x, int y, int c);//进行落子
void allow(int c);//计算可行点
int analyze(int n, int upbest, int depth);//递归搜索
void printmap(int pto[16][16]);//调试用
int score(void);//判断局势
void valid0(void);//重置可行点数组
int wall();//边角稳定子
void placex();//调整策略
int main(void)
{
	int placeX, placeY;
	char order;
	map[7][7] = map[8][8] = 2;
	map[7][8] = map[8][7] = 1;
	for (count = 0;; count++)
	{
		order = getchar();//读取命令的首字符
		if (order == 'S')
		{
			scanf_s("%*s %d", &color);
			while (getchar() != '\n');//清除输入
			printf("OK\n");
			fflush(stdout);
		}
		else if (order == 'P')
		{
			scanf_s("%*s %d %d", &placeX, &placeY);
			while (getchar() != '\n');
			change(placeX, placeY, 3 - color);
		}
		else if (order == 'T')
		{
			while (getchar() != '\n');
			placex();
			if (count < 235)
				analyze(4, 0, 4);
			else if (count >= 235 && count < 245)
				analyze(6, 0, 6);
			else
				analyze(8, 0, 8);
			printf("%d %d\n", bestx, besty);
			fflush(stdout);
			change(bestx, besty, color);
		}
		printmap(map);//调
					  //printmap(valid);//。
					  //printmap(value);//。
		printf("%d ", score());//试
	}
	return 0;
}
void change(int x, int y, int c)//进行落子
{
	int cd, rd, row, col;
	map[x][y] = c;
	for (cd = -1; cd <= 1; cd++)
		for (rd = -1; rd <= 1; rd++)
		{
			if (x + rd < 0 || y + cd < 0 || x + rd>15 || y + cd>15 || (rd == 0 && cd == 0))
				continue;//检查是否出界
			if (map[x + rd][y + cd] == 3 - c)
			{
				for (row = x + rd, col = y + cd;;)
				{
					row += rd;
					col += cd;
					if (row < 0 || row>15 || col < 0 || col>15 || map[row][col] == 0)
						break;
					if (map[row][col] == c)
					{
						while (map[row -= rd][col -= cd] == 3 - c)
							map[row][col] = c;
						break;
					}//进行翻转
				}
			}
		}
}
void valid0(void)//重置可行点数组
{
	int i, j;
	for (i = 0; i<16; i++)
		for (j = 0; j < 16; j++)
			valid[i][j] = 0;
}
void allow(int c)//找出可行点
{
	int cd, rd, row, col, x, y;
	valid0();
	for (row = 0; row < 16; row++)
		for (col = 0; col < 16; col++)
		{
			if (map[row][col] != 0)
				continue;//找到空位
			for (rd = -1; rd <= 1; rd++)
				for (cd = -1; cd <= 1; cd++)
				{
					if (row + rd < 0 || row + rd>15 || col + cd < 0 || col + cd>15 || (rd == 0 && cd == 0))
						continue;//检查是否出界
					if (map[row + rd][col + cd] == 3 - c)
					{
						for (x = row + rd, y = col + cd;;)
						{
							x += rd;
							y += cd;
							if (x < 0 || x>15 || y < 0 || y>15 || map[x][y] == 0)
								break;
							if (map[x][y] == c)
							{
								valid[row][col] = 1;//标记可行点
								break;
							}
						}
					}
				}
		}
}
void printmap(int pto[16][16])//调试
{
	int i, j;
	printf("   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15\n");
	for (i = 0; i < 16; i++)
	{
		printf("%2d ", i);
		for (j = 0; j < 16; j++)
			if (!pto[i][j])
				printf("+    ");
			else
				printf("%c    ", pto[i][j] == 1 ? 'X' : 'O');
		printf("\n\n");
	}
}
int score()//判断局势
{
	int i, j, s = 0, t = 0;
	allow(color);
	for (i = 0; i<16; i++)
		for (j = 0; j < 16; j++)
		{
			if (map[i][j] == color)
			{
				t++;
				s += value[i][j];
				if (count > 200)s += 5;//吃
			}
			else if (map[i][j] == 3 - color)
			{
				s -= value[i][j];
				if (count > 220)s -= 5;//多吃一点
			}
			else continue;
		}
	if (count > 244)return t;//最后8步
	if (count > 5 && t == 0)return -30000;//不要被吃光
	else if (count > 9 && t == 1)return -25000;//不要被吃光
	else if (count > 9 && t == 2)return -20000;//不要被吃光
	else if (count > 9 && t == 3)return -10000;//不要被吃光
	s /= 3;
	for (i = 0; i<16; i++)
		for (j = 0; j < 16; j++)
			if (valid[i][j])//行动力修正
				s += 5;
	allow(3 - color);
	for (i = 0; i < 16; i++)
		for (j = 0; j < 16; j++)
			if (valid[i][j])//对手行动力
				s += -5;
	s += wall();
	return s;
}
int wall()//判断边角稳定子
{
	int i, sum = 0, dwall = 7;
	for (i = 0; i < 16; i++)
		if (map[0][i] == color)(sum += dwall);
		else
			break;
	for (i = 0; i < 16; i++)
		if (map[i][15] == color)(sum += dwall);
		else
			break;
	for (i = 0; i < 16; i++)
		if (map[15][i] == color)(sum += dwall);
		else
			break;
	for (i = 0; i < 16; i++)
		if (map[i][0] == color)(sum += dwall);
		else
			break;
	for (i = 15; i >= 0; i--)
		if (map[0][i] == color)(sum += dwall);
		else
			break;
	for (i = 15; i >= 0; i--)
		if (map[i][15] == color)(sum += dwall);
		else
			break;
	for (i = 15; i >= 0; i--)
		if (map[15][i] == color)(sum += dwall);
		else
			break;
	for (i = 15; i >= 0; i--)
		if (map[i][0] == color)(sum += dwall);
		else
			break;
	return sum;
}
void placex()//调整策略
{
	if (map[0][0])
		value[0][1] = value[1][0] = value[1][1] = 20;
	if (map[15][0])
		value[15][1] = value[14][0] = value[14][1] = 20;
	if (map[0][15])
		value[0][14] = value[1][15] = value[1][14] = 20;
	if (map[15][15])
		value[14][15] = value[15][14] = value[14][14] = 20;
}
int analyze(int n, int upbest, int depth)
{
	int row, col, s, t = 1, bestscore = (n % 2) ? 32767 : -32767;
	int map_copy[16][16], valid_copy[16][16];
	allow(n % 2 ? 3 - color : color);
	memcpy(map_copy, map, sizeof(map));//备
	memcpy(valid_copy, valid, sizeof(valid));//份
	for (row = 0; row < 16; row++)
		for (col = 0; col < 16; col++)
		{
			if (valid[row][col])
			{
				t = 0;//有落子点
				change(row, col, n % 2 ? 3 - color : color);
				if (n == depth)//存储当前落子点
				{
					s = analyze(n - 1, bestscore, depth);
					if (bestscore <= s)
					{
						bestscore = s;
						bestx = row;
						besty = col;
					}
				}
				else
				{
					if (n % 2)//对手回合
					{
						s = (n == 1) ? score() : analyze(n - 1, bestscore, depth);
						if (bestscore >= s)
							bestscore = s;
						if (bestscore <= upbest)
							return -32767;
					}
					else
					{
						s = analyze(n - 1, bestscore, depth);
						if (bestscore <= s)
							bestscore = s;
						if (bestscore >= upbest)
							return 32767;
					}
				}
			}
			memcpy(map, map_copy, sizeof(map));//复
			memcpy(valid, valid_copy, sizeof(valid));//位
		}
	if (t)bestscore = score();
	return bestscore;
}