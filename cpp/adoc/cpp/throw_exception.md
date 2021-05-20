~~~c++

void fn() throw();           // 可能抛出任意类型的异常
void fn() throw(int);        // 只抛出 int 类型
void fn() throw(int, char);  // 只抛出 int/char 类型

void fn() noexcept;          // 不抛任何异常