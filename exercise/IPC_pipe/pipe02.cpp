#include <unistd.h>
#include <string>
#include <iostream>
#include <climits>
#include <csignal>

using std::cout;
/**测试当管道读端关闭时，write管道会产生信号SIGPIPE.
 * 如果捕获或者忽略该信号write返回-1，
 * errno设置为EPIPE
 */

void sig_pipe_handler(int signo)
{
    if (signo == SIGPIPE)
    {
        cout << "内核发出的SIGPIPE，在这里捕获，输出此条信息"<<'\n';
    }
}
int main()
{
    int p[2];
    pipe(p);

    // 测试1：直接将读端关闭，。尝试写，此时会发出SIGPIPE信号（默认动作时终止进程。）。如果不重新设置信号的默认行为
    close(p[0]);
    if (signal(SIGPIPE, sig_pipe_handler) == SIG_ERR)
    {
        cout << "捕捉SIGPIPE信号失败"<< '\n';
    }
    int w_ret;
    if ((w_ret = write(p[1], "123", 4)) == -1)
    {
        cout << "捕获SIGPIPE信号后，write会返回-1，输出这条信息"<<'\n';
    }
    // 此处会终止程序 (broken pipe)
    cout << "如果未捕捉或者忽略SIGPIPE信号，不会输出这句话。否则你讲看到这句话"<<'\n';
}