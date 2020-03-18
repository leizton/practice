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

  /*
  int reuse_opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_opt, sizeof(reuse_opt)) != 0) {
    cout << "set reuse fail" << endl;
    return -1;
  }
  //*/

  const char *ip = "0.0.0.0";
  const int port = 20001;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  if (::bind(fd, (sockaddr*)&addr, sizeof(addr)) != 0) {
    cout << "bind fail" << endl;
    return -1;
  }

  if (::listen(fd, 100) != 0) {
    cout << "listen fail" << endl;
    return -1;
  }

  char buf[1024];
  while (true) {
    socklen_t addr_len = 0;
    int client_fd = ::accept(fd, (sockaddr*)&addr, &addr_len);
    if (client_fd < 0) {
      cout << "accept fail" << endl;
      return -1;
    }
    ssize_t n_read = ::read(client_fd, buf, 1024);
    buf[n_read] = '\0';
    cout << "recv: " << buf << endl;
    ::write(client_fd, "server: ", strlen("server: "));
    ::write(client_fd, buf, n_read);
    close(client_fd);
    close(fd);
    break;
  }
}