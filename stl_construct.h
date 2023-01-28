#ifndef MY_STL_CONSTRUCT_H
#define MY_STL_CONSTRUCT_H

#include <new.h>

//*******************construct*****************//
template <class T1>
inline void constuct(T1* p,) {
    new (p) T1();
}

template <class T1, class T2>
inline void constuct(T1* p, const T2& value) {
    new (p) T1(value); // 调用定位new，就是在p指向的地址，调用T1构造函数
}

//*******************destroy*****************//
// 第一个版本，只接受一个指针
template <class T>
inline void destroy(T* pointer) {
    pointer->~T();
}

// 第二个版本，接受两个迭代器，然后找出迭代器所指对象的类型，然后根据类型采取最佳destroy措施
template <class ForWardIterator>
inline void __destroy_aux(ForWardIterator first,ForWardIterator last,__true_type) {}

template <class ForWardIterator>
inline void __destroy_aux(ForWardIterator first,ForWardIterator last,__false_type) {
    for (; first < last; ++first) {
        destroy(*first);
    }
}

template <class ForWardIterator, class T>
inline void __destroy(ForWardIterator first,ForWardIterator last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first,last,trivial_destructor());
}

template <class ForWardIterator>
inline void destroy(ForWardIterator first,ForWardIterator last) {
    __destroy(first,last,value_type(first));
}

//特化版本:
inline void destroy(char*,char*){}
inline void destroy(double*,double*){}
inline void destroy(float*,float*){}
inline void destroy(wchar_t*,wchar_t*){}
inline void destroy(int*,int*){}


#endif // MY_STL_CONSTRUCT_H