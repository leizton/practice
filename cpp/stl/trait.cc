#include "util/base.h"


#if comment(SFINAE)
SFINAE: substitution failure is not a error, 替换失败未必是一个错误
当将 模板形参 替换为 显式指定或推导出的类型 失败时, 从重载集中丢弃这个特化, 而非导致编译失败
#endif


run() {
}


main_run;