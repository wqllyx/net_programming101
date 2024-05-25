
#include "../include/netHeader.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <cstring>
#include <cassert>

using namespace std;

void str_cli(int sock_fd)
{
    // 1.从标准输入接受数据，发送到服务器套接字上。
    /**这个版本采用cin接受标准输入的数据。
     * cin的特点是，每次调用都会跳过开头的空白，然后读取遇到空白符时，结束此次读取。
     * 空白会被保留在标准输入的缓冲区中。如果下次读取的话没有忽略空白的机制，将会读取到上次遗留的空白。
     */
    string buffer{};
    int nwrite{};
    int nread{};
    char bufs[BUFFSIZE];
    bzero(bufs, sizeof(bufs));
    while (cin >> bufs)
    {
        // 2.将读取到的数据写入到scoket套接字中。

        if ((nwrite = write(sock_fd, bufs, sizeof(bufs))) < 0)
        {
            err_sys("str_cli: write");
        }
        // 3.从socket读取服务器回显的数据，一次读取可能不完整，需要持续读取。
        while (true)
        {
            bzero(bufs, sizeof(bufs));
            nread = read(sock_fd, bufs, BUFFSIZE);
            // 如果读取遭受中断。应该继续读取。
            if (nread < 0 && errno == EINTR)
            {
                continue;
            }
            // 读取出错。
            else if (nread < 0)
            {
                err_sys("str_cli: read");
            }
            // 达到空终止，读取结束
            else if (nread == 0)
            {
                break;
            }
            // 如果读取数小于BUFFSIZE，说明已经读取所有数据。
            if (nread <= BUFFSIZE)
            {
                // 4.将读取到的数据写到标准输出。
                cout << bufs << endl;
                break;
            }
        }
    }
}

int main()
{
    // 固定IP地址和端口号。
    string server_ip{"127.0.0.1"};
    unsigned short port{SERV_PORT};

    int client_fd;

    // 成功时返回fd（非负），失败返回-1
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
    str_cli(client_fd);
    close(client_fd);
}