#include "../include/netHeader.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <strings.h>
#include <iostream>
using namespace std;

void Echo(int sock_fd)
{
    // 1.读取套接字上的数据，有可能一次读取不完整，需要循环读取。
    string buffer{};
    buffer.resize(BUFFSIZE);
    int nread{};
    int nwrite{};
    while (true)
    {
        nread = read(sock_fd, buffer.data(), BUFFSIZE);
        // 如果读取遭受中断。应该继续读取。
        if (nread < 0)
        {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EINPROGRESS)
            {
                return;
            }
            err_sys("Echo: read");
        }
        // 对端关闭连接，返回0.TCP流没有终止标志。
        else if (nread == 0)
        {
            break;
        }
        if ((nwrite = write(sock_fd, buffer.data(), nread)) < 0)
        {
            err_sys("Echo: write");
        }
    }
}

int main(/*int argc, char **argv*/)
{
    // 固定IP和端口号
    string server_ip{"127.0.0.1"};
    int server_port{SERV_PORT};

    // 1.创建文件描述符，用于数据读写。
    int server_socket;
    // 成功时返回fd（非负），失败返回-1
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_sys("socket error");
    }
    // 2.设置socket地址（使用ip和端口号唯一标识标识服务器socket地址）
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    // 失败返回0
    if (inet_pton(AF_INET, server_ip.data(), &server_addr.sin_addr.s_addr) <= 0)
    {
        err_quit("inet_pton error for");
    }

    if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
    {
        err_sys("bind error");
    }

    // 监听,LISTENQ:最大监听队列
    if (listen(server_socket, LISTENQ) < 0)
    {
        err_sys("listen error");
    }

    // 接受连接。
    cout << "准备开始服务： " << '\n';

    // string recv_buffer{};
    // recv_buffer.resize(BUFFSIZE);
    string send_buffer{};
    send_buffer.resize(BUFFSIZE);
    // 存储接已经受连接的客户端socket地址信息。
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int acceptfd;
    while (true)
    {

        if ((acceptfd = accept(server_socket, reinterpret_cast<sockaddr *>(&client_addr), &addr_len)) < 0)
        {
            err_sys("accept error");
        }
        // 打印已经接受的客户端地址信息（可选）：
        string client_ip{};
        client_ip.resize(INET_ADDRSTRLEN);
        if (inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, &client_ip[0], INET_ADDRSTRLEN) == nullptr)
        {
            err_quit("inet_ntop error");
        }
        cout << "新接受的客户端进程的ip地址是: " << client_ip << '\n';
        cout << "新接受的客户端进程的端口号是： " << ntohs(client_addr.sin_port) << '\n';

        //  fork一个子进程处理连接请求。
        int child_pid;
        if ((child_pid = fork()) < 0)
        {
            err_sys("fork");
        }
        if (child_pid == 0)
        {
            // 关闭 服务器用于连接请求的套接字（监听套接字）。
            close(server_socket);
            // 调用echo处理请求。
            Echo(acceptfd);
            // 当处理结束时，打印被处理的客户端的信息。
            cout << "客户端ip: " << client_ip << " 端口号：" << ntohs(client_addr.sin_port) << " 断开连接" << '\n';
            exit(0);
        }
        // 父进程关闭acceptfd套接字。
        close(acceptfd);
    }
}