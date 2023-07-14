/** @file log.c */
#include <stdlib.h>
#include <string.h>
#include "log.h"

/**
对log进行初始化，log的“构造函数”
 */

void log_init(log_t *l)
{
    l->head = NULL;
    return;
}

void node_destroy(node *p)
{
    if (p->next)
    {
        node_destroy(p->next);
    }
    free(p->cmd);
    free(p);
    return;
}

/**
销毁一个log，释放所有内存空间，log的“析构函数”
 */

void log_destroy(log_t *l)
{
    if (l->head != NULL)
    {
        node_destroy(l->head);
        l->head = NULL;
    }
    return;
}

/**
向log中推入一个字符串，你可以将log视为一个由链表组成的栈
 */

void log_push(log_t *l, const char *item)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->cmd = (char *)malloc(sizeof(char) * (strlen(item) + 1));
    strcpy(newNode->cmd, item);
    newNode->next = l->head;
    l->head = newNode;
    return;
}

/**
搜索log中是否含有对应前缀的字符串
 */

char *log_search(log_t *l, const char *prefix)
{
    log_t *pointer = l;
    while (pointer->head)
    {
        int i;
        for (i = 0; prefix[i] != '\0'; i++)
        {
            if (prefix[i] != pointer->head->cmd[i])
            {
                break;
            }
        }
        if (prefix[i] == '\0')
        {
            return pointer->head->cmd;
        }
        pointer->head = pointer->head->next;
    }
    return NULL;
}
