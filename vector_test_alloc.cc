#include "stl_alloc.h"
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<int,__default_alloc_template<0>> vec;
    for(int i = 0;i < 10; i++)
    {
        vec.push_back(i);
    }

    cout << "输出测试：" << endl;
    for(int i:vec) {
        cout << i << endl;
    }
}