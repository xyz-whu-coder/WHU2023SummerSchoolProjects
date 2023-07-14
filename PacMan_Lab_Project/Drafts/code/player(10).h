#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/playerbase.h"
#define INF 0x3f3f3f3f
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
void init(struct Player *player);
int in(int x, int y);
void Path(struct Player *player);
void avoid(struct Player *player);
int symbol(int x);
struct Point em_avoid(struct Player *player);
struct Point chase(struct Player *player);
int alley(struct Player *player, int x, int y);
struct Point suicide(void);
struct Point rush(struct Player *player, struct Point point);

int row, col;
struct Point *stack;	 
int top;
int visit[60][60];
int vis[60][60];
int alley_visit[60][60];
int ave_dis;
float ave_weight; 
int super_dis;
float super_weight; 
int oppo_dis;
float oppo_weight;
int ghost1_dis;
int ghost2_dis; 
float ghost_weight;
int rad[4][2] = {{-1,0},{0,-1},{0,1},{1,0}};
int died[60][60]; 
int is_alley[60][60];
int value[60][60];
int dig_step;
int rushing;
struct Point back;
int died2[60][60];
float died_work;
int did_wait;

void init(struct Player *player) {
	row = player->row_cnt;
	col = player->col_cnt;
	stack = (struct Point*)malloc(row*col*sizeof(struct Point));
	top = -1;
	ave_weight = 1;
	super_weight = 20;
	oppo_weight = 10;
	ghost_weight = 500;
	memset(is_alley,0,sizeof(is_alley));
	dig_step = -1;
}

struct Point suicide() {
	struct Point point = {-1,-1};
	return point;
} 

struct Point rush(struct Player *player, struct Point point) {
	struct Point *queue = (struct Point*)malloc(row*col*sizeof(struct Point));
	int *pre = (int*)malloc(row*col*sizeof(int));
	memset(vis,0,sizeof(vis));
	int ql = 0, qr = 0;
	vis[player->your_posx][player->your_posy] = 1;
	pre[qr] = -1;
	queue[qr].X = player->your_posx;
	queue[qr++].Y = player->your_posy;
	while(ql < qr) {
		int x = queue[ql].X;
		int y = queue[ql++].Y;
		if(point.X == x && point.Y == y)	break;
		for(int i = 0; i < 4; i++) {
			int nx = x + rad[i][0];
			int ny = y + rad[i][1];
			if(!in(nx,ny) || vis[nx][ny] || player->mat[nx][ny] == '#')	continue;
			vis[nx][ny] = 1;
			pre[qr] = ql - 1;
			queue[qr].X = nx;
			queue[qr++].Y = ny;
		}
	}
	int i;
	for(i = ql-1; pre[i]; i = pre[i]);
	struct Point ans = queue[i];
	free(queue);
	free(pre);
	return ans;
}

int alley(struct Player *player, int fx, int fy) {
	if(player->opponent_status == -1)	return 0;
	memset(alley_visit,0,sizeof(alley_visit));
	struct Point *queue = (struct Point*)malloc(row*col*sizeof(struct Point));
	int *pre = (int*)malloc(row*col*sizeof(int));
	int *depth = (int*)malloc(row*col*sizeof(int));
	int ql=1, qr=0;
	pre[qr] = -1;
	depth[qr] = 0;
	alley_visit[player->your_posx][player->your_posy] = 1;
	queue[qr].X = player->your_posx;
	queue[qr++].Y = player->your_posy;
	pre[qr] = 0;
	depth[qr] = 1;
	queue[qr].X = fx;
	queue[qr++].Y = fy;
	alley_visit[fx][fy] = 1;
	while(ql < qr) {
		int x = queue[ql].X;
		int y = queue[ql++].Y;
		for(int i = 0; i < 4; i++) {
			int nx = x + rad[i][0];
			int ny = y + rad[i][1];
			if(!in(nx,ny) || player->mat[nx][ny] == '#' 
			|| nx == queue[pre[ql-1]].X&&ny == queue[pre[ql-1]].Y)	
				continue;
			if(alley_visit[nx][ny]) {
				free(queue);
				free(pre);
				free(depth);
				return 0;
			}
			if(player->mat[nx][ny] == 'O') {
				free(queue);
				free(pre);
				free(depth);
				return 0;
			}
			pre[qr] = ql-1;
			depth[qr] = depth[ql-1] + 1;
			queue[qr].X = nx;
			queue[qr++].Y = ny;
			alley_visit[nx][ny] = 1;
		}
	}
	int tmp = depth[qr-1], i;
	for(i = 1; i < qr-1; i++)
		if(player->mat[queue[i].X][queue[i].Y] == 'o' || player->mat[queue[i].X][queue[i].Y] == 'O')
			break;
	value[fx][fy] = depth[i];
	free(queue);
	free(pre);
	free(depth);
	return tmp;
}

