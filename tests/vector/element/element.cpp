#include <iostream>
#include <vector>
#include <exception>
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
    vector<std::string> a(1, "hello");
    a.resize(2, "world");
    a.resize(3, "this");
    a.resize(4, "is");
    a.resize(5, "me");

    // Test operator[]
    for (size_t i = 0; i < 5; i++)
    {
        std::cout << a[i] << std::endl;
    }

    /// Test at
    for (size_t i = 0; i < 10; i++)
    {
        try
        {
            std::cout << a.at(i) << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << "Exception Caught" << std::endl;
        }
    }


    /// Test front
    std::cout << a.front() << std::endl;

    /// Test back
    std::cout << a.back() << std::endl;

    vector<std::string> b;
    /// Test data
    /// With Regular data
    std::cout << *a.data() << std::endl;

    /// With nullptr
    std::cout << b.data() << std::endl;

    return (0);
}
