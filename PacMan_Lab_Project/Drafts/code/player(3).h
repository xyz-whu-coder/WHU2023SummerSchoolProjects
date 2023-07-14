#include <bits/stdc++.h>
#include <math.h>
#include "../include/playerbase.h"
#define MAX 1000
#define MAPMAX 15
#define INF 9999999
int row, col, super_star_count, common_star_count, sstop;
char map[20][20], map_row, map_col;
bool vis[MAX][MAX], opvis[MAX][MAX];
int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

bool in(struct Player *p, int x, int y);

typedef struct Supreme
{
	struct Point point;
	int step;
} q;

struct note{
	int x;
	int y;
	int d;													//to calculate the distance
};

struct node {
    int x;
    int y;
    int dist;
	int index;
    bool operator> (const node &b) const
    {
        return this->dist > b.dist;
    }
};

typedef struct SuperStar
{
	struct Point Superstar_index;
	bool if_visited;
	int mts;
	struct SuperStar *next;
} superstar;
superstar *head;
//copy the map to local
void copy_block(struct Player* p)
{
	int x_bias = p -> your_posx - MAPMAX / 2, y_bias = p -> your_posy - MAPMAX / 2;
	for(int i = 0; i < MAPMAX; i++)
	{
		for(int j = 0; j < MAPMAX; j++)
		{
			if(in(p, i + x_bias, j + y_bias))
			{
				map[i][j] = p -> mat[i + x_bias][j + y_bias];
			}
			else
			{
				map[i][j] = '#';
			}
		}
	}
}

bool in(struct Player *p, int x, int y)
{
	return x >= 0 && y >= 0 && x < p -> row_cnt && y < p -> col_cnt;
}

bool inmap(int mapsize, int x, int y)
{
	return x >= 0 && y >= 0 && x < mapsize && y < mapsize;
}

bool if_queue_empty(int front, int rear)
{
	return front == rear;
}

bool if_queue_full(int rear)
{
	return rear == MAX - 1;
}

bool enqueue(q *queue, struct Point p, int s, int *rear)
{
	if(if_queue_full(*rear))
	{
		return false;
	}
	(*rear)++;
	queue[*rear].point = p;
	queue[*rear].step = s;
	return true;
}

bool dequeue(int *front, int *rear)
{
	if(if_queue_empty(*front, *rear))
	{
		return false;
	}
	(*front)++;
	return true;
}

bool if_stack_empty(int top)
{
	return top == -1;
}

bool if_stack_full(int top)
{
	return top == MAX - 1;
}

bool Push(int *stack, int *top, int index)
{
	if(if_stack_full(*top))
	{
		return false;
	}
	stack[++(*top)] = index;
	return true;
}

bool Pop(int *top)
{
	if(if_stack_empty(*top))
	{
		return false;
	}
	(*top)--;
	return true;
}

int Gettop(int *stack, int top)
{
	if(if_stack_empty(top))
	{
		return -MAX;
	}
	return stack[top];
}

struct Point face_to_ghost (struct Player *p) 
{
	struct Point ans;
	ans.X = -1;
	ans.Y = -1;
	if(abs(p->your_posx - p->ghost_posx[0]) + abs(p->your_posy -p->ghost_posy[0]) == 2 || abs(p->your_posx - p->ghost_posx[1]) + abs(p->your_posy - p->ghost_posy[1]) == 2) {
		ans.X = p->your_posx;
		ans.Y = p->your_posy;
		return ans;
	} 
	else if (abs(p->your_posx - p->ghost_posx[0]) + abs(p->your_posy - p->ghost_posy[0]) == 1) {
		ans.X = p->ghost_posx[0];
		ans.Y = p->ghost_posy[0];
		return ans;
	} else if (abs(p->your_posx - p->ghost_posx[1]) + abs(p->your_posy - p->ghost_posy[1]) == 1) {
		ans.X = p->ghost_posx[1];
		ans.Y = p->ghost_posy[1];
		return ans;
	}
	return ans;
}

