
#include "../include/netHeader.h"
#include<unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <cstring>
#include <cassert>
using namespace std;

int main()
{
    // 固定IP地址和端口号。
    string server_ip{"127.0.0.1"};
    unsigned short port{SERV_PORT};

    int client_fd;

    //成功时返回fd（非负），失败返回-1
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_sys("socket error");
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    // 要连接的服务器地址。
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // 失败返回0
    if (inet_pton(AF_INET, server_ip.data(), &server_addr.sin_addr.s_addr) <= 0)
    {
        err_quit("inet_pton error for");
    }
    // 向服务器发起连接:成功时返回0，失败返回-1
    if (connect(client_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
    {
        err_sys("connect error");
    }

    // 收发数据
    int nBytes;
    // 缓冲区
    string send_buffer{};
    send_buffer.resize(BUFFSIZE);
    string recv_buffer{};
    recv_buffer.resize(BUFFSIZE);
    /* c_str：返回一个null终止const字符数组，修改其内容未定义，
    c++17q前要想修改：使用&str[0]的形式。c++17后使用data的非const重载版本 */
    while ((nBytes = recv(client_fd,&recv_buffer[0],recv_buffer.size(),0)) > 0) // when还能成功读到数据：
    {
        cout << recv_buffer << '\n';
    }
    
    if ( nBytes < 0 )
    {
        err_sys("recv error");
    }
    // 引用计数减一，为0时关闭。
    close(client_fd);
    exit(0);
}