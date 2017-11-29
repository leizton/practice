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

    Resource(int _fd1, int _fd2)
            : fd(-1),
              ptr(nullptr, [_fd1, _fd2](void* p) { destory(_fd1); destory(_fd2); }) {
        printf("con %d, %d\n", _fd1, _fd2);
    }

    Resource(int _fd) : fd(_fd) {
        int fd = this->fd;
        ptr = shared_ptr<void>(nullptr, [fd](void *p) { destory(fd); });
        printf("con %d\n", fd);
    }

    // 通过new方式调用其他构造函数
    // 不可以用 Resource((int)x), 这是在创建一个临时对象
    Resource(double x) {
        printf("call other con %.2f\n", x);
        new (this) Resource((int)x);
    }

    ~Resource() {
        printf("decon %d\n", fd);
    }

    Resource(const Resource&& res) {
        printf("con&& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    Resource(const Resource& res) {
        printf("con& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    Resource& operator=(const Resource& res) {
        printf("con= %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
        return *this;
    }
};

Resource getRes(int fd) {
    return Resource(fd);
}

int main() {
    printf("\n>>\n");
    { Resource res = getRes(1); }

    printf("\n>>\n");
    { Resource res(5,6); }

    printf("\n>>\n");
    { Resource res(2.53); }

    printf("\n>>\n");
    Resource resX = getRes(3);

    printf("\n>> con&\n");
    Resource resX1 = resX;
    Resource resX2(resX);

    printf("\n>> con=\n");
    Resource resX3;
    resX3 = resX;

    printf("\n>> return\n");
    return 0;
}