#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
using namespace std;


int main() {
    vector<int> ve{1,2,3,4};
    ve.reserve(5);
    ve.insert(ve.end() - 1, 2, 5);
    std::ostream_iterator<int> oo(cout, " ");
    std::copy(ve.begin(), ve.end(), oo);
    ve.erase(ve.begin(), ve.end());
}