int symbol(int x) {
	if(!x)	return 0;
	return x/abs(x);
}

int in(int x, int y) {
	if(x < 0 || x > row-1 || y < 0 || y > col-1)	return 0;
	return 1;
}

void avoid(struct Player *player) {
	memset(died,0,sizeof(died));
	memset(died2,0,sizeof(died2));
	int flag1 = ghost1_dis <= 2, flag2 = ghost2_dis <= 2, flag3 = oppo_dis <= 2 && player->opponent_status > 1;
	if(flag1) {
		died[player->ghost_posx[0]][player->ghost_posy[0]] = died2[player->ghost_posx[0]][player->ghost_posy[0]] = 1;
		died[player->ghost_posx[0]-1][player->ghost_posy[0]] = 1;
		died[player->ghost_posx[0]][player->ghost_posy[0]-1] = 1;
		died[player->ghost_posx[0]+1][player->ghost_posy[0]] = 1;
		died[player->ghost_posx[0]][player->ghost_posy[0]+1] = 1;
	}
	if(flag2) {
		died[player->ghost_posx[1]][player->ghost_posy[1]] = died2[player->ghost_posx[1]][player->ghost_posy[1]] = 1;
		died[player->ghost_posx[1]-1][player->ghost_posy[1]] = 1;
		died[player->ghost_posx[1]][player->ghost_posy[1]-1] = 1;
		died[player->ghost_posx[1]+1][player->ghost_posy[1]] = 1;
		died[player->ghost_posx[1]][player->ghost_posy[1]+1] = 1;
	}
	if(flag3) {
		died[player->opponent_posx][player->opponent_posy] = died2[player->opponent_posx][player->opponent_posy] = 1;
		died[player->opponent_posx-1][player->opponent_posy] = 1;
		died[player->opponent_posx][player->opponent_posy-1] = 1;
		died[player->opponent_posx+1][player->opponent_posy] = 1;
		died[player->opponent_posx][player->opponent_posy+1] = 1;
	}
	int x = player->your_posx, y = player->your_posy;
	for(int i = 0; i < 4; i++) {
		int nx = x + rad[i][0];
		int ny = y + rad[i][1];
		if(!in(nx,ny) || player->mat[nx][ny] == '#')	continue;
		is_alley[nx][ny] = alley(player,nx,ny);
	}
}

struct Point em_avoid(struct Player *player) {
	int x = player->your_posx;
	int y = player->your_posy;
	for(int i = 0; i < 4; i++) {
		int nx = x + rad[i][0];
		int ny = y + rad[i][1];
		if(!in(nx,ny) || player->mat[nx][ny] == '#'
		|| (died[nx][ny] || is_alley[nx][ny]) && died_work == 1
		|| died[nx][ny] && died_work == 1.5 
		|| died2[nx][ny] && died_work == 2)	continue;
		struct Point point = {nx, ny};
		return point;
	}
	struct Point point = {x, y};
	return point;
}

