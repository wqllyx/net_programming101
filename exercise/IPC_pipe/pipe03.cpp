#include <unistd.h>
#include <string>
#include <iostream>
#include <climits>
#include <csignal>
#include <fstream>
#include <netHeader.h>
#include <sys/wait.h>
#include<cstring>
#include "netHeader.h"
using namespace std;
#define DEF_PAGER "/usr/bin/less"
/**编写一个分页程序，每次一页显示输出
 */

int main(int argc, char const *argv[])
{

    if (argc != 2)
        err_quit("需要文件名");

    int fd[2];
    if (pipe(fd) < 0)
        err_sys("pipe error");

    fstream fs;
    // 以只读模式打开文件
    fs.open(argv[1], std::ios::in);

    pid_t pid;
    if ((pid = fork()) < 0)
        err_sys("fork error");
    // 父进程:负责将文件内容读出并写入管道
    else if (pid > 0)
    {
        close(fd[0]);
        string buffer;
        if (fs.is_open())
        {
            while (getline(fs, buffer))
            {
                std::streamsize len = buffer.size();
                // 补一个换行符。
                buffer[len] = '\n';
                if (write(fd[1], buffer.data(), len+1) != len+1)
                    err_sys("write");
                // char LF = '\n';
                // char *L = &LF;
                // write(fd[1], L, 1);
            }
            if (fs.bad())
                err_sys("getline");
            close(fd[1]);
            if (waitpid(pid, NULL, 0) < 0)
                err_sys("waitpid ");
            exit(0);
        }
    }
    // 孩子负责把数据读出，并调用分页程序写到标准输出
    else
    {
        close(fd[1]);
        if (fd[0] != STDIN_FILENO)
        {
            if (dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO)
                err_sys("dup2 error");
            close(fd[0]);
        }
        char *argv0;
        char *pager;
        if (!(pager = getenv("PAGER")))
            pager = (char *)DEF_PAGER;
        if ((argv0 = strchr(pager,'/')) != nullptr)
            argv0++;
        else
            argv0 = pager;
        if (execl(pager,argv0,(char*)0) < 0)
            err_sys("execl");
    }
    return 0;
}
