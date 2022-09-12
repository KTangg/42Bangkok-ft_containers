#include <iostream>
#include <vector>
#include <stack>
#include <list>
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
    // Using vector as container
    stack<int, vector<int> >first;
    printStack(first);

    // Push new element to stack
    first.push(21);
    printStack(first);
    first.push(42);
    printStack(first);

    first.push(999);
    printStack(first);
    first.pop();
    printStack(first);

    // Using std::list as container
    stack<std::string, std::list<std::string> > second;
    printStack(second);

    // Push new element to stack
    second.push("base");
    printStack(second);
    second.push("mid");
    printStack(second);

    second.push("top");
    printStack(second);
    second.pop();
    printStack(second);
    second.pop();
    printStack(second);
    second.pop();
    printStack(second);
}
