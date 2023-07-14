#include <string.h>
#include "../include/playerbase.h"
#include <time.h>
#include <stdbool.h>

#define WALL '#'
#define NORMAL_STAR 'o'
#define SUPER_STAR 'O'
#define GROUND '.'
#define MAPSIZE 50

typedef struct PointPosition
{
	struct Point point;
	int distance;
	int flag;
} PointPos;

int map[MAPSIZE][MAPSIZE];
int step[4][2] = {
	{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
}

bool isInside(struct Player *player, int x, int y)
{
	return x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt;
}

bool isMeetingGhost(struct Player *player, int x, int y)
{
	return (x == player->ghost_posx[0] && y == player->ghost_posy[0]) || (x == player->ghost_posx[1] && x == player->ghost_posy[1]);
}

bool isValid(struct Player *player, int x, int y)
{
	return isInside(player, x, y) && player->mat[x][y] != WALL && !isMeetingGhost(player, x, y);
}

void searchStar(struct Player *player, int startx, int starty, PointPos *position, int distance, int visited[][MAPSIZE])
{
	if (position->flag == 1 || visited[startx][starty])
	{
		return;
	}
	if (player->mat[startx][starty] == NORMAL_STAR)
	{
		position->point.X = startx;
		position->point.Y = starty;
		position->distance = distance;
		position->flag = 1;
		return;
	}
	visited[startx][starty] = 1;
	for (int i = 0; i < 4; i++)
	{
		int nextx = startx + step[i][0];
		int nexty = starty + step[i][1];
		if (isValid(player, nextx, nexty))
		{
			searchStar(player, nextx, nexty, position, distance + 1, visited);
		}
	}
}

struct Point walk(struct Player *player)
{
	// This function will be executed in each round.
	struct Point ret = {player->your_posx, player->your_posy};
	PointPos position = {ret, 0, 0};
	int visited[MAPSIZE][MAPSIZE] = {0};
	searchStar(player, ret.X, ret.Y, &position, 0, visited);
	ret = position.point;
	return ret;
}
