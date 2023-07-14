#include <string.h>
#include "../include/playerbase.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

bool st[1000][1000];
bool flag;
int dir[5][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, 0}};
typedef struct
{
    int x;
    int y;
    int step;
}bu;

bu b[10000];

int dismov(struct Player *p)    //鬼在我们的斜角上时，保持不动，比较好逃出死胡同
{
    if((abs(p->your_posx - p->ghost_posx[0]) == 1 && abs(p->your_posy - p->ghost_posy[0]) == 1) ||
       (abs(p->your_posx - p->ghost_posx[1]) == 1 && abs(p->your_posy - p->ghost_posy[1]) == 1))
        return 1;
    else
        return 0;
}

int flee(struct Player *p)      //鬼离我们只有一步时 要不要扩展成两步？
{
    if((abs(p->your_posx - p->ghost_posx[0]) <= 1 && abs(p->your_posy - p->ghost_posy[0]) == 0) ||
       (abs(p->your_posx - p->ghost_posx[0]) == 0 && abs(p->your_posy - p->ghost_posy[0]) <= 1))
        return 1;
    else if((abs(p->your_posx - p->ghost_posx[1]) <= 1 && abs(p->your_posy - p->ghost_posy[1]) == 0) ||
            (abs(p->your_posx - p->ghost_posx[1]) == 0 && abs(p->your_posy - p->ghost_posy[1]) <= 1))
        return 2;
    else 
        return 0;
}

int in(struct Player *t,int x,int y)    //在地图里
{
    return x >= 0 && y >= 0 && x < t->row_cnt && y < t->col_cnt;
}

void init(struct Player *player)
{
    // 这个函数将在每场游戏开始时执行，仅执行一次。
}


int find_O(struct Player *player,int x,int y)    //用bfs搜最近的O，返回离他的距离
{   
    int r = 0, l = 0;
    memset(st,0,sizeof(st));
    st[x][y] = 1;
    bu p = {x , y, 0};
    b[r++] = p;
    while(r > l)
    {
        bu now = b[l++];
        if(in(player, now.x, now.y) &&
           player->mat[now.x][now.y] == 'O')
            return now.step;
        for(int i = 0;i < 4;i++)
        {
            int tx = now.x + dir[i][0];
            int ty = now.y + dir[i][1];
            if(in(player, tx, ty) && player->mat[tx][ty] != '#' && st[tx][ty] == 0)
            {
                st[tx][ty] = 1;
                bu tem = {tx, ty, now.step + 1};
                b[r++] = tem;
            }
        }
    }
    return -1;
}

int find_o(struct Player *player,int x,int y)    //用bfs搜最近的o，返回离他的距离
{   
    int r = 0, l = 0;
    memset(st,0,sizeof(st));
    st[x][y] = 1;
    bu p = {x , y, 0};
    b[r++] = p;
    while(r > l)
    {
        bu now = b[l++];
        if(in(player, now.x, now.y) &&
           player->mat[now.x][now.y] == 'o')
            return now.step;
        for(int i = 0;i < 4;i++)
        {
            int tx = now.x + dir[i][0];
            int ty = now.y + dir[i][1];
            if(in(player, tx, ty) && player->mat[tx][ty] != '#' && st[tx][ty] == 0)
            {
                st[tx][ty] = 1;
                bu tem = {tx, ty, now.step + 1};
                b[r++] = tem;
            }
        }
    }
    return -1;
}
        
int find_g0(struct Player *player,int x,int y)     //用bfs搜最近的鬼1，返回离他的距离
{   
    int r = 0, l = 0;
    memset(st,0,sizeof(st));
    st[x][y] = 1;
    bu p = {x , y, 0};
    b[r++] = p;
    while(r > l)
    {
        bu now = b[l++];
        if(in(player, now.x, now.y) &&
           now.x == player->ghost_posx[0] &&
           now.y == player->ghost_posy[0])
            return now.step;
        for(int i = 0;i < 4;i++)
        {
            int tx = now.x + dir[i][0];
            int ty = now.y + dir[i][1];
            if(in(player, tx, ty) && player->mat[tx][ty] != '#' && st[tx][ty] == 0)
            {
                st[tx][ty] = 1;
                bu tem = {tx, ty, now.step + 1};
                b[r++] = tem;
            }
        }
    }
    return -1;
}

