#include <string.h>
#include <stdlib.h>
#include "../include/playerbase.h"
int dir[4][2] = { {1, 0},{-1, 0},{0, 1}, {0, -1}};
struct Queue {
    int X;
    int Y;
    int last;
}q[1000];
void init(struct Player* player) {
    // This function will be executed at the begin of each game, only once.
}
int is_valid(struct Player* player, int posx, int posy) {
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt) {
        return 0;
    }
    else if (player->mat[posx][posy] == '#') {
        return 0;
    }
    else if ((posx == player->ghost_posx[0] && posy == player->ghost_posy[0])||(posx == player->ghost_posx[1] && posy == player->ghost_posy[1])){
        return 0;
    }
    else if ((posx+1 == player->ghost_posx[0] && posy == player->ghost_posy[0])||(posx-1 == player->ghost_posx[0] && posy == player->ghost_posy[0])||(posx == player->ghost_posx[0] && posy+1 == player->ghost_posy[0])||(posx == player->ghost_posx[0] && posy-1 == player->ghost_posy[0])){
        return 0;
    }
    else if ((posx+1 == player->ghost_posx[1] && posy == player->ghost_posy[1])||(posx-1 == player->ghost_posx[1] && posy == player->ghost_posy[1])||(posx == player->ghost_posx[1] && posy+1 == player->ghost_posy[1])||(posx == player->ghost_posx[1] && posy-1 == player->ghost_posy[1])){//判断一下走完这个点之后，下一个点附近有没有鬼
        return 0;
    }
    return 1;
}
int calculate_score(struct Player* player, int posx, int posy) {
    int score = 0;
    if ((player->mat[posx][posy] == 'o' || player->mat[posx][posy] == 'O') && is_valid(player, posx, posy)) {
        score += 10;
    }
    return score;
}
int bfs(struct Player* player,int posx,int posy){
    int front = 0, rear = 0; // 队列的前后指针
    struct Queue t = {player->your_posx, player->your_posy, 0};
    q[rear++]=t;
    while (front < rear) {
        struct Queue cur = q[front++];
        for (int i = 0; i < 4; i++) {
            if (calculate_score(player, cur.X, cur.Y) == 10) {
                if(is_valid(player, cur.X, cur.Y)){
                    return (front - 1);
                }
            /*player->your_posx = tx;
            player->your_posy = ty;
            visited[cur.X][cur.Y] = 0;
            struct Point ret = { player->your_posx , player->your_posy };*/
            }
            int tx = cur.X + dir[i][0];
            int ty = cur.Y + dir[i][1];
            if (is_valid(player, tx, ty)) {
                struct Queue t = {tx, ty, front-1};
                q[rear++]=t;
            }
        }
    }
    return -1;
}
int tmp = 0;
int num[1000];
struct Point walk(struct Player* player) {
    struct Point ret = {player->your_posx, player->your_posy};
    if (tmp == 0) {   
		int rear = bfs(player, player->your_posx, player->your_posy);
		for (int i = rear; i; i = q[i].last)
		{
			num[tmp] = i;
            tmp++;
		}
        tmp--;
		ret.X = q[num[tmp]].X;
		ret.Y = q[num[tmp]].Y;
	}
	else
	{
        tmp--;
		ret.X = q[num[tmp]].X;
		ret.Y = q[num[tmp]].Y;
	}
    return ret;
}
