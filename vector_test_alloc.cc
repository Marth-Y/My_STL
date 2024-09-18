#include <iostream>
#include "my_stl_vector.h"

#include "my_stl_alloc.h"

using namespace std;

int main() {
  MY_STL::vector<int, MY_STL::simple_alloc<int, MY_STL::alloc>> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }

  cout << "输出测试：" << endl;
  for (int i : vec) {
    cout << i << endl;
  }
}