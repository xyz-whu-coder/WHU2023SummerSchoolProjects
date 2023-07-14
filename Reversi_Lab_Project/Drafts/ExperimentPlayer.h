/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define CORNERWEIGHT 2000
#define DANGEROUSPOS -2000
#define SAFEPOSITION 200
#define EDGEPOSITION 100
#define SUBEDGEVALUE -300
#define MIDDLEWEIGHT 100
#define SIGN 100

typedef struct PointWeight
{
    struct Point point;
    int weight;
    int value;
} PointWeight;

int map[20][20];
int direction[8][2] = {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

bool isCorner(struct Player *player, int x, int y)
{
    return (x == 0 && x == player->row_cnt - 1) || (y == 0 && y == player->col_cnt - 1);
}

bool isDangerousPoint(struct Player *player, int x, int y)
{
    return (x == 0 && y == 1) || (x == 1 && y == 0) || (x == 1 && y == 1) || (x == 1 && y == player->col_cnt - 2) || (x == 1 && y == player->col_cnt - 1) || (x == 0 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == 1) || (x == player->row_cnt - 2 && y == 0) || (x == player->row_cnt - 1 && y == 1) || (x == player->row_cnt - 2 && y == player->col_cnt - 2) || (x == player->row_cnt - 1 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == player->col_cnt - 1);
}

bool isSubEdge(struct Player *player, int x, int y) {
    for (int i = 1; i <= player->col_cnt - 2; i++) {
        if (y == i && (x == 1 || x == player->row_cnt - 2)) {
            return true;
        }
    }
    for (int i = 1; i <= player->row_cnt - 2; i++) {
        if (x == i && (y == 1 || y == player->col_cnt - 2)) {
            return true;
        }
    }
    return false;
}

bool isMiddlePoint(int rowMid, int columnMid, int x, int y) {
    if (x == rowMid || x == rowMid + 1) {
        if (y == columnMid - 1 || y == columnMid + 2) {
            return true;
        }
    }
    if (y == columnMid || y == columnMid + 1) {
        if (x == rowMid - 1 || x == rowMid + 2) {
            return true;
        }
    }
    return false;
}

bool isImportantMid(int rowMid, int columnMid, int x, int y) {
    return (x == rowMid - 1 && y == columnMid - 1) || (x == rowMid - 1 && y == columnMid + 2) || (x == rowMid + 2 && y == columnMid - 1) || (x == rowMid + 2 && y == columnMid + 2);
}

bool isEdge(int rowMax, int columnMax, int x, int y) {
    for (int i = 3; i <= rowMax - 3; i++) {
        if (i == x && (y == 0 || y == columnMax)) {
            return true;
        }
    }
    for (int i = 3; i <= columnMax - 3; i++) {
        if (i == y && (x == 0 || x == rowMax)) {
            return true;
        }
    }
    return false;
}

bool isImportantEdge(int rowMax, int columnMax, int x, int y) {
    return (x == 0 && y == 2) ||  (x == 2 && y == 0) || (x == 0 && y == columnMax - 2) || (x == 2 && y == columnMax) || (x == rowMax - 2 && y == 0) || (x == rowMax && y == 2) || (x == rowMax - 2 && y == columnMax) || (x == rowMax && y == columnMax - 2);
}

bool isInside(struct Player *player, int x, int y)
{
    return x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt;
}

bool isNumber(char ch)
{
    return ch >= '1' && ch <= '9';
}

void initMap(struct Player *player)
{
    int rowMax = player->row_cnt - 1;
    int columnMax = player->col_cnt - 1;
    int rowMid = rowMax / 2;
    int columnMid = columnMax / 2;

    // for (int i = 0; i < rowMax; i++) {
    //     for (int j = 0; j < columnMax; j++) {
    //         if (isCorner(player, i, j)) {
    //             map[i][j] = CORNERWEIGHT * player->mat[i][j];
    //             continue;
    //         }
    //         if (isDangerousPoint(player, i, j)) {
    //             map[i][j] = DANGEROUSPOS;
    //             continue;
    //         }
    //         if (isSubEdge(player, i, j)) {
    //             map[i][j] = SUBEDGEVALUE;
    //             if (isDangerousPoint(player, i, j)) {
    //                 map[i][j] = DANGEROUSPOS;
    //                 continue;
    //             }
    //             continue;
    //         }
    //         if (isMiddlePoint(rowMid, columnMid, i, j)) {
    //             map[i][j] = MIDDLEWEIGHT;
    //             continue;
    //         }
    //         if (isImportantMid(rowMid, columnMid, i, j)) {
    //             map[i][j] = 2 * MIDDLEWEIGHT;
    //             continue;
    //         }
    //         if (isEdge(rowMax, columnMax, i, j)) {
    //             map[i][j] = EDGEPOSITION;
    //         }
    //         if (isImportantEdge(rowMax, columnMax, i, j)) {
    //             map[i][j] = 10 * EDGEPOSITION;
    //         }
    //     }
    // }
    map[0][0] = CORNERWEIGHT * player->mat[0][0];
    map[0][columnMax] = CORNERWEIGHT * player->mat[0][columnMax];
    map[rowMax][0] = CORNERWEIGHT * player->mat[rowMax][0];
    map[rowMax][columnMax] = CORNERWEIGHT * player->mat[rowMax][columnMax];

    map[0][1] = map[1][0] = DANGEROUSPOS;
    map[0][columnMax - 1] = map[1][columnMax] = DANGEROUSPOS;
    map[rowMax - 1][0] = map[rowMax][1] = DANGEROUSPOS;
    map[rowMax - 1][columnMax] = map[rowMax][columnMax - 1] = DANGEROUSPOS;

    for (int i = 1; i <= columnMax - 1; i++)
    {
        map[1][i] = map[rowMax - 1][i] = SUBEDGEVALUE;
    }
    for (int i = 1; i <= rowMax - 1; i++)
    {
        map[i][1] = map[i][columnMax - 1] = SUBEDGEVALUE;
    }
    map[1][1] = map[1][columnMax - 1] = map[rowMax - 1][1] = map[rowMax - 1][columnMax - 1] = DANGEROUSPOS;

    
    for (int i = rowMid; i <= rowMid + 1; i++)
    {
        map[i][columnMid - 1] = map[i][columnMid + 2] = MIDDLEWEIGHT;
    }
    for (int i = columnMid; i <= columnMid + 1; i++)
    {
        map[rowMid - 1][i] = map[rowMid + 2][i] = MIDDLEWEIGHT;
    }
    map[rowMid - 1][columnMid - 1] = map[rowMid - 1][columnMid + 2] = map[rowMid + 2][columnMid - 1] = map[rowMid + 2][columnMid + 2] = 2 * MIDDLEWEIGHT;

    for (int i = 3; i <= rowMax - 3; i++)
    {
        map[i][0] = map[i][columnMax] = EDGEPOSITION;
    }
    for (int i = 3; i <= columnMax - 3; i++)
    {
        map[0][i] = map[rowMax][i] = EDGEPOSITION;
    }
    map[0][2] = map[2][0] = map[0][columnMax - 2] = map[2][columnMax] = map[rowMax - 2][0] = map[rowMax - 2][columnMax] = map[rowMax][2] = map[rowMax][columnMax - 2] = 2 * EDGEPOSITION;
    return;
}

void init(struct Player *player)
{
    // This function will be executed at the begin of each game, only once.
    initMap(player);
    srand(time(0));
}

bool isValid(struct Player *player, int startx, int starty, int *value)
{
    if (!isInside(player, startx, starty) || player->mat[startx][starty] == 'o' || player->mat[startx][starty] == 'O')
    {
        return false;
    }
    for (int dir = 0; dir < 8; dir++)
    {
        int tempx = startx + direction[dir][0];
        int tempy = starty + direction[dir][1];
        if (!isInside(player, tempx, tempy) || player->mat[tempx][tempy] != 'o')
        {
            continue;
        }
        int counter = 1;
        while (true)
        {
            tempx += direction[dir][0];
            tempy += direction[dir][1];
            if (!isInside(player, tempx, tempy) || isNumber(player->mat[tempx][tempy]))
            {
                break;
            }
            counter++;
            if (player->mat[tempx][tempy] == 'O')
            {
                *value = counter;
                return true;
            }
        }
    }
    return false;
}

bool updateFun(struct Player *player, int x, int y)
{
    int rowMax = player->row_cnt - 1;
    int columnMax = player->col_cnt - 1;
    if (map[x][y] == SIGN)
    {
        return false;
    }
    if (x == 0 && y == 0)
    {
        for (int i = y; i <= columnMax - 2; i++)
        {
            map[x][i] = SIGN;
        }
        for (int i = x; i <= rowMax - 2; i++)
        {
            map[i][y] = SIGN;
        }
        map[1][1] = SIGN;
    }
    else if (x == 0 && y == columnMax)
    {
        for (int i = 2; i <= y; i++)
        {
            map[x][i] = SIGN;
        }
        for (int i = x; i <= rowMax - 2; i++)
        {
            map[i][y] = SIGN;
        }
        map[1][columnMax - 1] = SIGN;
    }
    else if (x == rowMax && y == 0)
    {
        for (int i = y; i <= columnMax - 2; i++)
        {
            map[x][i] = SIGN;
        }
        for (int i = 2; i <= x; i++)
        {
            map[i][y] = SIGN;
        }
        map[rowMax - 1][1] = SIGN;
    }
    else if (x == rowMax && y == columnMax)
    {
        for (int i = 2; i <= y; i++)
        {
            map[x][i] = SIGN;
        }
        for (int i = 2; i <= x; i++)
        {
            map[i][y] = SIGN;
        }
        map[rowMax - 1][columnMax - 1] = SIGN;
    }
    return true;
}

void dfs(struct Player *player, int startx, int starty, int visited[][20])
{
    if (visited[startx][starty] || !isInside(player, startx, starty))
    {
        return;
    }
    visited[startx][starty] = 1;
    if (player->mat[startx][starty] != 'O')
    {
        map[startx][starty] = SIGN;
        return;
    }
    for (int dir = 0; dir < 8; dir++)
    {
        int nextx = startx + direction[dir][0];
        int nexty = starty + direction[dir][1];
        dfs(player, nextx, nexty, visited);
    }
}

void updateMap(struct Player *player)
{
    int rowMax = player->row_cnt - 1;
    int columnMax = player->col_cnt - 1;
    int visited[20][20] = {0};
    for (int i = 0; i <= rowMax; i += rowMax)
    {
        for (int j = 0; j <= columnMax; j += columnMax)
        {
            if (isCorner(player, i, j) && player->mat[i][j] == 'O')
            {
                if (!updateFun(player, i, j))
                {
                    dfs(player, i, j, visited);
                }
            }
        }
    }
}



void reverseMat(struct Player *player, int startx, int starty)
{
    player->mat[startx][starty] = 'O';
    bool flag;
    for (int dir = 0; dir < 8; dir++)
    {
        flag = false;
        int tempx = startx + direction[dir][0];
        int tempy = starty + direction[dir][0];
        while (isInside(player, tempx, tempy))
        {
            if (player->mat[tempx][tempy] == 'O')
            {
                flag = true;
                break;
            }
            tempx += direction[dir][0];
            tempy += direction[dir][1];
        }
        if (flag)
        {
            tempx = startx + direction[dir][0];
            tempy = starty + direction[dir][1];
            while (isInside(player, tempx, tempy))
            {
                if (player->mat[tempx][tempy] == 'o')
                {
                    player->mat[tempx][tempy] = 'O';
                }
                else
                {
                    break;
                }
                tempx += direction[dir][0];
                tempy += direction[dir][1];
            }
        }
    }
    return;
}

bool isOpponent(struct Player *player, int startx, int starty)
{
    if (!isInside(player, startx, starty) || player->mat[startx][starty] == 'O' || player->mat[startx][starty] == 'o')
    {
        return false;
    }
    for (int dir = 0; dir < 8; dir++)
    {
        int nextx = startx + direction[dir][0];
        int nexty = starty + direction[dir][1];
        if (!isInside(player, nextx, nexty))
        {
            continue;
        }
        if (player->mat[nextx][nexty] != 'O')
        {
            continue;
        }
        while (true)
        {
            nextx += direction[dir][0];
            nexty += direction[dir][1];
            if (!isInside(player, nextx, nexty) || isNumber(player->mat[nextx][nexty]))
            {
                break;
            }
            if (player->mat[nextx][nexty] == 'o')
            {
                return true;
            }
        }
    }
    return false;
}

struct Point place(struct Player *player)
{
    updateMap(player);
    // int chessCount = player->your_score + player->opponent_score;
    // int sum = player->col_cnt * player->row_cnt;
    PointWeight *validPoint = (PointWeight *)malloc(sizeof(PointWeight) * player->col_cnt * player->row_cnt);
    int validCount = 0;
    int maxNum = -1000;
    int minNum = 1000;
    int value = 0;
    int cornerNum = 100;
    char tempMat[20][20];
    struct Point point = initPoint(-1, -1);
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (isValid(player, i, j, &value))
            {
                validPoint[validCount++].point = initPoint(i, j);
                validPoint[validCount++].weight = map[i][j];
                for (int ii = 0; ii < player->row_cnt; ii++)
                {
                    for (int jj = 0; jj < player->col_cnt; jj++)
                    {
                        tempMat[ii][jj] = player->mat[ii][jj];
                    }
                }
                reverseMat(player, i, j);
                int num = 0;
                for (int ii = 0; ii < player->row_cnt; ii++)
                {
                    for (int jj = 0; jj < player->col_cnt; jj++)
                    {
                        player->mat[ii][jj] = tempMat[ii][jj];
                    }
                }
                if (map[i][j] > maxNum || num < cornerNum)
                {
                    point.X = i;
                    point.Y = j;
                    cornerNum = num;
                    maxNum = map[i][j];
                }
                else if (map[i][j] == maxNum)
                {
                    for (int ii = 0; ii < player->row_cnt; ii++)
                    {
                        for (int jj = 0; jj < player->col_cnt; jj++)
                        {
                            tempMat[ii][jj] = player->mat[ii][jj];
                        }
                    }
                    reverseMat(player, i, j);
                    int num = 0;
                    for (int ii = 0; ii < player->row_cnt; ii++)
                    {
                        for (int jj = 0; jj < player->col_cnt; jj++)
                        {
                            if (isOpponent(player, ii, jj) && !isDangerousPoint(player, ii, jj))
                            {
                                num++;
                            }
                        }
                    }
                    if (num < minNum)
                    {
                        point.X = i;
                        point.Y = j;
                        minNum = num;
                    }
                    for (int ii = 0; ii < player->row_cnt; ii++)
                    {
                        for (int jj = 0; jj < player->col_cnt; jj++)
                        {
                            player->mat[ii][jj] = tempMat[ii][jj];
                        }
                    }
                }
            }
        }
    }
    free(validPoint);
    return point;
}