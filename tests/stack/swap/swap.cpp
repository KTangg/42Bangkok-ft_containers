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

template <typename T, typename C>
void printStack(stack<T, C> a)
{
    std::cout << "Size: " << a.size() << std::endl;
    std::cout << "Element: ";
    for (; !a.empty();)
    {
        std::cout << a.top() << " ";
        a.pop();
    }
    std::cout << std::endl;
}

int main(void)
{
    stack<float, std::deque<float> > first;

    std::deque<float> d(42, 4.2);
    stack<float, std::deque<float> > second(d);

    std::cout << "Before" << std::endl;
    printStack(first);
    printStack(second);

    /// Using member function
    first.swap(second);
    std::cout << "After" << std::endl;
    printStack(first);
    printStack(second);

    // Using non-member function
    swap(first, second);
    printStack(first);
    printStack(second);

    std::deque<float> d2(21 , 2.1);
    stack<float, std::deque<float> > third(d2);
    std::cout << "Before" << std::endl;
    printStack(second);
    printStack(third);

    /// Using member function
    second.swap(third);
    std::cout << "After" << std::endl;
    printStack(second);
    printStack(third);

    // Using non-member function
    swap(third, second);
    printStack(second);
    printStack(third);

}
