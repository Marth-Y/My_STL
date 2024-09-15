#ifndef MY_STL_UNINITIALIZED_H
#define MY_STL_UNINITIALIZED_H

#include "my_stl_construct.h"

namespace MY_STL {

// uninitialized_copy使我们能够将内存的配置与对象的构造行为分离开，
// 如果作为输出目的地，[result,result + (last - first))范围内的每一个
// 迭代器都指向未初始化的区域，则uninitialized_copy会使用拷贝构造
// 给身为输入来源之[first,last)的每一个对象产生一份复制品，放进输出范围中，
// 换句话说。针对输入范围内的每一个迭代器 i ，该函数会调用拷贝构造函数产生一个
// 复制品，放置于输出范围的相对位置上。
// C++标准规定，uninitialized_copy具有“commit or
// rollback”属性，要么就构造出所有元素，要么（任何一个拷贝构造失败时）就不构造任何东西
template <class InputIterator, class ForWardIterator>
ForWardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                   ForWardIterator result) {
  return __uninitialized_copy(first, last,result, velue_type(result));
}

template <class ForWardIterator, class T>
void uninitialized_fill(ForWardIterator first, ForWardIterator last,
                        const T& x);
// 在迭代器范围内，产生x的复制品，即调用x的拷贝构造
// 同uninitialized_copy 如果有任何一个，构造失败。则需要将已产生的所有元素析构掉

template <class ForWardIterator, class Size, class T>
inline ForWardIterator __uninitialized_fill_n_aux(ForWardIterator first, Size n,
                                                  const T& x, __true_type) {
  return fill_n(first, n, x);  // 交由高阶函数执行
}

template <class ForWardIterator, class Size, class T>
inline ForWardIterator __uninitialized_fill_n_aux(ForWardIterator first, Size n,
                                                  const T& x, __false_type) {
  ForWardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    constuct(&*cur, x);
  }
  return cur;
}

template <class ForWardIterator, class Size, class T, class T1>
inline ForWardIterator __uninitialized_fill_n(ForWardIterator first, Size n,
                                              const T& x, T1*) {
  // type_traits<>技法
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForWardIterator, class Size, class T>
inline ForWardIterator uninitialized_fill_n(ForWardIterator first, Size n,
                                            const T& x) {
  // 利用value_type取出first的类型，判断是否是POD类型
  return __uninitialized_fill_n(first, n, x, value_type(first));
}
};  // namespace MY_STL

#endif