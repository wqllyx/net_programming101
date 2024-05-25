#include "netHeader.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <strings.h>
#include <iostream>
#include<ctime>
using namespace std;
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
    time_t ticks;
    while (true)
    {

        if ((acceptfd = accept(server_socket, reinterpret_cast<sockaddr *>(&client_addr), &addr_len)) < 0)
        {
            err_sys("accept error");
        }
        ticks = time(nullptr);
        // 打印已经接受的客户端地址信息（可选）：
        string client_ip{};
        client_ip.resize(INET_ADDRSTRLEN);
        if (inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, &client_ip[0], INET_ADDRSTRLEN) == nullptr)
        {
            err_quit("inet_ntop error");
        }
        cout << "新接受的客户端进程的ip地址是: " << client_ip << '\n';
        cout << "新接受的客户端进程的端口号是： " << ntohs(client_addr.sin_port) << '\n';





        // // 读取客户端发送的信息。
        // cout << "客户端发送的消息是：" << '\n';
        // int readN;
        // while ((readN = recv(acceptfd, &recv_buffer[0], BUFFSIZE, 0)) > 0)
        // {
        //     cout << recv_buffer.c_str();
        // }
        // cout << endl;

        // 向客户端发送时间
        
        snprintf(&send_buffer[0], send_buffer.size(), "%.24s\r\n", ctime(&ticks));
        send(acceptfd, send_buffer.c_str(), send_buffer.size(), 0);
    }
}