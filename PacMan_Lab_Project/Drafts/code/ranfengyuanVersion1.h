#include <stdlib.h>
#include <string.h>
#include <queue>
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
int visited[MAPSIZE][MAPSIZE];
int value[MAPSIZE][MAPSIZE];
struct Point *stack;
int top;

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
    return isInside(player, x, y) && !isWall(player, x, y) && !isMeetingGhost(player, x, y);
}

int max_value[4]; // 0 为下 1为左 2为上 3为右
struct Point nearStar(struct Player *player, struct Point ret)
{
    init(player);
    std::queue<struct Point> q;
    memset(max_value, 0, sizeof max_value);
    for (int i = 0; i < 4; i++)
    {
        int m = 0;
        int tx = ret.X + step[i][0];
        int ty = ret.Y + step[i][1];
        memset(visited, 0, sizeof visited);
        if (!isValid(player, tx, ty))
            continue;
        q.push({tx, ty});

        visited[tx][ty] = 1;
        while (q.size() && m < 25)
        {
            struct Point cur = q.front();
            q.pop();
            for (int j = 0; j < 4; j++)
            {
                int txx = cur.X + step[j][0];
                int tyy = cur.Y + step[j][1];
                if (!isValid(player, txx, tyy))
                {
                    continue;
                }
                if (player->your_status > 0)
                {
                    value[player->ghost_posx[0]][player->ghost_posy[0]] =
                        value[player->ghost_posx[1]][player->ghost_posy[1]] = 200;
                }
                else
                {
                    value[player->ghost_posx[0]][player->ghost_posy[0]] =
                        value[player->ghost_posx[1]][player->ghost_posy[1]] = -100;
                }
                max_value[i] += value[txx][tyy];
                visited[txx][tyy] = 1;
                value[txx][tyy] = -5;
                q.push(cur);
                m++;
            }
        }
    }
    int j = 0;
    int max = max_value[0];
    for (int i = 1; i < 4; i++)
    {
        if (max < max_value[i])
        {
            max = max_value[i];
            j = i;
        }
    }
    struct Point res = {ret.X + step[j][0], ret.Y + step[j][1]};
    return res;
}

void init(struct Player *player)
{
    // This function will be executed at the begin of each game, only once.
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[i][j] == WALL)
            {
                value[i][j] = -500;
            }
            else if (player->mat[i][j] == NORMAL_STAR)
            {
                value[i][j] = 10;
            }
            else if (player->mat[i][j] == SUPER_STAR)
            {
                value[i][j] = 50;
            }
        }
    }
}

struct Point walk(struct Player *player)
{
    // This function will be executed in each round.
    struct Point ret = {player->your_posx, player->your_posy};
    ret = nearStar(player, ret);
    player->your_posx = ret.X, player->your_posy = ret.Y;
    return ret;
}