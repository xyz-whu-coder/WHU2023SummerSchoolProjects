#define _CRT_SECURE_NO_WARNINGS 1
#include <string.h>
#include <math.h>
#include "../include/playerbase.h"
#define BLANK 0
#define STAR 1
#define KILL 2
struct PW
{
	struct Point p;
	int d;
	int flag;
};
int step[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}};
struct Point next_star;
struct Point superstar[10];
int superstar_cnt;
int mind;
void find_superstar(struct Player *player)
{
	int hang = player->row_cnt - 1;
	int lie = player->col_cnt - 1;
	for (int i = 0; i <= hang; i++)
	{
		for (int j = 0; j <= lie; j++)
		{
			if (player->mat[i][j] == 'O')
			{
				superstar[++superstar_cnt].X = i;
				superstar[superstar_cnt].Y = j;
			}
		}
	}
}
void init(struct Player *player)
{
	find_superstar(player);
	next_star = superstar[superstar_cnt--];
	mind = STAR;
}
int is_valid(struct Player *player, int x, int y)
{
	// 合法性判断
	int hang = player->row_cnt - 1;
	int lie = player->col_cnt - 1;
	int gx1 = player->ghost_posx[0];
	int gy1 = player->ghost_posy[0];
	int gx2 = player->ghost_posx[1];
	int gy2 = player->ghost_posy[1];
	// 判断与边界 墙 和鬼的 位置
	if (x <= hang && x >= 0 && y >= 0 && y <= lie && (player->mat[x][y] != '#'))
		return 1;
	else
		return 0;
}
void find_dis(struct Player *player, struct PW *pos, int ex, int ey, int sx, int sy, int vis[][20], int dis)
{ // bfs搜索距离目标星星的最短距离
	if (pos->flag == 1)
		return;
	if (ex == sx && ey == sy)
	{
		pos->flag = 1;
		pos->d = dis;
		return;
	}
	if (vis[sx][sy])
		return;
	vis[sx][sy] = 1;
	// 标记访问
	int xx, yy;
	for (int i = 0; i < 4; i++)
	{
		xx = sx + step[i][0];
		yy = sy + step[i][1];
		if (is_valid(player, xx, yy) && vis[xx][yy] == 0)
		{
			find_dis(player, pos, ex, ey, xx, yy, vis, dis + 1);
		}
	}
}

void seek_op(struct Player *player)
{
	struct Point ret = {player->your_posx, player->your_posy}; // 当前位置
	struct Point op = {player->opponent_posx, player->opponent_posy};
	struct PW pos = {ret, 0, 0};
	int vis[20][20] = {0};
	if (player->your_status > 0 && player->your_status > player->opponent_status)
	{
		find_dis(player, &pos, player->opponent_posx, player->opponent_posy, ret.X, ret.Y, vis, 0);
		if (pos.d < player->your_status)
		{
			next_star = op;
			mind = KILL;
		}
	}
	else
	{
		next_star = ret;
		mind = STAR;
	}
}

void seek_star(struct Player *player, int x, int y, struct PW *pos, int dis, int vis[][20])
{ // bfs搜索和最短距离星星的位置
	if (pos->flag == 1)
		return;
	if (player->mat[x][y] == 'o' || player->mat[x][y] == 'O')
	{
		//	int op_vis[20][20] = { 0 };
		//	struct PW op_pos;
		//	op_pos.p.X = x;
		//	op_pos.p.Y = y;
		//	star_opDis(player, &op_pos, x, y, player->opponent_posx, player->opponent_posy, op_vis, 0);
		// if (op_pos.d >= dis) {
		// 计算对手和自己离星星的距离
		pos->p.X = x;
		pos->p.Y = y;
		pos->d = dis;
		pos->flag = 1;
		return;
		//}
	}
	if (vis[x][y])
		return;
	vis[x][y] = 1;
	// 标记访问
	int xx, yy;
	for (int i = 0; i < 4; i++)
	{
		xx = x + step[i][0];
		yy = y + step[i][1];
		if (is_valid(player, xx, yy) && vis[xx][yy] == 0)
		{
			seek_star(player, xx, yy, pos, dis + 1, vis);
		}
	}
}
struct Point walk(struct Player *player)
{
	seek_op(player);
	struct Point ret = {player->your_posx, player->your_posy}; // 当前位置
	struct PW pos = {ret, 0, 0};
	struct Point next = ret; // 下一步走的地方
	int xx, yy;
	int min_dis = 1000;
	// 判断是否到达要找的星星或者对手
	if (mind == STAR && next_star.X == ret.X && next_star.Y == ret.Y || mind == KILL && player->opponent_status == -1)
	{
		for (int i = 0; i < 4; i++)
		{
			pos.flag = 0;
			int vis[20][20] = {0}; // bfs搜索标记,每次都要重置
			xx = ret.X + step[i][0];
			yy = ret.Y + step[i][1];
			pos.flag = 0;
			if (is_valid(player, xx, yy))
			{
				// 寻找一个最近的星星,设定下一个目标星星
				seek_star(player, xx, yy, &pos, 1, vis);
				if (pos.d < min_dis)
				{
					next.X = xx;
					next.Y = yy;
					// 设置目标星星

					next_star = pos.p;
					min_dis = pos.d;
				}
			}
		}
		ret = next;
	}
	// 继续寻找目标星星
	else
	{
		for (int i = 0; i < 4; i++)
		{
			pos.flag = 0;
			int vis[20][20] = {0}; // bfs搜索标记,每次都要重置
			xx = ret.X + step[i][0];
			yy = ret.Y + step[i][1];
			pos.flag = 0;
			if (is_valid(player, xx, yy))
			{
				find_dis(player, &pos, next_star.X, next_star.Y, xx, yy, vis, 1);
				if (pos.d < min_dis)
				{
					next.X = xx;
					next.Y = yy;
					min_dis = pos.d;
				}
			}
		}
		ret = next;
	}

	return ret;
}