void Path(struct Player *player) {
	int mindist = 0;
	int maxx = 0;
	if(died_work)	maxx = max(oppo_dis, super_dis);
	ave_dis = super_dis = 1;
	if(!died_work)  oppo_dis = ghost1_dis = ghost2_dis = 1;
	if(died_work) {
		mindist = min(maxx, ghost1_dis);
		mindist = min(mindist, ghost2_dis);
	}
	struct Point *q = (struct Point*)malloc(row*col*sizeof(struct Point));
	int left = 0, right = 0;
	int *path = (int*)malloc(row*col*sizeof(int));
	memset(visit,0,sizeof(visit));
	path[right] = -1;
	q[right].X = player->your_posx;
	q[right++].Y = player->your_posy;
	int right1, right2, right3, right4_1, right4_2;
	int flag1 = 1, flag2 = 1, flag3 = 1, flag4_1 = 1, flag4_2 = 1;	//average & super & opponent & ghost 
	while(left < right && (flag1 || flag2 || flag3 || flag4_1 || flag4_2)) {
		int x = q[left].X;
		int y = q[left++].Y;
		for(int i = 0; i < 4; i++) {
			int nx = x + rad[i][0];
			int ny = y + rad[i][1];
			if(!in(nx,ny) || player->mat[nx][ny] == '#' || visit[nx][ny] 
			|| value[nx][ny]*2 > mindist && died_work == 0.5
			|| (died[nx][ny] || is_alley[nx][ny]) && died_work == 1
			|| died[nx][ny] && died_work == 1.5 
			|| died2[nx][ny] && died_work == 2)	continue;
			if(player->mat[nx][ny] == 'o' && flag1) {
				struct Point point = {nx, ny};
				q[right] = point;
				path[right++] = left-1;
				visit[nx][ny] = 1;
				right1 = right;
				flag1 = 0;
			} else if(player->mat[nx][ny] == 'O' && flag2) {
				struct Point point = {nx, ny};
				q[right] = point;
				path[right++] = left-1;
				visit[nx][ny] = 1;
				right2 = right;
				flag2 = 0;
			} else {
				struct Point point = {nx, ny};
				q[right] = point;
				path[right++] = left-1;
				visit[nx][ny] = 1;
			}
			if(nx == player->opponent_posx && ny == player->opponent_posy && flag3) {
				right3 = right;
				flag3 = 0;
			}
			if(nx == player->ghost_posx[0] && ny == player->ghost_posy[0] && flag4_1){
				right4_1 = right;
				flag4_1 = 0;
			}
			if(nx == player->ghost_posx[1] && ny == player->ghost_posy[1] && flag4_2) {
				right4_2 = right;
				flag4_2 = 0;
			}
		}
	}
{
	int i;
	if(flag1 == 1) {
		stack[1] = q[0];ave_dis = INF;
	} else{
		for(i = right1-1; path[i]; i = path[i])	ave_dis++; 
		stack[1] = q[i];
	}
	if(flag2 == 1) {
		stack[2] = q[0];super_dis = INF;
	} else{
		for(i = right2-1; path[i]; i = path[i])	super_dis++; 
		stack[2] = q[i];
	}
if(!died_work) {
	if(flag3 == 1) {
		stack[3] = q[0];oppo_dis = INF;
	} else{
		for(i = right3-1; path[i]; i = path[i])	oppo_dis++; 
		stack[3] = q[i];
	}
	if(flag4_1 == 1) {
		stack[5] = q[0];ghost1_dis = INF;
	} else{
		for(i = right4_1-1; path[i]; i = path[i])	ghost1_dis++; 
		stack[5] = q[i];
	}
	if(flag4_2 == 1) {
		stack[6] = q[0];ghost2_dis = INF;
	} else{
		for(i = right4_2-1; path[i]; i = path[i])	ghost2_dis++; 
		stack[6] = q[i];
	}
}
}
	free(q);
	free(path);
}

struct Point chase(struct Player *player) {
	if(did_wait == 0 && player->your_status > 1) {
		int flag = 0;
		int x = player->opponent_posx, y = player->opponent_posy;
		for(int i = 0; i < 4; i++) {
			int nx = x + rad[i][0];
			int ny = y + rad[i][1];
			if(!in(nx, ny) || player->mat[nx][ny] == '#' || nx == player->your_posx && ny == player->your_posy) {
				
			} 
			else {
				flag = 1;
				break;
			}
		}
		if(flag == 0) {
			did_wait = 1;
			struct Point point = {player->your_posx, player->your_posy};
			return point;
		}
		else {
			return stack[3];
		}
	}
	else {
		return stack[3];
	}
}

