#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <math.h>
int ghost[2][2];
int my_pos[2];
int my_status;
int opponent_status;
int enemy_pos[2]; // 敌人位置
int daliwan[4][2];
char arr[100][100];
int row;
int col;
int cnt;
int flag = 0;
int vis[1001][1001];
struct node
{
    int x, y, step;
} q[100010];
struct node prev[100][100];
struct node path[100];
int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

void init(struct Player *player)
{ // 更新地图为都没走过&&检查大力丸的位置&&更新鬼的位置
    // This function will be executed at the begin of each game, only once.
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            vis[i][j] = 0;
        }
    }
}

int in(int x, int y, struct Player *player)
{
    int row = player->row_cnt;
    int col = player->col_cnt;
    return x >= 0 && x < row && y >= 0 && y < col;
}

int bfs_dis(int x, int y, int sx, int sy, struct Player *player) // bfs求出两点间的距离
{
    init(player);
    if (x == sx && y == sy)
    {
        return 0;
    }
    // if(player->mat[sx][sy+1] == '#' && player->mat[sx+1][sy] == '#' && player->mat[sx-1][sy] == '#' && player->mat[sx][sy - 1] == '#')
    // {
    //     return 99999;
    // }
    struct node now = {x, y, 0};
    vis[x][y] = 1;
    int l = 0, r = 0;
    q[r++] = now;
    while (l < r)
    {
        int len = r - l;
        for (int i = 0; i < len; i++)
        {
            now = q[l++];
            vis[now.x][now.y] = 1;
            if (now.x == sx && now.y == sy)
            {
                return now.step;
            }
            for (int i = 0; i < 4; i++)
            {
                int tx = now.x + dir[i][0];
                int ty = now.y + dir[i][1];
                if (in(tx, ty, player) && !vis[tx][ty] && player->mat[tx][ty] != '#')
                {
                    struct node t = {tx, ty, now.step + 1};
                    q[r++] = t;
                    vis[tx][ty] = 1;
                }
            }
        }
    }
    return 999;
}

struct node walk_wan(int x, int y, struct Player *player)
{
    init(player);
    struct node now = {x, y, 0};
    vis[x][y] = 1;
    int l = 0, r = 0;
    q[r++] = now;
    while (l < r)
    {
        now = q[l++];
        vis[now.x][now.y] = 1;
        if (player->mat[now.x][now.y] == 'O')
        {
            return now;
        }
        for (int i = 0; i < 4; i++)
        {
            int tx = now.x + dir[i][0];
            int ty = now.y + dir[i][1];
            if (in(tx, ty, player) && !vis[tx][ty] && player->mat[tx][ty] != '#')
            {
                struct node t = {tx, ty, now.step + 1};
                q[r++] = t;
                vis[tx][ty] = 1;
            }
        }
    }
    return now;
}

struct node walk_nor(int x, int y, struct Player *player)
{
    init(player);
    struct node now = {x, y, 0};
    vis[x][y] = 1;
    int l = 0, r = 0;
    q[r++] = now;
    while (l < r)
    {
        int len = r - l;
        for (int i = 0; i < len; i++)
        {
            now = q[l++];
            vis[now.x][now.y] = 1;
            if (player->mat[now.x][now.y] == 'o')
            {
                return now;
            }
            for (int i = 0; i < 4; i++)
            {
                int tx = now.x + dir[i][0];
                int ty = now.y + dir[i][1];
                if (in(tx, ty, player) && !vis[tx][ty] && player->mat[tx][ty] != '#')
                {
                    struct node t = {tx, ty, now.step + 1};
                    q[r++] = t;
                    vis[tx][ty] = 1;
                }
            }
        }
    }

    return now;
}

struct node chase_ghost(int x, int y, struct Player *player, int gho_x, int gho_y)
{

    int my_dis = bfs_dis(my_pos[0], my_pos[1], gho_x, gho_y, player);
    for (int i = 0; i < 4; i++)
    {
        int tx = my_pos[0] + dir[i][0];
        int ty = my_pos[1] + dir[i][1];
        if (tx == gho_x && ty == gho_y)
        {
            my_pos[0] = tx;
            my_pos[1] = ty;
            struct node ret = {my_pos[0], my_pos[1]};
            return ret;
        }
        if (!in(tx, ty, player))
        {
            continue;
        }
        if (player->mat[tx][ty] != '#')
        {
            int t_dis = bfs_dis(tx, ty, gho_x, gho_y, player);
            int dif = my_dis - t_dis;
            if (dif == 1)
            {
                my_pos[0] = tx;
                my_pos[1] = ty;
                struct node ret = {my_pos[0], my_pos[1]};
                return ret;
            }
        }
    }
    struct node ret = {-1, -1, -1};
    return ret;
}

