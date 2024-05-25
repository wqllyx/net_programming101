/**
 * 用于各种预定义常量和头文件包含的文件。
  */
#pragma once

// man listen:如果 backlog 参数大于 /proc/sys/net/core/somaxconn 中的值，则它会默默地限制为该值
// 从Linux 5.4开始，该文件中的默认值是4096； 在早期的内核中，默认值为 128。在 Linux 之前 2.4.25 中，此限制是硬编码值 SOMAXCONN，值为 128。
#define LISTENQ 4096 /* 2nd argument to listen() */

/* Miscellaneous constants */
#define MAXLINE 4096  /* max text line length */
#define BUFFSIZE 8192 /* buffer size for reads and writes */

/* Define some port number that can be used for our examples */
#define SERV_PORT 9877               /* TCP and UDP */
#define SERV_PORT_STR "9877"         /* TCP and UDP */
#define UNIXSTR_PATH "/tmp/unix.str" /* Unix domain stream */
#define UNIXDG_PATH "/tmp/unix.dg"   /* Unix domain datagram */

/* Following shortens all the typecasts of pointer arguments: */
#define SA struct sockaddr

/* default file access permissions for new files */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* default permissions for new directories */
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int); /* for signal handlers */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))




// 错误处理
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);