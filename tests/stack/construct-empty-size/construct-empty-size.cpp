#include <iostream>
#include <vector>
#include <stack>
#include <deque>
#include "../../../stack.hpp"

#ifdef FT
    using namespace ft;
#else
    using namespace std;
#endif

int main(void)
{
    // Using vector as container
    vector<int> v_empty;
    vector<int> v_test(42, 42);

    stack<int, vector<int> > first(v_empty);
    stack<int, vector<int> > second(v_test);

    std::cout << first.empty() << std::endl;
    std::cout << first.size() << std::endl;

    std::cout << second.empty() << std::endl;
    std::cout << second.top() << std::endl;
    std::cout << second.size() << std::endl;

    // Using std::deque as container
    std::deque<char> d_empty;
    std::deque<char> d_test(10, 'a');

    stack<char, std::deque<char> >  third(d_test);
    stack<char, std::deque<char> >  fourth(d_test);

    std::cout << third.empty() << std::endl;
    std::cout << third.top() << std::endl;
    std::cout << third.size() << std::endl;

    std::cout << fourth.empty() << std::endl;
    std::cout << fourth.size() << std::endl;
}
