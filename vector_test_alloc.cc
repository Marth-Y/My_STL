#include <iostream>
#include "my_stl_vector.h"

#include "my_stl_alloc.h"

using namespace std;

int main() {
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