#include <string.h>
#include "../include/playerbase.h"
#include<queue>
#include<malloc.h>
#include<climits>
#include<stack>
#include<math.h>

int ghost[2][2];
int my_pos[2];
int my_status;
int opponent_status;
int enemy_pos[2]; // 敌人位置

char** arr;
int** weight;
int row;
int col;
bool** visited;
int dir[4][2] = {{-1,0},{1,0},{0,1},{0,-1}};
using namespace std;



struct P_info{
	int x;
	int y;
	int wei;
	int belong;
	int step;
	P_info* pre;  //先不管
};

// 仿函数，实现大顶堆 
struct cmp
{
    // 定义排序规则 
    bool operator() (P_info* a,P_info* b )
    { 
        
        return a->wei < b->wei;   // 每次出队最大weight
    }
}; 
// 定义优先队列
priority_queue<P_info*,vector<P_info*>,cmp> q;
stack<P_info*>path;

queue<P_info*>normal_que;

bool is_in(int x,int y)
{
	if(x<0 || y<0 || x>=row || y>= col)	
		return false;
	return true;
}
 
void pop_all()
{
	while (!q.empty())
	{
		q.pop();
	}
	
}

int manha(int* start,int *end)
{
	return abs(start[0]-end[0]) + abs(start[1]-end[1]);

}


P_info* get_a_point(int x,int y ,int step , int belong)
{
	P_info* temp= new P_info;
	temp->x = x;
	temp->y = y;
	temp->wei = weight[x][y];
	temp->belong =belong;
	temp->step = step;

	return temp;
}


void push_all_dir(P_info* cur)
{
	
	int x=cur->x;
	int y=cur->y;
	for(int i=0;i<4;i++)
	{
		int next_x = x+ dir[i][0];
		int next_y = y+ dir[i][1];
		if(!is_in(next_x,next_y) || visited[next_x][next_y] == true )
			continue;
		if(arr[next_x][next_y] == '#' )
			continue;
		q.push(get_a_point(next_x,next_y, cur->step +1 , cur->belong));  // 先只管一种代价
	}
}


int BFS()
{
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
			visited[i][j]=false;
	}

	int x = my_pos[0];
	int y = my_pos[1];
	for(int i=0;i<4;i++)
	{
		int next_x = x + dir[i][0];
		int next_y = y + dir[i][1];
		if(!is_in(next_x,next_y))
			continue;
		if(arr[next_x][next_y] == '#' || visited[next_x][next_y]==true)
			continue;
		q.push(get_a_point(next_x,next_y, 1 , i) );  // 先只管一种代价
		visited[next_x][next_y]=true;
	}

	P_info* res =NULL;

	while (!q.empty())
	{
		P_info* temp =q.top();
		q.pop();
		visited[temp->x][temp->y] = true;

	
		if(res==NULL || res->wei - res->step < temp->wei - temp->step)
			res = temp;
		
		if(temp->step > 50 && res !=NULL ) 
		{
			break;
		}
		push_all_dir(temp);
	}
	int d = res->belong;
	pop_all();
	return d;
}


void spread_big_douzi(int x,int y) //传入大豆子的位置
{
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
			visited[i][j]=false;
	}

	normal_que.push(get_a_point(x,y,0,0));

	while (!normal_que.empty())
	{
		P_info* temp=normal_que.front();
		normal_que.pop();
		if(temp->step >=7)
			break;
		weight[temp->x][temp->y] +=( 8 -temp->step);
		
		int x=temp->x;
		int y=temp->y;
		for(int i=0;i<4;i++)
		{
			int next_x = x+ dir[i][0];
			int next_y = y+ dir[i][1];
			if( !is_in(next_x,next_y) || visited[next_x][next_y] == true )
				continue;
			if(arr[next_x][next_y] == '#')
				continue;
			normal_que.push(get_a_point(next_x,next_y, temp->step +1 , temp->belong));  // 先只管一种代价
			visited[next_x][next_y] = true;
		} // 改1  
		
	}
	while (!normal_que.empty())
	{
		normal_que.pop();
	}
}

void update_wei()
{
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			if(arr[i][j] == 'O')
				spread_big_douzi(i,j);
		}
	}
}

// 以上是辅助函数-------------------------------------------------------


