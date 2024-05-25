/** 提供用于读取字节流套接字的包裹函数
 * fd用于读写的文件描述符
 * vptr：用于定位读取位置。
 * n:要读取的字节数。
 * return:成功，返回读取的字节数。失败返回-1。。
 */
#include <unistd.h>
#include <cerrno>
ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *ptr;
    ptr = (char *)vptr;
    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if (nread == 0) // EOF(这里网络的EOF可以自定义，也可以表对端关闭，也会返回0  )
            break;
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}