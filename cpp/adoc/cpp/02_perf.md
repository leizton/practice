--------------------------------------------------------------------------------------------------------------
# std::locale 多线程下的性能问题
往往是使用 ostringsteam 引起的
~~~c++
"include/std/ostream"::ostringsteam::<<(long double 等数值类型)
  -> ostringsteam::_M_insert()
  -> "local_facets"::num_put::put() 拷贝ostringsteam基类ios_base的locale成员变量

locale::locale(const locale& __other)
    : _M_impl(__other._M_impl) {
  _M_impl->_M_add_reference();
}
对other是左值引用, 因此构造函数返回前other不会析构, 所以拷贝_M_impl和增加引用计数分开执行没有线程安全问题
~~~

[ref](https://www.bookstack.cn/read/tendis-2.1-zh/05ede674d6993738.md)
[ref](./doc/ostringstream多线程下性能问题分析.pdf)
