#include <iostream>
using namespace std;


(void ()) *test2(int a)
{

}

static void (* test(int a))()
{
    int b = a;
    printf("%d\n",b);
    void (* result)();
    cout << typeid(result).name() << endl;
    return result;
}

// static void (* test2)()
// {
// }

int main()
{
    test(2);
    // test2();
}