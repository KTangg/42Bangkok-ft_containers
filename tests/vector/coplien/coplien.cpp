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
    d = vector<int>();
    printVector<int>(d);
    printVector<int>(a);

    // Default constuctor
    vector<char> a_char;
    printVector<char>(a_char);
    // Fill constuctor with 0 element
    vector<char> b_char(0);
    printVector<char>(b_char);
    // Fill constructor with 10 element default value
    vector<char> c_char(10);
    printVector<char>(c_char);
    // Fill constructor with 42 element with 42 value
    vector<char> d_char(42, 65);
    printVector<char>(d_char);
    // Range constructor with value
    vector<char> e_char(d_char.begin(), d_char.end());
    printVector<char>(e_char);
    // Range constructor without value
    vector<char> f_char(a_char.begin(), a_char.end());
    printVector<char>(e_char);
    // Assignment Operator
    a_char = d_char;
    d_char = vector<char>();
    printVector<char>(d_char);
    printVector<char>(a_char);
}
