#include <iostream>
#include <vector>
#include "../../../vector.hpp"

#ifdef FT
    using namespace ft;
#else
    using namespace std;
#endif

template<typename T>
void printVector(vector<T>& v)
{
    std::cout << "Vector: ";
    for (typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(void)
{
    // Default constuctor
    vector<int> a;
    printVector<int>(a);
    // Fill constuctor with 0 element
    vector<int> b(0);
    printVector<int>(b);
    // Fill constructor with 10 element default value
    vector<int> c(10);
    printVector<int>(c);
    // Fill constructor with 42 element with 42 value
    vector<int> d(42, 42);
    printVector<int>(d);
    // Range constructor with value
    vector<int> e(d.begin(), d.end());
    printVector<int>(e);
    // Range constructor without value
    vector<int> f(a.begin(), a.end());
    printVector<int>(e);
    // Assignment Operator
    a = d;
    printVector<int>(a);
}
