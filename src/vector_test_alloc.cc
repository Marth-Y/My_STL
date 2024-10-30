#include <iostream>
#include "my_stl_vector.h"
#include "my_stl_alloc.h"
#include "gtest/gtest.h"

using namespace std;

void Vector_Test() {
  MY_STL::vector<double, MY_STL::simple_alloc<double, MY_STL::alloc>> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }
  cout << "输出测试：" << endl;
  cout << "size：" << vec.size() << endl;
  for (int i = 0; i < vec.size(); ++ i) {
    cout << vec[i] << " ";
  }
  cout << "\n";
}

TEST(vec_test_double, push_back) {
  MY_STL::vector<double, MY_STL::simple_alloc<double, MY_STL::alloc>> vec;
  vec.push_back(10);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec.capacity(), 1);
}

TEST(vec_test_int, push_back) {
  MY_STL::vector<int, MY_STL::simple_alloc<int, MY_STL::alloc>> vec;
  vec.push_back(2);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec.capacity(), 1);
}

TEST(vec_test_int, resize) {
  MY_STL::vector<int> vec;
  vec.resize(10);
  ASSERT_EQ(vec.size(), 10);
  // ASSERT_EQ(vec.capacity(), 1);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}