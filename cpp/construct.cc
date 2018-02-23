#include "common/header.h"

class Resource;
Resource getRes(int fd);
void destory(int fd);

class Resource {
private:
    shared_ptr<void> ptr;
    int fd;
public:
    string resName;

public:
    Resource()
            :fd(-1), ptr(nullptr) {
        printf("con-0 \n");
    }

    Resource(int _fd)
            : fd(_fd),
              ptr(nullptr, [_fd](void *p) { destory(_fd); }) {
        printf("con-1 %d\n", _fd);
    }

    Resource(int _fd1, int _fd2)
            : fd(-1),
              ptr(nullptr, [_fd1, _fd2](void* p) { destory(_fd1); destory(_fd2); }) {
        printf("con-2 %d, %d\n", _fd1, _fd2);
    }

    // 通过new方式调用其他构造函数, 而不是直接调用其他构造函数
    // 直接调用Resource(x)是在创建一个临时对象
    Resource(double x) {
        printf("con-3 %.2f\n", x);
        new (this) Resource((int)x);
    }

    ~Resource() {
        if (resName.empty()) {
            printf("decon %d\n", fd);
        } else {
            printf("decon %s\n", resName.data());
        }
    }

    // 拷贝构造
    Resource(const Resource& res) {
        printf("con& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    // 赋值运算符
    Resource& operator=(const Resource& res) {
        if (this == &res) {
            return *this;
        }
        printf("con= %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
        return *this;
    }

    // 移动构造
    Resource(const Resource&& res) {
        printf("con&& %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
    }

    // 移动赋值
    Resource& operator=(const Resource&& res) {
        if (this == &res) {
            return *this;
        }
        printf("con&= %d\n", res.fd);
        ptr = res.ptr;
        fd = res.fd;
        return *this;
    }

    void reset(int fd) {
        *this = getRes(fd);  // 调用移动赋值, 该语句结束后调用析构
        printf("reseted\n");
    }
};

void destory(int fd) {
    printf("close %d\n", fd);
}

Resource getRes(int fd) {
    return Resource(fd);
}

void test1() {
    printf("\n>> A\n");
    {
        // output: con-1
        // 没有调用移动构造(con&&)
        Resource res = getRes(1);
        // output: decon, close
        // 先执行Resource的析构函数体, 再执行成员字段ptr的析构函数
        // 因为ptr的构造函数先于Resource的构造函数体执行
    }

    printf("\n>> B\n");
    {
        // output: con-2
        Resource res(5,6);
        // output: decon, close 5, close 6
    }

    printf("\n>> C\n");
    {
        // output: con-3, con-1
        Resource res(2.53);
        // output: decon, close
    }

    printf("\n>> D\n");
    {
        // output: con-1(1)
        Resource res(1);
        printf("to reset\n");
        // output: con-1(2), con&=
        res.reset(2);
        // output: close(1), decon(1), reseted, decon(2), close(2)
    }

    printf("\n>> E\n");
    // output: con-1. 没有调用移动构造
    Resource resX0 = getRes(3);
    resX0.resName = "resX0";

    printf("\n>> F\n");
    // output: con&
    // 此处不是调用赋值构造, 而是拷贝构造
    // 因为对象resX1还未创建, 不能调用赋值构造
    Resource resX1 = resX0;
    resX1.resName = "resX1";

    printf("\n>> G\n");
    // output: con&
    Resource resX2(resX0);
    resX2.resName = "resX2";

    printf("\n>> H\n");
    // output: con-0
    Resource resX3;
    // output: con=. 普通赋值
    resX3 = resX0;
    resX3.resName = "resX3";

    printf("\n>> I\n");
    // output: decon(resX3), decon(resX2), decon(resX1), decon(resX0) close
    // 由于ptr是shared_ptr, 所以只close一次
}

class Book {
public:
    Book(const char* name) {
        printf("book: %s\n", name);
    }
};

class Foo {
private:
    Book b1;
    Book b2;

public:
    Foo(const char* _b1, const char* _b2)
        : b2(_b2), b1(_b1) {
        // b1,b2的构造顺序与声明的顺序有关, 与初始化列表的顺序无关
    }
};

void test2() {
    Foo foo("x1", "x2");
}

int main() {
    test1();
    return 0;
}