int find_shortest_way(int x, int y, int goaltx, int goalty, int longest)	//find the shortest way to catch the goal
{
	struct note que[MAX * MAX];
	int book[MAX][MAX] = {0};
	int head = 0, tail = 0;
	int i, j, k, tx, ty, flag;
	que[tail].x = x;
	que[tail].y = y;
	que[tail].d = 0;
	tail++;
	flag = 0;
	while (head < tail) {
		for (k = 0; k < 4; k++) {
			tx = que[head].x + dir[k][0];
			ty = que[head].y + dir[k][1];
			if (tx < 0 || tx >= row || ty < 0 || ty >= col) {
				continue;
			}
			if (map[tx][ty] != '#' && book[tx][ty] == 0) {
				book[tx][ty] = 1;
				que[tail].x = tx;
				que[tail].y = ty;
				que[tail].d = que[head].d + 1;
				tail++;
				if (que[tail - 1].d > longest) {
					return INF;
				}
			}
			if (tx == goaltx && ty == goalty) {
				return que[tail - 1].d;
			}
		}
	}
}

double probability(int run_after_me, int run_after_enemy)
{
	double p;
	p = 1.0 * run_after_enemy / (run_after_me + run_after_enemy);
	return p;
}

/*this function is to decide whether to stay to wait the ghost to move or not*/
/*waiting for complete*/
/*but not for missions, just for competition*/

struct Point stay(struct Player* p, int &cnt1, int &cnt2)
{
	struct Point tmp;
	int dis1, dis2;
	tmp.X = -1;
	tmp.Y = -1;
	if(abs(p -> your_posx - p -> ghost_posx[0]) + abs(p -> your_posy - p -> ghost_posy[0]) == 1)
	{
		dis1 = 1;
		cnt1++;
	}
	if(abs(p -> your_posx - p -> ghost_posx[1]) + abs(p -> your_posy - p -> ghost_posy[1]) == 1)
	{
		dis2 = 1;
		cnt1++;
	}
	if(abs(p -> your_posx - p -> ghost_posx[0]) + abs(p -> your_posy - p -> ghost_posy[0]) == 2)
	{
		if(2 == find_shortest_way(p -> your_posx, p -> your_posy, p -> ghost_posx[0], p -> ghost_posy[0], INF))
		{
			dis1 = 2;
			cnt2++;
		}
	}
	if(abs(p -> your_posx - p -> ghost_posx[1]) + abs(p -> your_posy - p -> ghost_posy[1]) == 2)
	{
		if(2 == find_shortest_way(p -> your_posx, p -> your_posy, p -> ghost_posx[1], p -> ghost_posy[1], INF))
		{
			dis2 = 2;
			cnt2++;
		}		
	}
}

//judge one point is a cross or not
bool crossroad(struct Player* p, int X, int Y)
{
	int tx, ty;
	int cnt = 0;
	for(int i = 0; i < 4; i++)
	{
		tx = X + dir[i][0];
		ty = Y + dir[1][0];
		if(in(p, tx, ty))
		{
			if(p -> mat[tx][ty] != '#')
			{
				cnt++;
			}
		}
	}
	return cnt > 1;
}

/*
*if i can't escape, i'd rather suicide
*just to keep my money not to lose too much
*MONEY ALWAYS PREVAILS
*if i'm strengthened, i can walk anywhere
*if i'm not strengthened, i can only go to somewhere that is away from the ghost (and strenghthened opponent)
*when calculate, one should consider the probability of the ghost run to yourself and your opponent
*if your opponent are stronger than you, and he's enough to you, then suicide
*considering to the probability and randomness, we can't suicide too early though we might lose one or two points
*so we finally decide to regard three blocks as a "safe distance"
**/
/*
*in order to saving time for tha main calculation
*this suicide function will not be too complex
*/
/**********************************************/
/*this function is fxxking discarded!!!!!!!!!!*/
struct Point suicide(struct Player* p)
{
	
	struct Point tmp;
	tmp.X = -1;
	tmp.Y = -1;

