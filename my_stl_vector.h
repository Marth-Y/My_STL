#include "my_stl_alloc.h"
#include "my_stl_construct.h"
#include "my_stl_uninitialized.h"

#include <iostream>

namespace MY_STL {

template <class T, class Alloc = alloc>
class vector {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type* iterator;  // vector的迭代器就是普通指针
  typedef value_type& reference;
  typedef size_t size_type;
  typedef ptrdiff_t diffrence_type;

 protected:
  typedef simple_alloc<value_type, Alloc> data_alloctor;
  iterator start;
  iterator finish;
  iterator end_of_storage;
  void insert_aux(iterator position, const T& x);
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
  iterator end() { return finish; }
  size_type size() { return size_type(end() - begin()); }
  size_type capacity() const { return size_type(end_of_storage) - begin(); }
  bool empty() const { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }
  void insert(iterator position, size_type n, const T& x);

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
      MY_STL::construct(finish, value);
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
      insert(end(), new_size - size(), x);  //! 后续实现
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
 private:
  void Print(iterator first, iterator last);
};

template <class T, class Alloc>
void vector<T, Alloc>::Print(iterator first, iterator last) {
  std::cout << "------------------------------------ \n";
  for (;first != last; ++ first) {
    std::cout << "------ " << *first << std::endl;
  }
  std::cout << "------------------------------------ \n";
}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
  if (finish != end_of_storage) {
    // 在备用空间起始处构造一个元素，并以vector末尾元素作为初值
    construct(finish, *(finish - 1));
    ++finish;
    T x_copy = x;
    // copy_backward(position, finish - 2, finish - 1);  //! 尚未实现
    *position = x_copy;
  } else {
    const size_type old_size = size();
    const size_type len = old_size != 0 ? 2 * old_size : 1;
    iterator new_start = data_alloctor::allocate(len);
    iterator new_finish = new_start;
    try {
      // 分两段拷贝构建过来。
      // 1. 将插入位置position前的元素拷贝过来
      new_finish =
          uninitialized_copy(start, position, new_start);
      // 2. 在插入的位置构建x
      construct(new_finish, x);
      ++new_finish;
      // 3. 将position后面的元素拷贝过来
      new_finish = uninitialized_copy(position, finish, new_finish);
    } catch (...) {
      destroy(new_start, new_finish);
      data_alloctor::deallocate(new_start, len);
      throw;
    }

    // 析构原vector
    destroy(begin(), end());
    deallocate();

    // 调整迭代器
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
  if (n != 0) {
    if (size_type(end_of_storage - finish) >= n) {
      T x_copy = x;
      // 计算插入点之后现有元素的个数
      const size_type elems_after = finish - position;
      iterator old_finish = finish;
      // 插入点之后现有元素个数大于新增元素个数
      if (elems_after > n) {
        //? 我很疑惑这里为什么要分成两段拷贝?
        // 1. 拷贝最后n个元素到finish的位置
        uninitialized_copy(finish - n, finish, finish);
        finish += n;
        // 2. 拷贝最后position到old_finish - n的元素到old_finish
        // 即：将position~finish的元素分成了两段：1. finish - n ~ finish   2. position ~ finish - n
        // copy_backward(position, old_finish - n, old_finish);  //! 尚未实现
        // 3. 填充position ~ position + n的元素
        // fill(position, position + n, x_copy);                 //! 尚未实现
      } else {
        // 插入点之后的现有元素小于等于新增元素个数
        uninitialized_fill_n(finish, n - elems_after, x_copy);
        finish += n - elems_after;
        uninitialized_copy(position, old_finish, finish);
        finish += elems_after;
        // fill(position, old_finish, x_copy); //! 尚未实现
      }
    } else {
      const size_type old_size = size();
      const size_type len = old_size + std::max(old_size, n);
      iterator new_start = data_alloctor::allocate(len);
      iterator new_finish = new_start;
      try
      {
        new_finish = uninitialized_copy(start, position, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, x);
        new_finish = uninitialized_copy(position, end, new_finish);
      }
      catch(...)
      {
        // commit or rollback
        destroy(new_start, new_finish);
        data_alloctor::deallocate(new_start, new_finish);
        throw;
      }
      // 释放旧的空间
      destroy(start, finish);
      deallocate();
      // 调整新的指针
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }
}

};  // namespace MY_STL