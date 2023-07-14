#define _CRT_SECURE_NO_WARNINGS 1
#include <string.h>
#include <math.h>
#include "../include/playerbase.h"
#define SUPERSTAR 0
#define STAR 1
#define KILL 2
#define MAX_DEP 20
struct PW
{
	struct Point p;
	int d;
};
int step[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}};
struct Point next_goal;
struct Point superstar[100];
int superstar_cnt;
int mind; // 标记现在是去KILL 还是找星星
int kill_ghost(struct Player *player, int x, int y)
{
	int gx1 = player->ghost_posx[0];
	int gy1 = player->ghost_posy[0];
	int gx2 = player->ghost_posx[1];
	int gy2 = player->ghost_posy[1];
	if (player->your_status > 1 && (x == gx1 && y == gy1 || x == gx2 && y == gy2))
		return 1;
	return 0;
}
int kill_op(struct Player *player, int x, int y)
{
	int xx = player->opponent_posx;
	int yy = player->opponent_posy;
	if (x == xx && y == yy && player->your_status > 1 && player->opponent_status == 0)
		return 1;
	return 0;
}
int is_valid(struct Player *player, int x, int y)
{
	// 安全性判断
	int hang = player->row_cnt - 1, lie = player->col_cnt - 1;
	int gx1 = player->ghost_posx[0], gy1 = player->ghost_posy[0];
	int gx2 = player->ghost_posx[1], gy2 = player->ghost_posy[1];
	int opx = player->opponent_posx, opy = player->opponent_posy;
	int xx, yy;
	// 判断 没有强化时是否会遇到鬼
	if (player->your_status <= 1)
	{
		for (int i = 0; i < 4; i++)
		{
			xx = gx1 + step[i][0];
			yy = gy1 + step[i][1];
			if (xx == x && yy == y)
				return 0;
			xx = gx2 + step[i][0];
			yy = gy2 + step[i][1];
			if (xx == x && yy == y)
				return 0;
		}
		if (x == gx1 && y == gy1 || x == gx2 && y == gy2)
			return 0;
	}
	// 判断与对手是否安全
	if (player->opponent_status > player->your_status)
	{
		for (int i = 0; i < 4; i++)
		{
			xx = opx + step[i][0];
			yy = opy + step[i][1];
			if (xx == x && yy == y)
				return 0;
		}
		if (opx == x && opy == y)
			return 0;
	}
	// 判断与边界 墙 和鬼的 位置
	if (x <= hang && x >= 0 && y >= 0 && y <= lie && (player->mat[x][y] != '#'))
		return 1;
	else
		return 0;
}
int evaluate_dis(int ex, int ey, int x, int y)
{									  // A* 估值函数
	return abs(ex - x) + abs(ey - y); // 曼哈顿距离
}
void find_dis(struct Player *player, struct PW *pos, int ex, int ey, int sx, int sy, int vis[][20], int dis)
{ // dfs搜索距离目标的最短距离
	// A*剪枝
	if (dis + evaluate_dis(ex, ey, sx, sy) > pos->d)
		return;
	if (ex == sx && ey == sy && pos->d > dis)
	{
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
	vis[sx][sy] = 0;
}
void tag_superstar(struct Player *player)
{ // 标记超级星位置
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
int find_superstar(struct Player *player)
{
	superstar_cnt = 0;
	// 遍历地图，标记所有的超级星
	tag_superstar(player);
	int mindis = MAX_DEP;
	int flag = 0;													  // 标记有无超级星
	struct Point ret = {player->your_posx, player->your_posy};		  // 当前位置
	struct Point op = {player->opponent_posx, player->opponent_posy}; // 对手位置
	struct PW pos = {ret, mindis};
	int vis[20][20] = {0};
	for (int i = 1; i <= superstar_cnt; i++)
	{
		find_dis(player, &pos, superstar[i].X, superstar[i].Y, ret.X, ret.Y, vis, 0);
		if (pos.d < mindis)
		{
			mindis = pos.d;
			next_goal = superstar[i];
			mind = SUPERSTAR;
			flag = 1;
		}
	}
	return flag;
}
void init(struct Player *player)
{
	if (!find_superstar(player))
	{
		next_goal.X = player->your_posx;
		next_goal.Y = player->your_posy;
	}
}
// 暂时不用这个函数
// int  seek_op(struct Player* player) {    //判断当前状态，如果强化状态，且有可能击杀对手，就设置目标去kill opponent
//	struct Point ret = { player->your_posx, player->your_posy };  //当前位置
//	struct Point op = { player->opponent_posx, player->opponent_posy }; //对手位置
//	struct PW  pos = { ret, MAX_DEP };
//	int vis[20][20] = { 0 };
//	//判断能否有可能追上
//	int flag = 0;
//	if (player->your_status > 1 && player->your_status > player->opponent_status) {
//		find_dis(player, &pos, player->opponent_posx, player->opponent_posy, ret.X, ret.Y, vis, 0);
//		if (pos.d < player->your_status) {
//			next_goal = op;
//			mind = KILL;
//			flag = 1;
//		}
//	}
//	return flag;
// }
int seek_ghost(struct Player *player)
{																	  // 判断当前状态，如果强化状态，且有可能击杀对手，就设置目标去kill opponent
	struct Point ret = {player->your_posx, player->your_posy};		  // 当前位置
	struct Point g1 = {player->ghost_posx[0], player->ghost_posy[0]}; // 1鬼位置
	struct Point g2 = {player->ghost_posx[1], player->ghost_posy[1]}; // 2鬼位置
	struct PW pos = {ret, MAX_DEP};
	int vis[20][20] = {0};
	// 判断能否有可能追上
	int flag = 0;
	int temp_dis = 0;
	if (player->your_status > 1)
	{
		find_dis(player, &pos, g1.X, g1.Y, ret.X, ret.Y, vis, 0);
		if (pos.d < player->your_status)
		{
			temp_dis = pos.d;
			next_goal = g1;
			mind = KILL;
			flag = 1;
		}
		find_dis(player, &pos, g2.X, g2.Y, ret.X, ret.Y, vis, 0);
		if (pos.d < temp_dis)
		{
			next_goal = g2;
			mind = KILL;
			flag = 1;
		}
	}
	return flag;
}
void seek_star(struct Player *player, int x, int y, struct PW *pos, int dis, int vis[][20])
{ // dfs搜索和最短距离的星星的位置
	if (dis > pos->d)
		return;
	if ((player->mat[x][y] == 'o' || player->mat[x][y] == 'O'))
	{
		pos->p.X = x;
		pos->p.Y = y;
		pos->d = dis;
		return;
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
	vis[x][y] = 0;
}
void judge_reset(struct Player *player)
{
	struct Point ret = {player->your_posx, player->your_posy};
	// 判断是否找到超级星
	if (mind == SUPERSTAR && next_goal.X == ret.X && next_goal.Y == ret.Y)
	{
		mind = STAR;
		// 试试下一个超级星看能否设置为目标
		find_superstar(player);
	}
	// 判断能否设置KILL为目标
	seek_ghost(player);
	// 看能否设置
	if (mind == STAR && superstar_cnt > 0)
	{
		find_superstar(player);
	}
}
struct Point walk(struct Player *player)
{
	struct Point ret = {player->your_posx, player->your_posy}; // 当前位置
	int mindis = MAX_DEP;									   // 限制搜索深度
	struct PW pos = {ret, mindis};
	struct Point next = ret; // 下一步走的地方 初始化为原地不动
	int xx, yy;
	// 判断目标重置
	judge_reset(player);
	// 判断是否达到目标   如果达到更新目标并朝新目标移动一次
	if (next_goal.X == ret.X && next_goal.Y == ret.Y)
	{
		for (int i = 0; i < 4; i++)
		{
			int vis[20][20] = {0}; // 搜索标记,每次都要重置
			xx = ret.X + step[i][0];
			yy = ret.Y + step[i][1];
			if (kill_op(player, xx, yy))
			{
				next_goal = ret;
				break;
			}
			if (is_valid(player, xx, yy))
			{

				// 寻找一个最近的星星,设定下一个目标星星
				seek_star(player, xx, yy, &pos, 1, vis);
				if (pos.d < mindis)
				{
					next.X = xx;
					next.Y = yy;
					// 设置目标星星
					next_goal = pos.p;
					mindis = pos.d;
				}
			}
		}
		ret = next;
		mind = STAR;
	}
	// 继续寻找目标
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int vis[20][20] = {0}; // 搜索标记,每次都要重置
			xx = ret.X + step[i][0];
			yy = ret.Y + step[i][1];
			if (kill_op(player, xx, yy))
			{
				next_goal = ret;
				break;
			}
			if (is_valid(player, xx, yy))
			{
				find_dis(player, &pos, next_goal.X, next_goal.Y, xx, yy, vis, 1);
				if (pos.d < mindis)
				{
					next.X = xx;
					next.Y = yy;
					mindis = pos.d;
				}
			}
		}
		ret = next;
	}
	return ret;
}