	//if i'm 1, i'll fxxk others over
	//hohoho
	if (p -> your_status > 0)
	{
		tmp.X = -1;
		tmp.Y = -1;
		return tmp;
	}

	//calculate the distances of the three opponent point from myself
	//int disp2p, disp2g1, disp2g2, diso2g1, diso2g2;
	// disp2p = find_shortest_way(p -> your_posx, p -> your_posy, p -> opponent_posx, p -> opponent_posy);
	// disp2g1 = find_shortest_way(p -> your_posx, p -> your_posy, p -> ghost_posx[0], p -> ghost_posy[0]);
	// disp2g2 = find_shortest_way(p -> your_posx, p -> your_posy, p -> ghost_posx[1], p -> ghost_posy[1]);
	// diso2g1 = find_shortest_way(p -> opponent_posx, p -> opponent_posy, p -> ghost_posx[0], p -> ghost_posy[0]);
	// diso2g2 = find_shortest_way(p -> opponent_posx, p -> opponent_posy, p -> ghost_posx[1], p -> ghost_posy[1]);

	//if i'm 0 (???), i'm the weakest(that's true)
	//which means that i should avoid meeting the ghost
	//and if opponent is 1(emm), i don't want to be fxxked over
	int tx, ty, score = 0, flag = 0;									//
	int cnt = 0;														//count the ghost around me
	if (p -> your_status == 0)
	{
		for (int i = 0; i < 4; i++)										//search from four directions
		{
			int step = 0;
			tx = p -> your_posx + dir[i][0];
			ty = p -> your_posy + dir[i][1];
			if (p -> mat[tx][ty] == '#')
			{
				cnt++;
				continue;
			}
			while (in(p, tx, ty) && p -> mat[tx][ty] != '#')				//search for a cross road
			{
				tx += dir[i][0];
				ty += dir[i][1];
				step++;													//calculate the steps
				if (tx == p -> ghost_posx[0] && ty == p -> ghost_posy[0])
				{
					cnt++;
					if (cnt == 4)										//if we are blocked
					{
						flag = 1;
						break;
					}
					continue;											//this path cannot continue
				}
				if (tx == p -> ghost_posx[1] && ty == p -> ghost_posy[1])
				{
					cnt++;
					if (cnt == 4)
					{
						flag = 1;
						break;
					}
					continue;
				}
				if (p -> opponent_status > 2 - step)
				{
					if (tx == p -> opponent_posx && ty == p -> opponent_posy)
					{
						cnt++;
						if (cnt == 4)
						{
							flag = 1;
							break;
						}
						continue;
					}
				}
				if (p -> mat[tx][ty] == 'o')
				{
					score += 10;
				}			
				if (p -> mat[tx][ty] == 'O')
				{
					score += 10;
					return tmp;
				}								
				if (step >= 2)											//if we can walk more than 3 steps, we are now safe
				{
					return tmp;
				}
			}
		}
	}
	if (flag && (!score))												//if we can't get point in the way
	{
		for (int i = 0; i < 4; i++)
		{
			if (p -> mat[p -> your_posx + dir[i][0]][p -> your_posy + dir[i][1]] == '#')
			{
				tmp.X = p -> your_posx + dir[i][0];
				tmp.Y = p -> your_posy + dir[i][1];
			}
		}
	}
	return tmp;
}

struct Point find(q *queue, int *path, int start)	//Problem here
{
    // printf("2323432\n");
	while(path[start] != 1)
	{
		start = path[start];
	}
    // printf("111\n");
	return queue[start].point;
}

int randnum()
{
	return rand() % 101;
}

