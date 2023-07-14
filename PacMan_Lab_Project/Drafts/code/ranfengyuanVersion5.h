#include <stdlib.h>
#include <string.h>
#include "../include/playerbase.h"
#include <time.h>
#include <stdbool.h>

#define WALL '#'
#define NORMAL_STAR 'o'
#define SUPER_STAR 'O'
#define GROUND '.'
#define MAPSIZE 100
int map[MAPSIZE][MAPSIZE];
int step[4][2] = {
    {1, 0}, {0, -1}, {-1, 0}, {0, 1}};

bool isInside(struct Player *player, int x, int y)
{
    return x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt;
}

bool isMeetingGhost(struct Player *player, int x, int y)
{
    return (x == player->ghost_posx[0] && y == player->ghost_posy[0]) || (x == player->ghost_posx[1] && x == player->ghost_posy[1]);
}
bool isWall(struct Player *player, int x, int y)
{
    return player->mat[x][y] == WALL;
}

bool isNormalStar(struct Player *player, int x, int y)
{
    return player->mat[x][y] == NORMAL_STAR;
}

bool isSuperStar(struct Player *player, int x, int y)
{
    return player->mat[x][y] == SUPER_STAR;
}

bool isValid(struct Player *player, int x, int y)
{
    return isInside(player, x, y) && !isWall(player, x, y);
}

bool isMeetingOpponent(struct Player *player, int x, int y)
{
    return x == player->opponent_posx && y == player->opponent_posy && player->opponent_score >= 350 && player->opponent_status == 0;
}
int min_dis;
int vis[100][100];
int flag;
void nearSuperStar(struct Player *player, int x, int y, int dis)
{
    if (dis > 25)
        return;
    if (min_dis < dis || !isValid(player, x, y) || vis[x][y] || isMeetingGhost(player, x, y))
        return;

    if (player->mat[x][y] == SUPER_STAR)
    {
        min_dis = dis;
        return;
    }
    vis[x][y] = 1;
    nearSuperStar(player, x, y - 1, dis + 1);
    nearSuperStar(player, x, y + 1, dis + 1);
    nearSuperStar(player, x - 1, y, dis + 1);
    nearSuperStar(player, x + 1, y, dis + 1);
    vis[x][y] = 0;
    return;
}
void nearNormalStar(struct Player *player, int x, int y, int dis)
{
    if (min_dis < dis || !isValid(player, x, y) || vis[x][y])
        return;
    if (player->your_status == 0 && isMeetingGhost(player, x, y))
    {
        return;
    }
    else if (player->mat[x][y] == NORMAL_STAR)
    {
        min_dis = dis;
        return;
    }
    vis[x][y] = 1;
    nearNormalStar(player, x + 1, y, dis + 1);
    nearNormalStar(player, x, y + 1, dis + 1);
    nearNormalStar(player, x - 1, y, dis + 1);
    nearNormalStar(player, x, y - 1, dis + 1);
    vis[x][y] = 0;
    return;
}
int MIN(int a, int b)
{
    return a < b ? a : b;
}
void init(struct Player *player)
{
    // This function will be executed at the begin of each game, only once.
}
void nearVector(struct Player *player, int x, int y, int dis)
{

    if (isMeetingOpponent(player, x, y))
    {
        min_dis = dis;
        return;
    }
    else if (isMeetingGhost(player, x, y))
    {
        min_dis = dis;
        return;
    }
    vis[x][y] = 1;
    nearVector(player, x + 1, y, dis + 1);
    nearVector(player, x, y + 1, dis + 1);
    nearVector(player, x - 1, y, dis + 1);
    nearVector(player, x, y - 1, dis + 1);
    vis[x][y] = 0;
    return;
}
struct Point walk(struct Player *player)
{
    // This function will be executed in each round.
    struct Point ret = {player->your_posx, player->your_posy};
    memset(vis, 0, sizeof vis);
    int min = 10000;
    int j = 0;
    min_dis = 10000;
    flag = 0; // 四个方向只要有一个找到就可以了  所以放在外面
    memset(vis, 0, sizeof vis);
    int row_max = MIN(ret.X + 10, player->row_cnt);
    int col_max = MIN(player->col_cnt, ret.Y + 10);
    for (int i = 0; i < row_max; i++)
    {
        for (int j = 0; j < col_max; j++)
        {
            if (player->mat[i][j] == 'O')
            {
                flag = 1;
            }
        }
    }
    if (flag == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            if (isValid(player, ret.X + step[i][0], ret.Y + step[i][1]))
            {
                nearSuperStar(player, ret.X + step[i][0], ret.Y + step[i][1], 0);
                if (min > min_dis)
                {
                    min = min_dis;
                    j = i;
                }
            }
        }
        ret = {ret.X + step[j][0], ret.Y + step[j][1]};
        player->your_posx = ret.X;
        player->your_posy = ret.Y;
        return ret;
    }
    else if (player->your_status > 10)
    {
        for (int i = 0; i < 4; i++)
        {
            nearVector(player, ret.X + step[i][0], ret.Y + step[i][1], 0);
            if (min > min_dis)
            {
                min = min_dis;
                j = i;
            }
        }
    }
    else if (flag == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (isValid(player, ret.X + step[i][0], ret.Y + step[i][1]))
            {
                nearNormalStar(player, ret.X + step[i][0], ret.Y + step[i][1], 0);
                if (min > min_dis)
                {
                    min = min_dis;
                    j = i;
                }
            }
        }
    }
    ret = {ret.X + step[j][0], ret.Y + step[j][1]};
    player->your_posx = ret.X;
    player->your_posy = ret.Y;
    return ret;
}