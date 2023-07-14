# Shell_Lab_Project

[toc]

# 实验：设计自己的shell

## 工作分配

无

## 题目

利用之前的知识，这个实验中我们要自己实现一个简单的 shell ——跟我们之前使用到的 bash shell 类似，它的功能是 **显示当前所在的目录，然后对输入的命令进行解释**。在目录下有一个文件`main_sol`，它是一个已经编译好的标程，会用来生成测试用的文件。你可以在目录下执行`make`后直接运行这个程序，查看它的效果——你的 shell 必须跟它的显示效果保持一致。

### 第一部分：`log_t`

在实现`shell`之前，你需要实现一个数据结构`log_t`——结构体的布局可以参见`log.h`头文件，它相当于一个用链表实现的栈，作用是保存输入的每一条命令以便查找。

在`log.h`中定义了这样几个函数，你需要全部实现它们：

- `void log_init(log_t *l);`初始化一个`log_t`——熟悉面向对象程序设计的同学应该能看出，这相当于是`log_t`的构造函数。你需要将`l`的`head`设置为`NULL`。

- `void log_destroy(log_t* l);`销毁一个`log_t`，相当于`log_t`的析构函数——你需要回收一个`log_t`所占用的所有内存，并将`l`的`head`设置为`NULL`，如果`head`已经为`NULL`则直接返回。

- `void log_push(log_t* l, const char *item);`把一个字符串`item`插入到一个`log_t`的末尾。如果`l`的`head`为`NULL`的话，你需要在这个函数中为`log_t`建立第一个结点并分配对应的内存空间。

- `char *log_search(log_t* l, const char *prefix);`遍历整个`log_t`的链表，搜索是否有包含前缀`prefix`存在的字符串，如果有的话则返回这个字符串（只返回第一个），没有的话直接返回`NULL`。

有 C++ 或者其他面向对象语言基础的同学可以看出，这一部分我们其实是在用 C 语言来进行面向对象编程——对于系统编程来说，很多情况下确实需要这种看起来有些“蹩脚”的编程方式，因此这里需要大家对此熟练掌握。

### 第二部分：shell

接下来，我们需要来实现 shell 本身——一个 shell 相当于一个无限循环的程序，它在循环中应该执行这些操作：

- 向标准输出打印一个命令提示符
- 从标准输入读取一个命令
- 判断要执行哪些命令

在这一步，你需要修改 shell.c 里的`prefix`函数和`execute`函数。接下来我们将依次介绍进一步的细节：

- 命令提示符

你需要在`prefix`函数里输出以下格式的命令提示符：

```
/path/to/cwd$
```

这里可以直接用`printf("%s$ ", cwd)`来实现——`cwd`部分为当前所在目录。你可以自行查阅文档，阅读关于`getcwd()`函数的相关信息。

- 从标准输入读取命令

你的程序要每次从标准输入读取一行，获取输入的命令——你可以查阅文档学习`getline()`函数的用法，这里要 **注意回收内存**，不然你将无法获得内存泄露检测的分数。这部分功能已经在 main.c 文件中实现了，你可以阅读这部分代码，但无法对其进行修改，最终测试时会将你所做的任何修改覆盖掉。读入后，会将命令作为参数传递给`execute`函数。

- 对内置命令进行解析

你需要在`execute`函数里对内置命令进行解析，实现如下几个命令：

#### 1）`cd`

```
cd xxx
```

进入某个目录（上述代码会进入xxx目录）——你可以查阅`chdir()`函数的用法。如果目录不存在，则要打印`xxx: No such file or directory`并换行，其中`xxx`表示输入的目录名称。

#### 2）`exit`

```
exit
```

退出 shell 程序，在`execute`函数中直接返回 $0$。在其他情况下，`execute`返回一个非零值即可。

#### 3）显示历史命令

```
!#
```

所有输入的命令都保存在一个`log_t`中，这个命令的作用是显示所有输入过的命令，每个占一行。**从栈底元素开始输出，`log_t`变量名为`Log`，定义在 shell.h 文件中，请勿修改。**

**注意，以!开头的所有命令都不会被放入命令栈中。**

#### 4）根据前缀查找命令

```
!prefix
```

查找是否曾经输入过包含前缀`prefix`的命令，如果找到（**如果有多个，只找最靠近栈顶的一个**），则执行这条命令，如果没有，则返回`No Match`换行。新执行的命令也会被放入栈顶。

#### 5）`ls`

```
ls
```

跟 bash shell 的`ls`命令一样，列举当年目录下所有子目录和文件——你可以直接用`system()`函数执行这个命令。

执行外部命令

你需要使用`fork()`和`exec()`等函数来执行一个外部命令——如果执行失败，则输出`%s: no such command`换行。注意，这里输入的命令采用的是 DOS 格式，而不是 Unix 格式——运行当前目录下可执行文件 **`args`** 应该直接输入 **`args`** 而非 **`./args`**，因此你 不能使用 **`system()`** 函数来执行该命令，否则将通不过判题测试。

> 文件夹下有一个`args.cpp`文件，你可以调用`g++`（注意不是`gcc`，因为是 C++ 代码）编译它，然后试着用你的 shell 来执行这个程序。这个程序会输出一行`Hello world!`然后依次输出它接收的每一个参数。

如果外部命令无法执行，则输出`XXX: no such command`并换行，其中`XXX`表示输入的完整外部命令。注意，你必须确保无论执行成功还是失败，都 **不要有额外的子进程留下**。

**即使外部命令未正确执行，也依然将这条命令放入命令栈中。**

### 本地判分

在`/include`文件夹内包含了 **头文件（header file）** ，头文件声明了函数和类；

在`/src`文件夹内含有具体的 **.c**，其中包含了对相关函数的实现——你需要实现的函数在`shell.c`和`log.c`中。

主函数保存在`main.c`中。**不要擅自修改主函数代码！** 否则可能会导致你的代码不能通过判题。

在自测时你可以在终端中输入`make`来编译执行本地判分器，输入`make clean`可以清空之前编译出的文件。其它的`make`指令可以在`Makefile`中查看。

编译出的可执行文件名为`main`，在运行`make`之后，你可以直接执行`./main`来直接运行你的 shell。

### 提交与在线判分

正如前面提到的，这次作业由在线判分器进行自动判分。

**在这次任务中，你只能修改 shell.c 和 log.c 文件，对其他文件进行的修改将会被覆盖。** 你不能通过`standard output`输出任何额外的内容（这意味着你 **不可以** 使用`printf`或类似的函数输出多余的信息），因为这会导致在线判分器判分失误从而影响你的成绩。在你提交作业后，自动判分器将针对你提交的内容进行正确性测试与内存测试——本地判题将只进行正确性测试，一共占 $45\%$ 的成绩。

在你已经通过正确性测试的前提条件下，你的程序接下来将会接受内存测试——如果没有内存泄露的话你将获得额外 $20\%$ 成绩。至此，你可以获得基础的 $65\%$ 的成绩。

在文件夹下有一个空的`fbomb.txt`，输入任意内容让该文件不为空，就可以进行`fork bomb`测试——通过测试后，你将获得剩余的 $35\%$ 的成绩。

注意，你必须确保你的`fork`实现是正确的——如果产生了额外的进程，无法终止的话判题器将不能正常结束，被系统杀死，然后你将 **无法得到任何分数**。


## 实现

主要实现`log.c`和`shell.c`:


### log.c

```c
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

```

### shell.c

```c
#include "shell.h"
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

```

***注意内存泄漏……***

***感谢组内所有成员***