bool cmp(struct Point p1, struct Point p2)
{
	return p1.X == p2.X && p1.Y == p2.Y;
}
// /*
// int binarysearch(int start, int end, super *ss, struct Point dest)
// {
// 	if (start <= end)
// 	{
// 		int mid;
// 		mid = (start + end) / 2;
// 		if (cmp(ss[mid].pos,dest))
// 		{
// 			return mid;
// 		}
// 		if (dest.X + dest.Y < ss[mid].pos.X + ss[mid].pos.Y)
// 		{
// 			return binarysearch(start, mid - 1, ss, dest);
// 		}
// 		else
// 		{
// 			return binarysearch(mid + 1, end, ss, dest);
// 		}
// 	}
// }
// */
struct Point bfs(struct Player *p)	// map     BigO      Smallo
{
	q my_queue[MAX];
    std::priority_queue <node, std::vector <node>, std::greater <node> > Smallo;
	int my_stat, myfront, myrear, mypath_top, mypath[MAX];
	struct Point tmpp1;
	myfront = myrear = mypath_top = 0;
	tmpp1 = initPoint(MAPMAX / 2, MAPMAX / 2);
	enqueue(my_queue, tmpp1, 0, &myrear);	// index == 1
	// vis[p -> your_posx][p -> your_posy] = true;
	// opvis[p -> opponent_posx][p -> opponent_posy] = true;
    // printf("R U Serious?????????????\n");
	while (!if_queue_empty(myfront, myrear))
	{
		dequeue(&myfront, &myrear);
		int mytx, myty;
        mytx = my_queue[myfront].point.X;
		myty = my_queue[myfront].point.Y;
        // printf("%d\n", vis[mytx][myty]);
		if(inmap(MAPMAX, mytx, myty) && map[mytx][myty] != '#' && !vis[mytx][myty])
		{	
            vis[mytx][myty] = true;
			for (int i = 0; i < 4; i++)
			{
				mytx = my_queue[myfront].point.X;
				myty = my_queue[myfront].point.Y;
				mytx += dir[i][0];
				myty += dir[i][1];
				tmpp1 = initPoint(mytx, myty);
				enqueue(my_queue, tmpp1, my_queue[myfront].step + 1, &myrear);
				mypath[myrear] = myfront;
                // printf("myfront: %d  myrear: %d\n", myfront, myrear);
				// vis[mytx][myty] = true;
                // printf("mypath[%d] = %d\n", myrear, myfront);
                // for (int k = 0; k < 9; k++)
                // {
                //     for (int j = 0; j < 9; j++)
                //     {
                //         putchar(map[k][j]);
                //     }
                //     printf("\n");
                // }
                // printf("\n");
                // printf("mytx: %d  myty: %d map[mytx][myty] = %c\n", mytx, myty, map[mytx][myty]);
				if (map[mytx][myty] == 'O')
				{
                    // printf("My side has traversalled big O.\n");
					struct Point next_point_on_map, next_point_on_mat;
					next_point_on_map = find(my_queue, mypath, myrear);
					next_point_on_mat = initPoint(p -> your_posx + next_point_on_map.X - 6, p -> your_posy + next_point_on_map.Y - 6);
                    //printf("%d, %d\n", next_point_on_mat.X, next_point_on_mat.Y);
					return next_point_on_mat;
				}
				if (map[mytx][myty] == 'o')
				{
                    // printf("FFFUCK\n");
					Smallo.push((node){mytx, myty, my_queue[myfront].step + 1, myrear});
				}
			}
		}
	}
	if (!Smallo.empty())
	{
		tmpp1 = find(my_queue, mypath, Smallo.top().index);
        //printf("%d, %d\n", p -> your_posx + tmpp1.X - 6, p -> your_posy + tmpp1.Y - 6);
		return initPoint(p -> your_posx + tmpp1.X - 6, p -> your_posy + tmpp1.Y - 6);
	}
	else
	{

		// TODO

	}
}

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
	row = player -> row_cnt;
	col = player -> col_cnt;
	head -> next = NULL;
	head -> Superstar_index = initPoint(-1, -1);
	head -> if_visited = false;
	superstar *temp;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (player -> mat[i][j] == 'O')
			{
				
			}
		}
	}
    // print(ss, sstop);
	srand(time(NULL));
}


struct Point walk(struct Player *player) {
	// This function will be executed in each round.
	//suicide(player);
	memset(vis, 0, sizeof(vis));
	struct Point ret = {player->your_posx, player->your_posy};
    copy_block(player);
	ret = bfs(player);

    return ret;
}