struct Point walk(struct Player *player)
{
    // This function will be executed in each round.
    int row = player->row_cnt;
    int col = player->col_cnt;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (player->mat[i][j] == 'O')
            {
                daliwan[cnt][0] = i;
                daliwan[cnt][1] = j;
                cnt++;
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        ghost[i][0] = player->ghost_posx[i];
        ghost[i][1] = player->ghost_posy[i];
    }
    // printf("%d %d\n", cnt, flag);
    int index = -1;
    init(player);
    // printf("%d\n", cnt);
    my_pos[0] = player->your_posx;
    my_pos[1] = player->your_posy;
    // printf("%d %d\n", my_pos[0], my_pos[1]);
    enemy_pos[0] = player->opponent_posx;
    enemy_pos[1] = player->opponent_posy;
    my_status = player->your_status;
    opponent_status = player->opponent_status;
    int gho_x, gho_y, min;
    int dis1 = bfs_dis(my_pos[0], my_pos[1], ghost[0][0], ghost[0][1], player);
    int dis2 = bfs_dis(my_pos[0], my_pos[1], ghost[1][0], ghost[1][1], player);
    if (dis1 > dis2)
    {
        gho_x = ghost[1][0];
        gho_y = ghost[1][1];
        min = dis2;
    }
    else
    {
        gho_x = ghost[0][0];
        gho_y = ghost[0][1];
        min = dis1;
    }
    int dis_g = bfs_dis(my_pos[0], my_pos[1], enemy_pos[0], enemy_pos[1], player);
    if (min <= 2 && !my_status)
    {
        int my_dis = bfs_dis(my_pos[0], my_pos[1], gho_x, gho_y, player);
        for (int i = 0; i < 4; i++)
        {
            int tx = my_pos[0] + dir[i][0];
            int ty = my_pos[1] + dir[i][1];
            if (!in(tx, ty, player))
            {
                continue;
            }
            if (player->mat[tx][ty] != '#')
            {
                int t_dis = bfs_dis(tx, ty, gho_x, gho_y, player);
                int dif = my_dis - t_dis;
                if (dif == -1)
                {
                    my_pos[0] = tx;
                    my_pos[1] = ty;
                    struct Point ret = {my_pos[0], my_pos[1]};
                    return ret;
                }
            }
        }
    }
    if (dis_g <= 2 && opponent_status)
    {
        int my_dis = bfs_dis(my_pos[0], my_pos[1], enemy_pos[0], enemy_pos[1], player);
        for (int i = 0; i < 4; i++)
        {
            int tx = my_pos[0] + dir[i][0];
            int ty = my_pos[1] + dir[i][1];
            if (!in(tx, ty, player))
            {
                continue;
            }
            if (player->mat[tx][ty] != '#')
            {
                int t_dis = bfs_dis(tx, ty, enemy_pos[0], enemy_pos[1], player);
                int dif = my_dis - t_dis;
                if (dif == -1)
                {
                    my_pos[0] = tx;
                    my_pos[1] = ty;
                    struct Point ret = {my_pos[0], my_pos[1]};
                    return ret;
                }
            }
        }
    }
    if (cnt > 0 && !my_status)
    {
        cnt = 0;
        struct node target = walk_wan(my_pos[0], my_pos[1], player);
        int my_dis = bfs_dis(my_pos[0], my_pos[1], target.x, target.y, player);
        for (int i = 0; i < 4; i++)
        {
            int tx = my_pos[0] + dir[i][0];
            int ty = my_pos[1] + dir[i][1];
            if (!in(tx, ty, player))
            {
                continue;
            }
            if (player->mat[tx][ty] != '#')
            {
                int t_dis = bfs_dis(tx, ty, target.x, target.y, player);
                int dif = my_dis - t_dis;
                if (dif == 1)
                {
                    my_pos[0] = tx;
                    my_pos[1] = ty;
                    struct Point ret = {my_pos[0], my_pos[1]};
                    return ret;
                }
            }
        }
    }
    else if (my_status && min < 15)
    {
        struct node now = chase_ghost(my_pos[0], my_pos[1], player, gho_x, gho_y);
        struct Point ret = {now.x, now.y};
        return ret;
    }
    else
    {

        struct node target = walk_nor(my_pos[0], my_pos[1], player);
        int my_dis = bfs_dis(my_pos[0], my_pos[1], target.x, target.y, player);
        for (int i = 0; i < 4; i++)
        {
            int tx = my_pos[0] + dir[i][0];
            int ty = my_pos[1] + dir[i][1];
            if (!in(tx, ty, player))
            {
                continue;
            }
            if (player->mat[tx][ty] != '#')
            {
                int t_dis = bfs_dis(tx, ty, target.x, target.y, player);
                int dif = my_dis - t_dis;
                if (dif == 1)
                {
                    my_pos[0] = tx;
                    my_pos[1] = ty;
                    struct Point ret = {my_pos[0], my_pos[1]};
                    return ret;
                }
            }
        }
    }
    struct Point ret = {my_pos[0], my_pos[1]};
    return ret;
}
