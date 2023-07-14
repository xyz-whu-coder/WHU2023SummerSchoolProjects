#include <string.h>
#include <math.h>
#include <stdio.h>
#include "../include/playerbase.h"
#define up 0
#define down 1
#define left 2
#define right 3
#define upperleft 4
#define upperright 5
#define downleft 6
#define downright 7
struct move
{
    int x, y;
    int nev;
    int before;
};
struct move move[1000000];
int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
char vis[100][100];
int ghostDist[2];
struct superstar
{
    int X, Y;
    int dis;
} superstar;
int opponentDist;
// 得到其他单位相对与自己的方向
int orientation(struct Player *player, int x, int y)
{
    x = player->your_posx - x;
    y = player->your_posy - y;
    if (x == 0 && y > 0)
    {
        return up;
    }
    else if (x == 0 && y < 0)
    {
        return down;
    }
    else if (x > 0 && y == 0)
    {
        return left;
    }
    else if (x < 0 && y == 0)
    {
        return right;
    }
    else if (x > 0 && y > 0)
    {
        return upperleft;
    }
    else if (x > 0 && y < 0)
    {
        return upperright;
    }
    else if (x < 0 && y > 0)
    {
        return downleft;
    }
    else if (x < 0 && y < 0)
    {
        return downright;
    }
}
// 计算出自己到其他单位的距离
void dis(struct Player *player)
{
    // 计算鬼的位置
    for (int i = 0; i < 2; i++)
    {
        int x = player->ghost_posx[i];
        int y = player->ghost_posy[i];
        ghostDist[i] = abs(x - player->your_posx) + abs(y - player->your_posy);
    }
    
    superstar.dis = 1000000;
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[i][j] == 'O')
            {
                if (abs(i - player->your_posx) + abs(j - player->your_posy) < superstar.dis && orientation(player, i, j) != orientation(player, player->ghost_posx[0], player->ghost_posy[0]) // 不要走到鬼身上
                    && orientation(player, i, j) != orientation(player, player->ghost_posx[1], player->ghost_posy[1]))
                {
                    // fprintf(stderr,"superstar.X=%d superstar.Y=%d,superstar.dis=%d\n",i,j,superstar.dis);
                    superstar.X = i;
                    superstar.Y = j;
                    superstar.dis = abs(i - player->your_posx) + abs(j - player->your_posy);
                }
            }
        }
    }

    // 计算对手的位置
    opponentDist = abs(player->opponent_posx - player->your_posx) + abs(player->opponent_posy - player->your_posy);
}

void init(struct Player *player)
{
    // This function will be executed at the begin of each game, only once.
}

int getway(struct Player *player, int tail)
{
    if (move[tail].before == 0)
        return move[tail].nev;
    else
        return getway(player, move[tail].before);
}

int bfs(struct Player *player, char flag)
{
    struct move tmp;
    tmp.x = player->your_posx;
    tmp.y = player->your_posy;
    tmp.nev = -1;
    tmp.before = -1;
    int head = 0, tail = 0;
    move[tail++] = tmp;
    memset(vis, 0, sizeof(vis));
    vis[tmp.x][tmp.y] = 1;
    while (head < tail)
    {
        tmp = move[head++];
        for (int i = 0; i < 4; i++)
        {
            int nx = tmp.x + dir[i][0];
            int ny = tmp.y + dir[i][1];
            if (nx < 0 || nx >= player->row_cnt || ny < 0 || ny >= player->col_cnt || vis[nx][ny] == 1 || player->mat[nx][ny] == '#' || nx == player->ghost_posx[0] && ny == player->ghost_posy[0] || nx == player->ghost_posx[1] && ny == player->ghost_posy[1])
                continue;
            vis[nx][ny] = 1;
            move[tail].x = nx;
            move[tail].y = ny;
            move[tail].nev = i;           // 上一步时从什么方向走过来的
            move[tail].before = head - 1; // 上一步的编号
            tail++;
            if (player->mat[nx][ny] == 'O' || player->mat[nx][ny] == flag)
            {
                return getway(player, tail - 1);
            }
        }
    }
    return -1;
}
int bfs2(struct Player *player)
{
    struct move tmp;
    tmp.x = player->your_posx;
    tmp.y = player->your_posy;
    tmp.nev = -1;
    tmp.before = -1;
    int head = 0, tail = 0;
    move[tail++] = tmp;
    memset(vis, 0, sizeof(vis));
    vis[tmp.x][tmp.y] = 1;
    while (head < tail)
    {
        tmp = move[head++];
        for (int i = 0; i < 4; i++)
        {
            int nx = tmp.x + dir[i][0];
            int ny = tmp.y + dir[i][1];
            if (nx < 0 || nx >= player->row_cnt || ny < 0 || ny >= player->col_cnt || vis[nx][ny] == 1 || player->mat[nx][ny] == '#')
                continue;
            vis[nx][ny] = 1;
            move[tail].x = nx;
            move[tail].y = ny;
            move[tail].nev = i;           // 上一步时从什么方向走过来的
            move[tail].before = head - 1; // 上一步的编号
            tail++;
            if (nx == player->ghost_posx[0] && ny == player->ghost_posy[0] || nx == player->ghost_posx[1] && ny == player->ghost_posy[1])
            {
                return getway(player, tail - 1);
            }
        }
    }
    return -1;
}
struct Point superstep(struct Player *player)
{
    switch (bfs(player, 'O'))
    {
    case 0:
        return initPoint(player->your_posx + 1, player->your_posy);
    case 1:
        return initPoint(player->your_posx - 1, player->your_posy);
    case 2:
        return initPoint(player->your_posx, player->your_posy + 1);
    case 3:
        return initPoint(player->your_posx, player->your_posy - 1);
    default:
        return initPoint(-1, -1);
    }
}
struct Point normalstep(struct Player *player)
{
    switch (bfs(player, 'o'))
    {
    case 0:
        return initPoint(player->your_posx + 1, player->your_posy);
    case 1:
        return initPoint(player->your_posx - 1, player->your_posy);
    case 2:
        return initPoint(player->your_posx, player->your_posy + 1);
    case 3:
        return initPoint(player->your_posx, player->your_posy - 1);
    default:
        return initPoint(player->your_posx, player->your_posy);
    }
}
struct Point catchghost(struct Player *player)
{
    switch (bfs2(player))
    {
    case 0:
        return initPoint(player->your_posx + 1, player->your_posy);
    case 1:
        return initPoint(player->your_posx - 1, player->your_posy);
    case 2:
        return initPoint(player->your_posx, player->your_posy + 1);
    case 3:
        return initPoint(player->your_posx, player->your_posy - 1);
    default:
        return initPoint(-1, -1);
    }
}
struct Point walk(struct Player *player)
{
    dis(player);
    // This function will be executed in each round.
    struct Point ret = {player->your_posx, player->your_posy};
    if (ghostDist[0] < player->your_status || ghostDist[1] < player->your_status)
    {
        ret = catchghost(player);
        if (ret.X != -1 && ret.Y != -1)
            return ret;
    }
    if (superstar.dis < 100)
    {
        // fprintf(stderr,"superstar=%d\n",superstar.dis);
        ret = superstep(player);
        if (ret.X != -1 && ret.Y != -1)
            return ret;
    }

    ret = normalstep(player);

    return ret;
}
