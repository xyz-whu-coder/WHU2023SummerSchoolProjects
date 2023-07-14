#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <queue>
#include <cmath>
#include "../include/playerbase.h"

void init(struct Player* player) {
    // 此函数将在每个游戏开始时执行，仅执行一次。
}

// 检查移动是否合法（不与墙壁或幽灵发生碰撞）
bool isValidMove(struct Player* player, int x, int y) {
    // 检查是否超出边界
    if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
        return false;
    }

    // 检查是否与墙壁碰撞
    if (player->mat[x][y] == '#') {
        return false;
    }

    // 检查是否与幽灵碰撞
    for (int i = 0; i < 2; i++) {
        if (x == player->ghost_posx[i] && y == player->ghost_posy[i]) {
            return false;
        }
    }

    return true;
}

// 寻找最近的星星坐标
struct Point findNearestStar(struct Player* player) {
    int minDistance = 100; // 初始化最小距离为一个较大的值
    struct Point nearestStar = {-1, -1}; // 初始化最近星星的坐标为无效值

    // 遍历地图，寻找最近的星星
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (player->mat[i][j] == 'o' || player->mat[i][j] == 'O') { // 普通星星或超级星星
                // 计算当前格子与玩家的距离
                int distance = abs(i - player->your_posx) + abs(j - player->your_posy);

                // 更新最小距离和最近星星的坐标
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestStar.X = i;
                    nearestStar.Y = j;
                }
            }
        }
    }

    return nearestStar;
}

// 计算两点之间的曼哈顿距离
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 使用A*算法寻找最短可行路径
struct Point findShortestFeasiblePath(struct Player* player, int startX, int startY, int targetX, int targetY) {
    struct Point ret = {-1, -1};

    // 创建优先队列，用于存储待扩展的节点
    std::priority_queue<std::pair<int, std::pair<int, int>>> pq;
    pq.push(std::make_pair(0, std::make_pair(startX, startY)));

    // 创建距离和父节点记录表
    int** distances = new int* [player->row_cnt];
    for (int i = 0; i < player->row_cnt; i++) {
        distances[i] = new int[player->col_cnt];
        for (int j = 0; j < player->col_cnt; j++) {
            distances[i][j] = -1;
        }
    }
    distances[startX][startY] = 0;

    // 创建父节点坐标记录表
    struct Point** parents = new struct Point* [player->row_cnt];
    for (int i = 0; i < player->row_cnt; i++) {
        parents[i] = new struct Point[player->col_cnt];
        for (int j = 0; j < player->col_cnt; j++) {
            parents[i][j].X = -1;
            parents[i][j].Y = -1;
        }
    }

    while (!pq.empty()) {
        int currX = pq.top().second.first;
        int currY = pq.top().second.second;
        pq.pop();

        if (currX == targetX && currY == targetY) {
            // 找到目标节点，跳出循环
            break;
        }

        // 向四个方向扩展
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        for (int i = 0; i < 4; ++i) {
            int newX = currX + dx[i];
            int newY = currY + dy[i];

            // 检查新的位置是否合法
            if (newX >= 0 && newX < player->row_cnt && newY >= 0 && newY < player->col_cnt &&
                player->mat[newX][newY] != '#' && isValidMove(player, newX, newY)) {
                int newDistance = distances[currX][currY] + 1;

                if (distances[newX][newY] == -1 || newDistance < distances[newX][newY]) {
                    // 更新距离和父节点
                    distances[newX][newY] = newDistance;
                    parents[newX][newY].X = currX;
                    parents[newX][newY].Y = currY;

                    // 计算优先级（F值），使用曼哈顿距离作为启发函数
                    int priority = newDistance + manhattanDistance(newX, newY, targetX, targetY);
                    pq.push(std::make_pair(-priority, std::make_pair(newX, newY)));
                }
            }
        }
    }

    // 如果找到了最短可行路径，回溯路径，找到接下来需要移动到的坐标
    if (parents[targetX][targetY].X != -1 && parents[targetX][targetY].Y != -1) {
        struct Point current = {targetX, targetY};
        while (parents[current.X][current.Y].X != startX || parents[current.X][current.Y].Y != startY) {
            current = parents[current.X][current.Y];
        }
        ret = current;
    }

    // 释放动态分配的内存
    for (int i = 0; i < player->row_cnt; i++) {
        delete[] distances[i];
    }
    delete[] distances;

    for (int i = 0; i < player->row_cnt; i++) {
        delete[] parents[i];
    }
    delete[] parents;

    return ret;
}

struct Point walk(struct Player* player) {
    struct Point ret = {player->your_posx, player->your_posy};

    // 寻找最近的星星坐标
    struct Point nearestStar = findNearestStar(player);

    // 如果找到了星星
    if (nearestStar.X != -1 && nearestStar.Y != -1) {
        // 使用A*算法寻找最短可行路径
        struct Point nextMove = findShortestFeasiblePath(player, ret.X, ret.Y, nearestStar.X, nearestStar.Y);

        // 如果找到了最短可行路径
        if (nextMove.X != -1 && nextMove.Y != -1) {
            ret = nextMove;
        }
    }

    return ret;
}
