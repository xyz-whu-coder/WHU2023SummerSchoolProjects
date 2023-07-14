# reversi_lab_project

[toc]

# 黑白棋

## 实验说明

1. [黑白棋的规则](https://baike.baidu.com/item/黑白棋/80689)

2. 游戏有两名 AI 玩家（由两份代码控制下棋）

- 机器人 AI
- 你的 AI

3. 实验任务
你需要在 `player.h` 文件中编写 `struct Point place(struct Player *player);` 函数，你的 AI 程序会在你的回合中根据此函数决策落子的位置。当一方本回合无可落子位置时，返回 `-1 -1` 表示放弃本回合。


### 游戏规则

在经典 [黑白棋](https://baike.baidu.com/item/黑白棋/80689) 规则的基础之上，增加一个额外信息：棋子分数。对于棋盘上的每个格子，均有一个 $1$ 至 $9$ 之间的分数，格子上的分数属于格子上的棋子所属人。

对于你的 AI，己方棋子在棋盘上用大写`O`表示，对方棋子用小写`o`表示。

初始每个人的棋子所在格子的分数为 $0$，其余可通过 `mat` 初始时的值得到。

当一方本回合无可落子位置时，返回 `-1 -1` 表示放弃本回合。

如果移动不合法（包含移动到不合法位置、在不必须放弃的回合直接放弃等），则分数直接记为 $−100$。

## Missions

### 任务一

地图中所有格子**除了中间四个**以外的分数均为 $1$。

程序编译通过可以获得基础 $10$ 分。

给定三张地图在`data/map*.txt`中可以查看，对手 AI 代码可以在`code/computer.h`中查看。对于每张地图会与对手 AI 对战两次，若总分大于对手，则该张图获得 $30$ 分，否则不得分。

> 如果对战的时候，将对手的棋子吃完，则当前回合的计分规则：
> - 当前分数 $≥$ 棋盘总分 $\times$ $\frac{3}{4}$ ，则本局分数为：当前分数 $+bonus$；（棋盘大小 $8×8,10×10$ 时 $bonus=10$；$12×12$ 时，$bonus=20$）
> - 当前分数 $<$ 棋盘总分 $× \frac{3}{4}$，则本局分数为：棋盘总分 $× \frac{3}{4}$。

### 任务二

地图中所有格子**除了中间四个以及四个角**以外的分数均为 $1$。

给定四张地图在`data/map*.txt`中可以查看，对手 AI 代码可以在`code/computer.h`中查看。对于每张地图会与对手 AI 对战两次，若总分大于对手，则该张图获得 $25$ 分，否则不得分。

> 如果对战的时候，将对手的棋子吃完，则当前回合的计分规则：
> - 当前分数 $≥$ 棋盘总分 $\times$ $\frac{3}{4}$ ，则本局分数为：当前分数 $+bonus$；（棋盘大小 $8×8,10×10$ 时 $bonus=10$；$12×12$ 时，$bonus=20$）
> - 当前分数 $<$ 棋盘总分 $× \frac{3}{4}$，则本局分数为：棋盘总分 $× \frac{3}{4}$。

### 任务三

地图中所有格子**除了中间四个以及最外层一圈**以外的格子分数均为 $1$。

给定四张地图在`data/map*.txt`中可以查看，对手 AI 代码可以在`code/computer.h`中查看。对于每张地图会与对手 AI 对战两次，若总分大于对手，则该张图获得 $25$ 分，否则不得分。

> 如果对战的时候，将对手的棋子吃完，则当前回合的计分规则：
> - 当前分数 $≥$ 棋盘总分 $\times$ $\frac{3}{4}$ ，则本局分数为：当前分数 $+bonus$；（棋盘大小 $8×8,10×10$ 时 $bonus=10$；$12×12$ 时，$bonus=20$）
> - 当前分数 $<$ 棋盘总分 $× \frac{3}{4}$，则本局分数为：棋盘总分 $× \frac{3}{4}$。

### 任务四

地图中**除了中间四个格子**以外的其余格子的分数范围为 $1∼9$。

给定四张地图在`data/map*.txt`中可以查看，对手 AI 代码可以在`code/computer.h`中查看。对于每张地图会与对手 AI 对战两次，若总分大于对手，则该张图获得 $25$ 分，否则不得分。

> 如果对战的时候，将对手的棋子吃完，则当前回合的计分规则：
> - 当前分数 $≥$ 棋盘总分 $\times$ $\frac{3}{4}$ ，则本局分数为：当前分数 $+bonus$；（棋盘大小 $8×8,10×10$ 时 $bonus=10$；$12×12$ 时，$bonus=20$）
> - 当前分数 $<$ 棋盘总分 $× \frac{3}{4}$，则本局分数为：棋盘总分 $× \frac{3}{4}$。

### PlayerBase 结构体

```c
struct Player {
    char **mat;  // Game Data
​
    int row_cnt;  // Count of rows in current map
​
    int col_cnt;  // Count of columns in current map
​
    int your_score;  // Your AI's score
​
    int opponent_score;  // Opponent AI's score
    
};
​
struct Point place(struct Player *player);  // Place function which you need to implement
​
void init(struct Player *player);  // Init function which you need to implement

```
### 测试运行

执行`./run.sh`脚本即可测试。可以在`logs`目录内查看游戏过程和最终分数。

可以执行`./run.sh --visible`进行可视化测试。

可以将要测试的地图内容复制到`data/map.txt`中进行测试。

### 时间限制

- 初始化的时间限制为 $1$ s。
- 每回合移动的计算时间限制为 $100$ ms。
- 若回合移动的计算超时，则系统会认为此次不移动。

### 空间限制

空间限制为 $512$ MB，于最终评测时进行限制。

### 函数调用限制

以下系统调用及函数禁止使用

- 新建进程（调用fork）
- 禁止读写文件
- 禁止调用`PlayerBase`类的`_recv`、`_send`、`_work`、`_init_thread`、`_walk_thread`、`_syscall_check`方法。

### 程序异常处理

若 AI 进程在某次`place`计算时异常退出，则系统会认为该 AI 失败，游戏结束。

### 项目包下载

为了避免大家打开实验时卡顿，请同学们自行下载每个实验对应的项目包，同时配合上一页先行了解游戏规则。大家尽量在**本地进行测试**。

1. 任务一链接: https://pan.baidu.com/s/10uTjMtIoqDrhyB09Gax__Q 提取码: 1024

2. 任务二链接: https://pan.baidu.com/s/1iOn3gA26V99qqCSzDcQEeA 提取码: 1024

3. 任务三链接: https://pan.baidu.com/s/1FegOKDA9poMp0CtprOvL4g 提取码: 1024

4. 任务四链接: https://pan.baidu.com/s/1mIEFL6chlFqzANHCYk8lLA?pwd=1024 提取码: 1024

下载后双击解压，就可以得到项目包了。


## 策略

应该尽量争取角落（因为不会被翻转），避免别人占据角落（不下距离角落最近的三个格子），以此扩大优势。

## 实现

### 最新的一版`player.h`

```c
/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
typedef struct PW
{
	struct Point point;
	int weight; // 权重
	int value;	// 价值：翻转棋子个数
} P;
int Map[20][20];
int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};

void init_Map(struct Player *player)
{
	int hang = player->row_cnt - 1, lie = player->col_cnt - 1;
	Map[0][0] = 500 * player->mat[0][0];
	Map[0][lie] = 500 * player->mat[0][lie];
	Map[hang][0] = 500 * player->mat[hang][0];
	Map[hang][lie] = 500 * player->mat[hang][lie];
	Map[0][1] = Map[0][lie - 1] = Map[1][0] = Map[1][lie] = Map[hang - 1][0] = Map[hang - 1][lie] = Map[hang][1] = Map[hang][lie - 1] = -45;
	for (int i = 1; i <= lie - 1; i++)
		Map[1][i] = Map[hang - 1][i] = -1;
	for (int i = 1; i <= hang - 1; i++)
		Map[i][1] = Map[i][lie - 1] = -1;
	Map[1][1] = Map[1][lie - 1] = Map[hang - 1][1] = Map[hang - 1][lie - 1] = -45;
	int midx = hang / 2, midy = lie / 2;
	for (int i = midx; i <= midx + 1; i++)
		Map[i][midy - 1] = Map[i][midy + 2] = 2;
	for (int i = midy; i <= midy + 1; i++)
		Map[midx - 1][i] = Map[midx + 2][i] = 2;
	Map[midx - 1][midy - 1] = Map[midx - 1][midy + 2] = Map[midx + 2][midy - 1] = Map[midx + 2][midy + 2] = 3;
	for (int i = 3; i <= hang - 3; i++)
		Map[i][0] = Map[i][lie] = 5;
	for (int i = 3; i <= lie - 3; i++)
		Map[0][i] = Map[hang][i] = 5;
	Map[0][2] = Map[0][lie - 2] = Map[2][0] = Map[2][lie] = Map[hang - 2][0] = Map[hang - 2][lie] = Map[hang][lie - 2] = Map[hang][2] = 100;
}
void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
	init_Map(player);
	srand(time(0));
}
int is_valid(struct Player *player, int posx, int posy, int *value)
{
	if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
	{
		return 0;
	}
	if (player->mat[posx][posy] == 'o' || player->mat[posx][posy] == 'O')
	{
		return 0;
	}
	for (int dir = 0; dir < 8; dir++)
	{
		int x = posx + step[dir][0];
		int y = posy + step[dir][1];
		if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
			continue;
		if (player->mat[x][y] != 'o')
			continue;
		int cnt = 1;
		while (1)
		{
			x += step[dir][0];
			y += step[dir][1];
			if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || player->mat[x][y] >= '1' && player->mat[x][y] <= '9')
				break;
			cnt++;
			if (player->mat[x][y] == 'O')
			{
				*value = cnt;
				return 1;
			}
		}
	}
	return 0;
}
int is_jiao(struct Player *player, int x, int y)
{
	if ((x == 0 || x == player->row_cnt - 1) && (y == 0 || y == player->col_cnt - 1))
		return 1;
	return 0;
}
int update_fun(struct Player *player, int i, int j)
{
	int hang = player->row_cnt - 1, lie = player->col_cnt - 1;
	if (Map[i][j] == 200)
		return 0;
	if (i == 0 && j == 0)
	{
		for (int k = j; k <= lie - 2; k++)
			Map[i][k] = 200;
		for (int k = i; k <= hang - 2; k++)
			Map[k][j] = 200;
		Map[1][1] = 200;
	}
	else if (i == 0 && j == lie)
	{
		for (int k = 2; k <= j; k++)
			Map[i][k] = 200;
		for (int k = i; k <= hang - 2; k++)
			Map[k][j] = 200;
		Map[1][lie - 1] = 200;
	}
	else if (i == hang && j == 0)
	{
		for (int k = j; k <= lie - 2; k++)
			Map[i][k] = 200;
		for (int k = 2; k <= i; k++)
			Map[k][j] = 200;
		Map[hang - 1][1] = 200;
	}
	else if (i == hang && j == lie)
	{
		for (int k = 2; k <= j; k++)
			Map[i][k] = 200;
		for (int k = 2; k <= i; k++)
			Map[k][j] = 200;
		Map[hang - 1][lie - 1] = 200;
	}
	return 1;
}
void dfs(struct Player *player, int i, int j, int vis[][20])
{
	if (vis[i][j] || i < 0 || j < 0 || i >= player->row_cnt || j >= player->col_cnt)
		return;
	vis[i][j] = 1;
	if (player->mat[i][j] != 'O')
	{
		Map[i][j] = 100;
		return;
	}
	for (int dir = 0; dir < 8; dir++)
	{
		int xx = i + step[dir][0];
		int yy = j + step[dir][1];
		dfs(player, xx, yy, vis);
	}
}
void update_map(struct Player *player)
{
	int hang = player->row_cnt - 1, lie = player->col_cnt - 1;
	int vis[20][20] = {0};
	for (int i = 0; i <= hang; i += hang)
	{
		for (int j = 0; j <= lie; j += lie)
		{
			if (is_jiao(player, i, j) && player->mat[i][j] == 'O')
			{
				if (!update_fun(player, i, j))
					dfs(player, i, j, vis);
			}
		}
	}
}
int SensPoint(struct Player *player, int x, int y)
{
	return (x == 0 && y == 1) || (x == 1 && y == 0) || (x == 1 && y == 1) || (x == 1 && y == player->col_cnt - 2) || (x == 1 && y == player->col_cnt - 1) || (x == 0 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == 1) || (x == player->row_cnt - 2 && y == 0) || (x == player->row_cnt - 1 && y == 1) || (x == player->row_cnt - 2 && y == player->col_cnt - 2) || (x == player->row_cnt - 1 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == player->col_cnt - 1);
}
void ChangeMat(struct Player *player, int i, int j)
{
	player->mat[i][j] = 'O';
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
	int flag; // flag用来判断一个方向上是否有子要翻转
	for (int dir = 0; dir < 8; dir++)
	{
		flag = 0; // 初始为0，可以翻转为1 ，然后进行翻转。
		int x = i + step[dir][0];
		int y = j + step[dir][1];
		while (x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt)
		{
			if (player->mat[x][y] == 'O')
			{
				flag = 1;
				break;
			}
			x += step[dir][0];
			y += step[dir][1];
		}
		if (flag)
		{
			x = i + step[dir][0];
			y = j + step[dir][1];
			while (x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt)
			{
				if (player->mat[x][y] == 'o')
				{
					player->mat[x][y] = 'O';
				}
				else
				{
					break;
				}
				x += step[dir][0];
				y += step[dir][1];
			}
		}
	}
	return;
}
// 判断算法, 从computer.h中获取
int opponent_valid(struct Player *player, int posx, int posy)
{
	// 检查坐标（posx，posy）是否在棋盘内。如果坐标在棋盘外则返回false。
	if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
	{
		return false;
	}
	// 检查在坐标（posx，posy）的棋盘位置是否空着。如果该位置已经有棋子（'o'或'O'），则返回false。
	if (player->mat[posx][posy] == 'O' || player->mat[posx][posy] == 'o')
	{
		return false;
	}
	// 定义8个方向的坐标偏移，分别对应右，左，下，上，右下，左上，右上，左下。
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};

	// 遍历所有的方向。
	for (int dir = 0; dir < 8; dir++)
	{
		// 计算在当前方向上的下一步的坐标。
		int x = posx + step[dir][0];
		int y = posy + step[dir][1];
		// 如果下一步的坐标在棋盘外，则继续检查下一个方向。
		if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
		{
			continue;
		}
		// 如果下一步的坐标上的棋子不是'o'，则继续检查下一个方向。
		if (player->mat[x][y] != 'O')
		{
			continue;
		}
		// 当下一步的坐标在棋盘内且该位置的棋子为'o'时，继续沿当前方向前进，直到走到棋盘边缘或遇到非'o'的棋子。
		while (true)
		{
			x += step[dir][0];
			y += step[dir][1];
			if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
			{
				break;
			}
			// 如果在这个方向上有'O'，则说明当前位置（posx，posy）是一个合法的位置，返回true。
			if (player->mat[x][y] == 'o')
			{
				return true;
			}
		}
	}
	// 如果所有的方向都被检查过仍然没有找到'O'，则说明当前位置（posx，posy）不是合法位置，返回false。
	return false;
}

struct Point place(struct Player *player)
{
	update_map(player);
	int chess_cnt = player->your_score + player->opponent_score;
	int sum = (player->col_cnt) * (player->row_cnt);
	struct PW *ok_point = (struct PW *)malloc((player->row_cnt * player->col_cnt) * sizeof(struct PW));
	int ok_cnt = 0; // 可以走的数量
	int Max_weight = -1000;
	int value = 0;
	int Min_num = 1000; // 最小行动力
	int jiao_num = 100;
	char temp_Mat[20][20];
	struct Point point = initPoint(-1, -1);
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (is_valid(player, i, j, &value))
			{
				ok_point[ok_cnt++].point = initPoint(i, j);
				ok_point[ok_cnt].weight = Map[i][j];
				for (int ii = 0; ii < player->row_cnt; ii++)
					for (int jj = 0; jj < player->col_cnt; jj++)
						temp_Mat[ii][jj] = player->mat[ii][jj];
				ChangeMat(player, i, j);
				int num = 0;
				for (int pp = 0; pp < player->row_cnt; pp++)
					for (int qq = 0; qq < player->col_cnt; qq++)
						if (opponent_valid(player, pp, qq) && is_jiao(player, pp, qq))
							num++;
				for (int ii = 0; ii < player->row_cnt; ii++)
					for (int jj = 0; jj < player->col_cnt; jj++)
						player->mat[ii][jj] = temp_Mat[ii][jj];
				if (Map[i][j] > Max_weight || num < jiao_num)
				{
					point.X = i;
					point.Y = j;
					jiao_num = num;
					Max_weight = Map[i][j];
				}

				else if (Map[i][j] == Max_weight)
				{
					for (int ii = 0; ii < player->row_cnt; ii++)
						for (int jj = 0; jj < player->col_cnt; jj++)
							temp_Mat[ii][jj] = player->mat[ii][jj];

					ChangeMat(player, i, j);
					int num = 0; // 统计行动力
					for (int pp = 0; pp < player->row_cnt; pp++)
						for (int qq = 0; qq < player->col_cnt; qq++)
							if (opponent_valid(player, pp, qq) && !SensPoint(player, pp, qq))
								// 四个角的敏感区不算有效步
								num++;
					if (num < Min_num)
					{
						point.X = i;
						point.Y = j;
						Min_num = num;
					}
					for (int ii = 0; ii < player->row_cnt; ii++)
						for (int jj = 0; jj < player->col_cnt; jj++)
							player->mat[ii][jj] = temp_Mat[ii][jj];
				}
			}
		}
	}
	free(ok_point);
	return point;
}

```

***感谢小组内的所有成员***