struct Point walk(struct Player *player) {	
	if(player->opponent_status == -1) {
		memset(is_alley,0,sizeof(is_alley));
	} 
	struct Point ret;
	if(dig_step > 0 && !(player->your_posx==back.X&&player->your_posy==back.Y)) {
		if(rushing == 1)	return rush(player,back);
		died_work = 0;
		Path(player);
		int mindist = min(ghost1_dis, ghost2_dis);
		mindist = min(mindist,max(oppo_dis,super_dis));
		if((dig_step+1)*2 > mindist) {
			rushing = 1;
			return rush(player,back);
		}
		else {
			if(ave_dis/ave_weight < super_dis/super_weight)
				ret = stack[1];
			else
				ret = stack[2];
		}
		dig_step++;
		return ret;
	}
	dig_step = -1;
	rushing = 0;
	memset(died,0,sizeof(died));
	memset(died2,0,sizeof(died2));
	died_work = 0;
	Path(player);
	if(player->your_status == 0) {
		if(ghost1_dis <= 2 || ghost2_dis <= 2 || oppo_dis <= 2 && player->opponent_status > 1) {
			avoid(player);
			died_work = 1;
			Path(player);
				ret = stack[2];
			if(ret.X == player->your_posx && ret.Y == player->your_posy)
				ret = stack[1];
			if(ret.X == player->your_posx && ret.Y == player->your_posy) {
				if(oppo_dis == 2 && player->opponent_status > 1) {
					died_work = 0;
					Path(player); 
					avoid(player);
					died_work = 1;
					ret = em_avoid(player);
					return ret;
				}
			}
			if(ret.X == player->your_posx && ret.Y == player->your_posy) {
				died_work = 1;
				ret = em_avoid(player);
			}
			if(ret.X == player->your_posx && ret.Y == player->your_posy && min(ghost1_dis, ghost2_dis) == 1) {
				died_work = 1.5;
				ret = em_avoid(player); 
			}
			if(ret.X == player->your_posx && ret.Y == player->your_posy && min(ghost1_dis, ghost2_dis) == 1) {
				died_work = 2;
				ret = em_avoid(player); 
			}
			if(oppo_dis == 1 && player->opponent_status > 1 &&
				ret.X == player->your_posx && ret.Y == player->your_posy) {
				ret.X = player->opponent_posx;
				ret.Y = player->opponent_posy;
				return ret;
			}
			if(ret.X == player->your_posx && ret.Y == player->your_posy && player->opponent_status <= 0 && min(ghost1_dis, ghost2_dis) == 1) {
				return suicide();
			}
		}
		else {
			//? 
			avoid(player);
			died_work = 0.5;
			Path(player);
			//
			if(ave_dis/ave_weight < super_dis/super_weight)
				ret = stack[1];
			else
				ret = stack[2];
			if(is_alley[ret.X][ret.Y]) {
				dig_step = 1;
				back.X = player->your_posx;
				back.Y = player->your_posy;
			} 
		}
    } else {
    	int ghost_dis = min(ghost1_dis, ghost2_dis);
    	if(ghost_dis < player->your_status*2 || ghost_dis == 1 ||
		(oppo_dis < player->your_status-6 || oppo_dis == 1) && player->opponent_status == 0) {
    		if(ghost_dis/ghost_weight > oppo_dis/oppo_weight && player->opponent_status == 0) {
    			ret = chase(player);
			}
			else {
				if(ghost_dis == ghost1_dis) 
    				ret = stack[5];
    			else
    				ret = stack[6];
			}
		}
		else {
			//? 
			avoid(player);
			died_work = 0.5;
			Path(player);
			//
			if(ave_dis/ave_weight < super_dis/super_weight)
				ret = stack[1];
			else
				ret = stack[2];
			if(is_alley[ret.X][ret.Y]) {
				dig_step = 1;
				back = {player->your_posx, player->your_posy};
			} 
		}
	}
	return ret;
}

