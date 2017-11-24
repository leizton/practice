#include "common/header.h"

void destory(int fd) {
    printf("close %d\n", fd);
}

class Resource {
private:
    shared_ptr<void> ptr;
    int fd;

public:
    Resource() :fd(-1), ptr(nullptr) {}
    Resource(int _fd) : fd(_fd), ptr(nullptr, [_fd](void *p) { destory(_fd); }) { printf("con %d\n", fd); }
    ~Resource() { printf("decon %d\n", fd); }

    Resource(const Resource&& res) {
        printf("con-&& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    Resource(const Resource& res) {
        printf("con-& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    Resource& operator=(const Resource& res) {
        printf("con-= %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
        return *this;
    }
};

Resource getRes(int fd) {
    return Resource(fd);
}

int main() {
    {
        Resource res1 = getRes(1);
    }
    printf("\n");

    Resource res2 = getRes(2);
    Resource res21 = res2;
    Resource res22(res2);

    Resource res23;
    res23 = res2;
    return 0;
}