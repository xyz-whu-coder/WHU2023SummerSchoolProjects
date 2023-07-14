#define _CRT_SECURE_NO_WARNINGS 1
#include <string.h>
#include <math.h>
#include "../include/playerbase.h"
struct PW
{
    struct Point p;
    int d;
    int flag;
};
int step[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
void init(struct Player *player)
{
}
// void superstar_dis(struct Player* player) {
//	int hang, lie;
//	hang = player->row_cnt - 1;
//	lie = player->col_cnt - 1;
//
//
// }
// int is_opponent_killed(int x, int y, struct Player* player) {   //判断被对手杀死
//	int op_x, op_y;
//	op_x = player->opponent_posx;
//	op_y = player->opponent_posy;
//	if (player->opponent_status > 0) {
//		if (x == op_x && y == op_y)
//			return 1;
//	}
//	return 0;
// }
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
    if (x <= hang && x >= 0 && y >= 0 && y <= lie && (player->mat[x][y] == '.' || player->mat[x][y] == 'o' || player->mat[x][y] == 'O') && (x != gx1 || y != gy1) && (x != gx2 || y != gy2))
        return 1;
    return 0;
}
// void star_opDis(struct Player* player, struct PW* op_pos, int ex, int ey, int op_x, int op_y, int vis[][20], int dis) { //bfs搜索对手距离星星的最短距离
//	if (op_pos->flag == 1)return;
//	if (ex == op_x && ey == op_y) {
//		op_pos->flag = 1;
//		op_pos->d = dis;
//		return;
//	}
//	if (vis[op_x][op_y])return;
//	vis[op_x][op_y] = 1;
//	//标记访问
//	int xx, yy;
//	for (int i = 0; i < 4; i++) {
//		xx = op_x + step[i][0];
//		yy = op_y + step[i][1];
//		if (is_valid(player, xx, yy))
//		{
//			//star_opDis(player, , ex, ey, xx, yy, vis);
//		}
//	}
//
// }
void star_Dis(struct Player *player, int x, int y, struct PW *pos, int dis, int vis[][20], char ch)
{ // bfs搜索和最短距离星星的位置
    if (pos->flag == 1)
        return;
    if (player->mat[x][y] == ch)
    {
        //	int op_vis[20][20] = { 0 };
        //	struct PW op_pos;
        //	op_pos.p.X = x;
        //	op_pos.p.Y = y;
        //	star_opDis(player, &op_pos, x, y, player->opponent_posx, player->opponent_posy, op_vis, 0);
        // if (op_pos.d >= dis) {
        // 计算对手和自己离星星的距离
        pos->d = dis;
        pos->flag = 1;
        return;
        //}
    }
    if (vis[x][y] == 1)
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
            star_Dis(player, xx, yy, pos, dis + 1, vis, ch);
        }
    }
}
struct Point walk(struct Player *player)
{
    // This function will be executed in each round.
    struct Point ret = {player->your_posx, player->your_posy};
    // int can_eat[20][20] = { 0 };//0表示可以吃 -1表示不能吃
    int xx, yy;
    int min_dis = 100000;
    struct PW pos = {ret, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        int vis[20][20] = {0}; // bfs搜索标记
        xx = ret.X + step[i][0];
        yy = ret.Y + step[i][1];
        pos.flag = 0;
        if (is_valid(player, xx, yy))
        {
            star_Dis(player, xx, yy, &pos, 1, vis, 'o');
        }
        if (pos.d < min_dis)
        {
            ret.X = xx;
            ret.Y = yy;
            min_dis = pos.d;
        }
    }
    return ret;
}