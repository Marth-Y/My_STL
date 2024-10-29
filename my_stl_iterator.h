#ifndef __MY_STL_ITERATOR_H__
#define __MY_STL_ITERATOR_H__

#include <stddef.h>

namespace MY_STL {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 为了避免自定义容器忘记实现某个迭代器，使用继承的方式避免遗漏
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
  typedef Category iterator_category;
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
};

// 榨汁机 traist
template <class Iterator>
struct iterator_traits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
};

// 原生指针的偏特化
template <class T>
struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& reference;
};

// const的偏特化版本
template <class T>
struct iterator_traits<const T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& reference;
};

// 这个函数可以方便的决定某个迭代器的类型(category)
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
  typedef typename iterator_traits<Iterator>::iterator_category category;
  return category();
}

// 这个函数可以方便的决定某个迭代器的diffrence_type
template <class Iterator>
inline typename iterator_traits<Iterator>::diffrence_type* distance_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::diffrence_type*>(0);
}

// 这个函数可以方便的决定某个迭代器的value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type* velue_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// distance函数实现
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(
    InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(
    RandomAccessIterator first, RandomAccessIterator last,
    random_access_iterator_tag) {
  typename iterator_traits<RandomAccessIterator>::difference_type n = 0;
  return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(
    InputIterator first, InputIterator last) {
  typedef typename iterator_traits<InputIterator>::iterator_category category;
  return __distance(first, last, category());
}

// advance函数实现
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
  while (n--) ++i;
}

template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n,
                      bidirectional_iterator_tag) {
  if (n >= 0)
    while (n--) ++i;
  else
    while (n++) --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n,
                      random_access_iterator_tag) {
  i += n;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
  __advance(i, n, iterator_category(i));
}
};  // namespace MY_STL

#endif
