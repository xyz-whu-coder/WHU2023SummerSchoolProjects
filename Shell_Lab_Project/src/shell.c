#include "../include/shell.h"
/**
 * shell的入口
 */
void prefix()
{
    char cwd[MAX_SIZE];
    getcwd(cwd, sizeof(cwd));
    printf("%s$ ", cwd);
    return;
}

void searchHistory(node *p)
{
    if (p == NULL)
    {
        return;
    }
    searchHistory(p->next);
    printf("%s\n", p->cmd);
    return;
}

int listDirectory()
{
    log_push(&Log, "ls");
    system("ls");
    return 1;
}

int printHistory(node *p)
{
    if (p == NULL)
    {
        return 1;
    }
    // log_push(&Log, "!#");
    node *pointer = p;
    printHistory(pointer->next);
    printf("%s\n", pointer->cmd);
    return 1;
}

int changeDirectory(char *buffer)
{
    char directory[MAX_SIZE];
    strcpy(directory, buffer + 3);
    if (chdir(directory))
    {
        printf("%s: No such file or directory\n", buffer + 3);
    }
    log_push(&Log, buffer);
    return 1;
}

int findCommandByPrefix(char *buffer)
{
    char *goal = log_search(&Log, buffer + 1);
    if (goal)
    {
        log_push(&Log, goal);
        execute(goal);
    }
    else
    {
        printf("No Match\n");
    }
    free(goal);
    return 1;
}

int executeExternalCommand(char *buffer)
{
    int count = 0;
    char *argv[MAX_SIZE], argc[MAX_SIZE];
    char space[] = " ";
    char *token;
    token = strtok(buffer, space);
    argv[count++] = token;
    strcpy(argc, "./");
    strcat(argc, token);
    while (token)
    {
        token = strtok(NULL, space);
        argv[count] = token;
        count++;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execvp(argc, argv) < 0)
        {
            printf("%s: no such command\n", buffer);
        }
        exit(1);
    }
    else
    {
        log_push(&Log, buffer);
        wait(&pid);
    }
    return 1;
}

int execute(char *buffer)
{
    if (strcmp(buffer, "exit") == 0)
    {
        return 0;
    }
    if (strcmp(buffer, "ls") == 0)
    {
        return listDirectory();
    }
    if (buffer[0] == 'c' && buffer[1] == 'd')
    {
        return changeDirectory(buffer);
    }

    if (strcmp(buffer, "!#") == 0)
    {
        return printHistory(Log.head);
    }
    if (buffer[0] == '!' && buffer[1] != '#')
    {
        return findCommandByPrefix(buffer);
    }
    return executeExternalCommand(buffer);
}
