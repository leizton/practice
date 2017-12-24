# make error: undefined reference to `net_util::newServerSocket(char const*, int, int, bool)
newServerSocket在头文件中的声明第2个参数是int, 但在.cc文件中实现是uint16_t, 所以出现未定义的链接错误

# connect fail, errno = 115
查看: /usr/include/asm-generic/errno.h, 115是EINPROGRESS
查看手册: http://man7.org/linux/man-pages/man2/connect.2.html, The socket is nonblocking and the connection cannot be completed immediately
@ref https://stackoverflow.com/questions/10204134/tcp-connect-error-115-operation-in-progress-what-is-the-cause

# accept fail, errno = 22
errno=22定义在/usr/include/asm-generic/errno-base.h
accept传入的addr_len必须初始化成sizeof(addr)