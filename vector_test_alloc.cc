#include <iostream>
#include "my_stl_vector.h"

#include "my_stl_alloc.h"

using namespace std;

int main() {
  MY_STL::vector<int, MY_STL::simple_alloc<int, MY_STL::alloc>> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(10);
  }

  cout << "输出测试：" << endl;
  cout << "size：" << vec.size() << endl;
  for (int i = 0; i < vec.size(); ++ i) {
    cout << i << " " << vec[i] << endl;
  }
}