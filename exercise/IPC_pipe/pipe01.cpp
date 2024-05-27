#include <unistd.h>
#include <string>
#include <iostream>
#include <climits>
using namespace std;
/**测试当管道写端关闭时，read管道会返回0.
 * 当管道上无数据可读时，read阻塞。
 */
int main()
{
    string read_buffer;
    read_buffer.resize(4096);
    int p[2];
    pipe(p);
    int a = 10;
    while (true)
    {

        int read_ret;
        a--;
        // 测试从管道的读端获取数据，直到写端关闭。时，读返回0.
        write(p[1], "nihao", 6);
        if ((read_ret = read(p[0], read_buffer.data(), read_buffer.size())) == 0)
        {
            cout << "管道所有写端关闭，read返回" <<'\n';
        }
        else if (read_ret > 0)
        {
            cout << "正常,缓冲值为:" << read_buffer << '\n';
        }
        else
        {
            cout << "错误";
        }
        if (a == 5)
        {
            close(p[1]);
        }
        if (a == 0)
        {
            break;
        }
    }
}