void init(struct Player *player) 
{
	// This function will be executed at the begin of each game, only once.
	arr = player->mat;
	row =player->row_cnt;
	col = player->col_cnt;
	weight =(int**)malloc(sizeof(int*)*row);
	visited =(bool**)malloc(sizeof(bool*)*row);
	
	for(int j=0;j<row;j++)
	{
		weight[j] = (int*)malloc(sizeof(int)*col);
		visited[j] = (bool*)malloc(sizeof(bool)*col);
	
	}
	for(int i=0;i<row ; i++)
	{
		for(int j=0;j<col ;j++)
		{
			if(arr[i][j] == '#')
			{
				weight[i][j] = INT_MIN;
			}
			else if(arr[i][j] == 'O')
				weight[i][j] = 40;

			else if(arr[i][j] == 'o')
				weight[i][j]= 10;
		}
	}

	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			visited[i][j]=false;
		}
	}
    update_wei();
    // for(int i=0;i<row;i++)
    // {
    //     for(int j=0;j<col;j++)
    //         printf("%d  ",weight[i][j]);
    //      printf("\n");
    // }
    
        


}


void ghost_speard(int* ghost_pos,int spread_val,int spread_range)
{
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
			visited[i][j]=false;
	}

	normal_que.push(get_a_point(ghost_pos[0],ghost_pos[1],0,0));

	while (!normal_que.empty())
	{
		P_info* temp=normal_que.front();
		normal_que.pop();
		if(temp->step >= spread_range)
			break;
		if(spread_val < 0)
		{
			
			weight[temp->x][temp->y] = spread_val + temp->step;
			if(temp->step == 1)
			{
				weight[temp->x][temp->y] = spread_val -1;
			}
		}
		else
		{
			weight[temp->x][temp->y] = spread_val - temp->step;
		}
		
		int x= temp->x;
		int y= temp->y;
		for(int i=0;i<4;i++)
		{
			int next_x = x+ dir[i][0];
			int next_y = y+ dir[i][1];
		
			if( !is_in(next_x,next_y) || visited[next_x][next_y] == true )
				continue;
			if(arr[next_x][next_y] == '#')
				continue;
			normal_que.push(get_a_point(next_x,next_y, temp->step +1 , temp->belong));  // 先只管一种代价
			visited[next_x][next_y] = true;
		} // 改1  
	}
	while (!normal_que.empty())
	{
		normal_que.pop();
	}
}


int** weight_cpy()
{
	int ** temp =(int**)malloc(sizeof(int*)*row);
	
	
	for(int j=0;j<row;j++)
	{
		temp[j] = (int*)malloc(sizeof(int)*col);
	}
	for(int i=0;i<row ;i++)
	{
		for(int j=0;j<col ;j++)
		{
			temp[i][j] = weight[i][j];
		}
	}
	return temp;
}




struct Point walk(struct Player *player) {
	// This function will be executed in each round.
	stack<int**> wei_store;
	for(int i=0;i<2;i++)
	{
		ghost[i][0] = player->ghost_posx[i];
		ghost[i][1] = player->ghost_posy[i];
	}
	my_pos[0] = player->your_posx;
	my_pos[1] = player->your_posy;
	enemy_pos[0] = player->opponent_posx;
	enemy_pos[1] = player->opponent_posy;

	my_status = player->your_status;
	opponent_status = player->opponent_status;
	
	wei_store.push(weight_cpy());
 
	if( my_status <= 3)
	{
		ghost_speard(ghost[0],-9999,2);
		ghost_speard(ghost[1],-9999,2);
	}
	else
	{
		ghost_speard(ghost[0],INT_MAX,my_status-4);
		ghost_speard(ghost[1],INT_MAX,my_status-4); //最后5步开始跑 (其实可以算下离鬼有多远再决定跑不跑)
	}
	
	
	int d = BFS();

	weight = wei_store.top();
	wei_store.pop();

	int next_x = my_pos[0]+ dir[d][0];
	int next_y = my_pos[1]+ dir[d][1];

	struct Point ret = {next_x,next_y};
	if(weight[next_x][next_y]>0)
		weight[next_x][next_y]= 0;
	else
		weight[next_x][next_y] -= 1000 ;//走过了尽量不要走
	return ret;

}
		