int find_g1(struct Player *player,int x,int y)      //用bfs搜最近的鬼2，返回离他的距离
{   
    int r = 0, l = 0;
    memset(st,0,sizeof(st));
    st[x][y] = 1;
    bu p = {x , y, 0};
    b[r++] = p;
    while(r > l)
    {
        bu now = b[l++];
        if(in(player, now.x, now.y) &&
           now.x == player->ghost_posx[1] &&
           now.y == player->ghost_posy[1])
            return now.step;
        for(int i = 0;i < 4;i++)
        {
            int tx = now.x + dir[i][0];
            int ty = now.y + dir[i][1];
            if(in(player, tx, ty) && player->mat[tx][ty] != '#' && st[tx][ty] == 0)
            {
                st[tx][ty] = 1;
                bu tem = {tx, ty, now.step + 1};
                b[r++] = tem;
            }
        }
    }
    return -1;
}           



struct Point walk(struct Player *player)
{
    int min_O = 100000;
    int min_o = 100000;
    int max_g0 = -1;
    int max_g1 = -1;
    int Oflag = 1;
    int x = player->your_posx;
    int y = player->your_posy;
    struct Point ret;
    int d = 5;

    if(flee(player)==1 && player->your_status <= 2)
    {
        d = 5;
        int max_g0 = -1;
        for(int i = 0;i < 4;i++)
        {
            int tx = x + dir[i][0];
            int ty = y + dir[i][1];
            if(in(player,tx,ty) && player->mat[tx][ty] != '#')
            {
                int dis = find_g0(player, tx, ty);
                if(dis > max_g0)
                {
                    max_g0 = dis;
                    d = i;
                }
            }
        }
        ret = {x + dir[d][0], y + dir[d][1]};
        return ret;
    }
    else if(flee(player) == 2 && player->your_status <= 2)
    {
        int max_g1 = -1;
        d = 5;
        for(int i = 0;i < 4;i++)
        {
            int tx = x + dir[i][0];
            int ty = y + dir[i][1];
            if(in(player,tx,ty) && player->mat[tx][ty] != '#')
            {
                int dis = find_g1(player, tx, ty);
                if(dis > max_g1)
                {
                    max_g1 = dis;
                    d = i;
                }
            }
        }
        ret = {x + dir[d][0], y + dir[d][1]};
        return ret;
    }
    
    
    if(player->your_status >= 2)  //强化状态去吃鬼
    {
        int min_g = 100000;
        d = 5;
        for(int i = 0;i < 4;i++)
        {
            int tx = x + dir[i][0];
            int ty = y + dir[i][1];
            if(in(player,tx,ty) && player->mat[tx][ty] != '#')
            {
                int dis = find_g0(player, tx, ty);
                if(dis == -1)
                    continue;
                if(dis < min_g)
                {
                    min_g = dis;
                    d = i;
                }
            }
        }
        for(int i = 0;i < 4;i++)
        {
            int tx = x + dir[i][0];
            int ty = y + dir[i][1];
            if(in(player,tx,ty) && player->mat[tx][ty] != '#')
            {
                int dis = find_g1(player, tx, ty);
                if(dis == -1)
                    continue;
                if(dis < min_g)
                {
                    min_g = dis;
                    d = i;
                }
            }
        }
        if(min_g*1.1 < player->your_status)     //强化状态够吃到鬼时去吃，*1.4是因为鬼也会朝我们移动
        {
            ret = {x + dir[d][0], y + dir[d][1]};
            return ret;
        }
    }
   
    // if(dismov(player) && player->your_status == 0)
    // {
    //     ret = {x , y};
    //     return ret;
    // }
    
    d = 5;
    for (int k = 0; k < 4; k++)
    {
        int tx = x + dir[k][0];
        int ty = y + dir[k][1];
        if( in(player,tx,ty) && player->mat[tx][ty] != '#' )
        {
            int dis = find_O(player, tx, ty);
            if(dis == -1)
            {
                Oflag = 0;
                break;
            }
            else if(dis < min_O )
            {
                min_O = dis;
                d = k;
            }
        }
    }
    if (Oflag) {
        ret = {x + dir[d][0], y + dir[d][1]};
        return ret;
    }

    d = 5;
    for (int k = 0; k < 4; k++)
    {
        int tx = x + dir[k][0];
        int ty = y + dir[k][1];
        if( in(player,tx,ty) && player->mat[tx][ty] != '#' )
        {
            int dis = find_o(player, tx, ty);
            if(dis == -1)
            {
                ret = {x , y};
                return ret;
            }
            else if(dis < min_o )
            {
                min_o = dis;
                d = k;
            }
        }
    }
    ret = {x + dir[d][0], y + dir[d][1]};
    return ret;
}

