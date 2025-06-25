#include <iostream>
#include "my_stl_vector.h"
#include "my_stl_alloc.h"
#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace std;

// void Vector_Test() {
//   MY_STL::vector<double, MY_STL::simple_alloc<double, MY_STL::alloc>> vec;
//   for (int i = 0; i < 10; i++) {
//     vec.push_back(i);
//   }
//   cout << "输出测试：" << endl;
//   cout << "size：" << vec.size() << endl;
//   for (int i = 0; i < vec.size(); ++ i) {
//     cout << vec[i] << " ";
//   }
//   cout << "\n";
// }

// TEST(vec_test_double, push_back) {
//   MY_STL::vector<double, MY_STL::simple_alloc<double, MY_STL::alloc>> vec;
//   vec.push_back(10);
//   ASSERT_EQ(vec.size(), 1);
//   ASSERT_EQ(vec.capacity(), 1);
// }

// TEST(vec_test_int, push_back) {
//   MY_STL::vector<int, MY_STL::simple_alloc<int, MY_STL::alloc>> vec;
//   vec.push_back(2);
//   ASSERT_EQ(vec.size(), 1);
//   ASSERT_EQ(vec.capacity(), 1);
// }

// TEST(vec_test_int, resize) {
//   MY_STL::vector<int> vec;
//   vec.resize(10);
//   ASSERT_EQ(vec.size(), 10);
//   // ASSERT_EQ(vec.capacity(), 1);
// }

class Vec_Test: public ::testing::Test {
public:
  // static void TearDownTestSuite() {
  static void TearDownTestCase() {
    cout << "tear down test suite\n" << std::endl;
  }
  // static void SetUpTestSuite() {
  static void SetUpTestCase() {
    cout << "set up test suite\n" << std::endl;
  }
  virtual void SetUp() override {
    cout << "set up\n" << std::endl;
  }
  virtual void TearDown()override {
    cout << "tear down\n" << std::endl;
  }
};

TEST_F(Vec_Test, push_back) {
  MY_STL::vector<int, MY_STL::simple_alloc<int, MY_STL::alloc>> vec;
  vec.push_back(12);
  ASSERT_EQ(vec.size(), 1);
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("This message should not be displayed!");

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}