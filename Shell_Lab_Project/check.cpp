#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
string standard="no leaks are possible";
string filename="output/valgrind_test.txt";
int counter=0;
int correct=0;
void case_gen(const char* testcase,const char* part)
{
    pid_t p1 = fork();
    if (p1 == 0) {          //child process
        int fd = open(part, O_RDWR | O_CLOEXEC | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd < 0) {
            perror("Failed to open txt file ");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        execlp("./main_sol", "./main_sol", testcase,NULL);
        perror("Execlp Failed: ");    //this line is not supposed to be executed
        exit(1);                      //this line is not supposed to be executed
    }
    cout<<"case generated"<<endl;
}
int case_test(const char* testcase,const char* part,const char* sol)
{
    counter++;
    pid_t p1 = fork();
    if (p1 == 0) {          //child process
        int fd = open(part, O_RDWR | O_CLOEXEC | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd < 0) {
            perror("Failed to open txt file ");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        execlp("./main", "./main", testcase,NULL);
        perror("Execlp Failed: ");    //this line is not supposed to be executed
        exit(1);                      //this line is not supposed to be executed
    } else if (p1 > 0) {       //parent process
        int status;
        pid_t w_result = waitpid(p1, &status, 0);
        if (w_result == -1) {
            perror("Failed to wait");
            exit(1);
        }
        pid_t p2 = fork();
        if (p2 == 0) {
            int fd_null = open("/dev/null", O_RDWR);
            dup2(fd_null, STDOUT_FILENO);
            execlp("diff", "diff", part, sol, NULL);
            perror("Execlp diff Failed: ");    //this line is not supposed to be executed
            exit(1);
        } else if (p2 > 0) {
            int status2;
            waitpid(p2, &status2, 0);
            if (WIFEXITED(status2)) {
                if (WEXITSTATUS(status2) != 0) {
                    cout<<"测评用例 "<<counter<<"：错误"<<endl;

                    return 0;
                } else{
                    cout<<"测评用例 "<<counter<<"：正确"<<endl;
                    correct++;
                    return 1;
                }
            }
        } else{
            perror("Failed to fork");
            exit(1);
        }
    } else{
        perror("Failed to fork");
        exit(1);
    }
    return 0;
}

int mem_test(){

    pid_t p1=fork();
    if(p1==0){
        int fd=open("output/valgrind_test.txt",O_RDWR | O_CLOEXEC | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG);
        if(fd<0){
            perror("Failed to open txt file");
            exit(1);
        }
        int fd_null=open("/dev/null",O_RDWR);
        if(fd_null<0){
            perror("Failed to open txt file");
            exit(1);
        }
        dup2(fd_null,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
        execlp("valgrind","valgrind","./main","testcase1.in",NULL);
        perror("Execlp failed:");
        exit(1);
    }
    else if(p1>0){
        int status;
        pid_t w_result=waitpid(p1,&status,0);
        if(w_result==-1){
            perror("Failed to wait");
            exit(1);
        }
        ifstream file;
        file.open(filename,ios::in|ios::out);
        if(file.fail()){
            perror("failed to open valgrind text files");
            exit(1);
        }
        string line((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
        bool flag=false;
        if(line.find(standard)!=line.npos){
//                cout<<"get!"<<endl;
//		cout<<line<<endl;
		    flag=true;
        }
        file.close();
        if(flag){
            cout<<"Memory Test passed"<<endl;
	    return 1;
        }
        else{
            cout<<"Memory Test FAILED"<<endl;
            return 0;
        }

    }

    else{
        perror("Failed to fork");
        exit(1);
    }
    return 0;
}


int main()
{

    case_gen("testcase1.in","output/part1_sol.txt");
    case_gen("testcase2.in","output/part2_sol.txt");
    case_gen("testcase3.in","output/part3_sol.txt");
    int pts = 0;
    cout<<"测评信息"<<endl;
    cout<<"================================================"<<endl;

    pts = (case_test("testcase1.in","output/part1.txt","output/part1_sol.txt")*15);
    pts += (case_test("testcase2.in","output/part2.txt","output/part2_sol.txt")*15);
    pts += (case_test("testcase3.in","output/part3.txt","output/part3_sol.txt")*15);
    cout<<"你可以打开 testcaseX.in 查看对应测试用例的详细情况"<<endl;
    cout<<"测评结果"<<endl;
    cout<<"==================================="<<endl;
    cout<<"共计 "<<counter<<" 个测例，通过 "<<correct<<"个。"<<endl;
    cout<<pts<<endl;
    cout<<"注意，能通过本地测试不代表能通过线上测试"<<endl;
    return 0;

}

