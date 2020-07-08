#include "util/base.h"


#if comment(SFINAE)
SFINAE: substitution failure is not an error, 替换失败未必是一个错误
当将 模板形参 替换为某个 显式指定或推导出的类型 失败时, 从重载集中丢弃这个特化, 而非导致编译失败, 因为还可能命中其他重载
#endif


#if comment(enable_if)
// 利用SFINAE根据条件选择合适的函数重载或特化的实现
template<bool B, class T=void> struct enable_if {};
template<true, class T> struct enable_if { typedef T type; };
利用SFINAE时, 一般enable_if<b>::type都是void, b只是用来选择编译哪段代码
#endif
struct EnableIfTest {
  template<class T>
  static typename enable_if<is_integral<T>::value>::type  // 'type' is void. enable_if<B, T=void>
  test(const string& s) {
    cout << "i" << std::stoi(s) << endl;
  }

  template<class T>
  static typename enable_if<is_floating_point<T>::value>::type  // void
  test(const string& s) {
    cout << "f" << std::stof(s) << endl;
  }
};
def(enable_if) {
  EnableIfTest::test<int>("123.45");
  EnableIfTest::test<float>("123.45");
}


template<class T>
struct NestVecElemType {
  using EType = T;
};
template<class T>
struct NestVecElemType<std::vector<T>> {
  using EType = typename NestVecElemType<T>::EType;
};
def(nest_vec_elem_type) {
  vector<vector<vector<int>>> v1;
  vector<vector<vector<float>>> v2;
  assert_T(std::is_integral<NestVecElemType<decltype(v1)>::EType>::value);
  assert_T(std::is_floating_point<NestVecElemType<decltype(v2)>::EType>::value);
}


main_run;