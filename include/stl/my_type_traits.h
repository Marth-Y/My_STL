#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H

namespace MY_STL {

struct __true_type {};

struct __false_type {};

template <class _Tp>
struct __type_traits {
  typedef __true_type this_dummy_member_must_be_first;
  /* Do not remove this member. It informs a compiler which
     automatically specializes __type_traits that this
     __type_traits template is special. It just makes sure that
     things work if an implementation is using a template
     called __type_traits for something unrelated. */

  /* The following restrictions should be observed for the sake of
     compilers which automatically produce type specific specializations
     of this class:
         - You may reorder the members below if you wish
         - You may remove any of the members below if you wish
         - You must not rename members without making the corresponding
           name change in the compiler
         - Members you add will be treated like regular members unless
           you add the appropriate support in the compiler. */

  typedef __false_type has_trivial_default_constructor;
  typedef __false_type has_trivial_copy_constructor;
  typedef __false_type has_trivial_assignment_operator;
  typedef __false_type has_trivial_destructor;
  typedef __false_type is_POD_type;
};

template <>
struct __type_traits<int> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
struct __type_traits<double> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

// 对该类进行偏特化，就可以区分出traits类
};  // namespace MY_STL

#endif