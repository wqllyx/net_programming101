/** 提供用于写入字节流套接字的包裹函数
 * fd用于读写的文件描述符
 * vptr：用于定位读取位置。
 * n:要读取的字节数。
 * return:成功，返回读取的字节数。失败返回-1。。
 */

#include <unistd.h>
#include <cerrno>

ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    const char *ptr = (char *)vptr;

    while (nleft > 0)
    {
        // write函数，失败返回-1.
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}