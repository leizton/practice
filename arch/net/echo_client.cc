#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd < 0) {
    cout << "open socket fail" << endl;
    return -1;
  }

  const char *ip = "127.0.0.1";
  const int port = 20001;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  if (::connect(fd, (sockaddr*)&addr, sizeof(addr)) != 0) {
    cout << "connect fail" << endl;
    return -1;
  }

  char buf[1024] = "hello";
  ::write(fd, buf, strlen(buf));

  char rbuf[1024];
  ssize_t n;
  n = ::read(fd, rbuf, 1024);
  rbuf[n] = '\0';
  cout << rbuf << endl;
  n = ::read(fd, rbuf, 1024);
  rbuf[n] = '\0';
  cout << rbuf << endl;

  int i;
  cin >> i;
}