# connect失败, errno是115
查看: /usr/include/asm-generic/errno.h, 115是EINPROGRESS
查看手册: http://man7.org/linux/man-pages/man2/connect.2.html, The socket is nonblocking and the connection cannot be completed immediately
@ref https://stackoverflow.com/questions/10204134/tcp-connect-error-115-operation-in-progress-what-is-the-cause