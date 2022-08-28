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
    vector<char> empty;
    vector<char> test;

    for (int i = 0; i < 20; i++)
    {
        test.push_back(48 + i);
    }
    /**
     *  @defgroup Test for iterator
     */
    vector<char>::pointer x();
    /// Test begin
    if (empty.begin() == vector<char>::iterator())
        std::cout << "nullptr" << std::endl;
    if (*test.begin() == 0)
        std::cout << "0" << std::endl;

    /// Test end
    if (empty.end() == vector<char>::iterator())
        std::cout << "nullptr" << std::endl;
    if (*test.end() == 0)
        std::cout << "0" << std::endl;

    /// Test rbegin
    if (empty.rbegin() == vector<char>::reverse_iterator())
        std::cout << "nullptr" << std::endl;
    if (*test.rbegin() == 0)
        std::cout << "0" << std::endl;

    /// Test rend
    if (empty.rend() == vector<char>::reverse_iterator())
        std::cout << "nullptr" << std::endl;
    if (*test.rend() == 0)
        std::cout << "0" << std::endl;

    /// Test relational operator
    vector<char>::iterator start = test.begin();
    vector<char>::iterator mid = test.begin() + 5;
    vector<char>::iterator end = test.end();
    vector<char>::reverse_iterator rstart = test.rbegin();
    vector<char>::reverse_iterator rmid = test.rbegin() + 1;
    vector<char>::reverse_iterator rend = test.rend();

    // Test ==
    std::cout << "(start == start)" << (start == start) << std::endl;
    std::cout << "(start == mid)" << (start == mid) << std::endl;
    std::cout << "(rstart == rstart)" << (rstart == rstart) << std::endl;
    std::cout << "(rstart == rmid)" << (rstart == rmid) << std::endl;

    // Test !=
    std::cout << "(start != start)" << (start != start) << std::endl;
    std::cout << "(start != mid)" << (start != mid) << std::endl;
    std::cout << "(rstart != rstart)" << (rstart != rstart) << std::endl;
    std::cout << "(rstart != rmid)" << (rstart != rmid) << std::endl;

    // Test >
    std::cout << "(start > start)" << (start > start) << std::endl;
    std::cout << "(start > mid)" << (start > mid) << std::endl;
    std::cout << "(end > mid)" << (end > mid) << std::endl;
    std::cout << "(rstart > rstart)" << (rstart > rstart) << std::endl;
    std::cout << "(rstart > rmid)" << (rstart > rmid) << std::endl;
    std::cout << "(rend > rmid)" << (rend > rmid) << std::endl;

    // Test <
    std::cout << "(start < start)" << (start < start) << std::endl;
    std::cout << "(start < mid)" << (start < mid) << std::endl;
    std::cout << "(end < mid)" << (end < mid) << std::endl;
    std::cout << "(rstart < rstart)" << (rstart < rstart) << std::endl;
    std::cout << "(rstart < rmid)" << (rstart < rmid) << std::endl;
    std::cout << "(rend < rmid)" << (rend < rmid) << std::endl;

    // Test >=
    std::cout << "(start >= start)" << (start >= start) << std::endl;
    std::cout << "(start >= mid)" << (start >= mid) << std::endl;
    std::cout << "(end >= mid)" << (end >= mid) << std::endl;
    std::cout << "(rstart >= rstart)" << (rstart >= rstart) << std::endl;
    std::cout << "(rstart >= rmid)" << (rstart >= rmid) << std::endl;
    std::cout << "(rend >= rmid)" << (rend >= rmid) << std::endl;

    // Test <=
    std::cout << "(start <= start)" << (start <= start) << std::endl;
    std::cout << "(start <= mid)" << (start <= mid) << std::endl;
    std::cout << "(end <= mid)" << (end <= mid) << std::endl;
    std::cout << "(rstart <= rstart)" << (rstart <= rstart) << std::endl;
    std::cout << "(rstart <= rmid)" << (rstart <= rmid) << std::endl;
    std::cout << "(rend <= rmid)" << (rend <= rmid) << std::endl;

    // test increment
    for (vector<char>::iterator it = test.begin(); it != test.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (vector<char>::reverse_iterator it = test.rbegin(); it != test.rend(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // test decrement
    for (vector<char>::iterator it = test.end(); it != test.begin(); --it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (vector<char>::reverse_iterator it = test.rend(); it != test.rbegin(); --it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // test indexing
    std::cout << test.begin()[0] << std::endl;
    std::cout << test.begin()[5] << std::endl;
    std::cout << test.rbegin()[0] << std::endl;
    std::cout << test.rbegin()[5] << std::endl;

    // test arithmetic
    std::cout << *(test.begin() + 0) << std::endl;
    std::cout << *(test.begin() + 5) << std::endl;
    std::cout << *(test.end() - 1) << std::endl;
    std::cout << *(test.end() - 10) << std::endl;
    std::cout << *(test.rbegin() + 0) << std::endl;
    std::cout << *(test.rbegin() + 5) << std::endl;
    std::cout << *(test.rend() - 1) << std::endl;
    std::cout << *(test.rend() - 10) << std::endl;
}
