#include "stl_alloc.h"
#include "stl_construct.h"

namespace MY_STL {

template <class T, class Alloc = alloc>
class vector {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type* iterator; // vector的迭代器就是普通指针
  typedef value_type& reference;
  typedef size_t size_type;
  typedef ptrdiff_t diffrence_type;

 protected:
  typedef simple_alloc<value_type, Alloc> data_alloctor;
  iterator start;
  iterator finish;
  iterator end_of_storage;
  void insert_aux(iterator position, const T& x); //! 后续需要实现
  void deallocate() {
    if (start) {
      data_alloctor::deallocate(start, end_of_storage - start);
    }
  }
  void fill_initialize(size_type n, const T& value) {
    start = allocate_and_fill(n, value);
    finish = start + n;
    end_of_storage = finish;
  }

 public:
  iterator begin() { return start; }
  iterator end() { return end; }
  size_type size() { return size_type(end) - begin(); }
  size_type capacity() const { return size_type(end_of_storage) - begin(); }
  bool empty() const { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }

  vector() : start(0), finish(0), end_of_storage(0) {}
  vector(size_type n, const T& value) { fill_initialize(n, value); }
  vector(int n, const T& value) { fill_initialize(n, value); }
  vector(long n, const T& value) { fill_initialize(n, value); }
  explicit vector(size_type n) { fill_initialize(n, T()); }

  ~vector() {
    destroy(start, finish);  // 根据traits类型决定是否调用析构函数
    deallocate();            // 释放空间
  }

  reference front() { return *begin(); }
  reference back() { return *(end() - 1); }
  void push_back(const T& value) {
    if (finish != end_of_storage) {
      construct(finish, value);
      ++finish;
    } else {
      insert_aux(end(), value);
    }
  }

  void pop_back() {
    --finish;
    destroy(finish);
  }

  iterator erase(iterator position) {
    // 位置合法性判断
    if (position + 1 != end) {
      // copy(position + 1, finish, position); //! copy需要实现
    }
    --finish;
    destroy(finish);
    return position;
  }
  void resize(size_type new_size, const T& x) {
    if (new_size < size()) {
      erase(begin() + new_size, end());
    } else {
      insert(end(), new_size - size(), x); //! 后续实现
    }
  }
  void resize(size_type new_size) { resize(new_size, T()); }
  void clear() { erase(begin(), end()); }

 protected:
  iterator allocate_and_fill(size_type n, const T& x) {
    iterator result = data_alloctor::allocate(n);
    uninitialized_fill_n(result, n, x);
    return result;
  }
};

};  // namespace MY_STL