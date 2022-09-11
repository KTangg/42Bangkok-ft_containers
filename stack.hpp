/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 21:42:40 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/06 19:13:34 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STACK_HPP__
# define __STACK_HPP__

# include "vector.hpp"

namespace ft
{
    using namespace ft;
    /**
     *  @brief Stack is a type of container adaptor, with LIFO context (last in first out)
     *
     *  @tparam T Type of element to store in stack
     *  @tparam Container Internal container using by stack to store elements with @a <T> value type
     *  If not specified vector<T> will be used
     */
    template < class T, class Container = vector<T> >
    class stack
    {
        /**
         *  @defgroup Alias for typename in container
         */
        public:
            typedef T                                           value_type;
            typedef Container                                   container_type;
            typedef typename container_type::size_type          size_type;
            typedef typename container_type::reference          reference;
            typedef typename container_type::const_reference    const_reference;


        /**
         *  @brief Stack using another container under the hood
         */
        protected:
            container_type  _c;

        public:
        /**
         *  @brief Default constructor copy given container to internal stored container
         *
         *  @param ctn container to be copied as initialized internal container
         *  if not giving using default construct of internal container.
         */
            explicit stack(const container_type& ctn = container_type())
            : _c(ctn) { }

        /**
         *  @brief Check if stack is empty or not empty mean 0 element store
         */
            bool empty(void) const
            { return _c.empty(); }

        /**
         *  @brief Getting size of elements store in stack
         *
         *  @return size_type count of elements
         */
            size_type size(void) const
            { return _c.size(); }

        /**
         *  @brief Getting read/write reference to first element in stack
         *
         *  @return a reference to top element in the stack
         *
         *  @remark Stack is LIFO so first element is the last one in normal container
         */
            reference top(void)
            { return _c.back(); }

        /**
         *  @brief Getting read reference to first element in stack
         *
         *  @return a reference to top element in the stack
         *
         *  @remark Stack is LIFO so first element is the last one in normal container
         */
            const_reference top(void) const
            { return _c.back(); }

        /**
         *  @brief Push new element to current stack
         *
         *  @remark internal container should use push_back since stack is LIFO
         */
            void push(const value_type& val)
            { return _c.push_back(val); }

        /**
         *  @brief Pop top element out of stack
         */
            void pop(void)
            { return _c.pop_back(); }

        /**
         *  @brief swap elements store inside stack with other
         *
         *  @remark Using swap in the internal container
         */
            void swap(stack& other)
            { _c.swap(other._c); }

        /**
         *  @defgroup declare friend to let == and < operation using
         *  @a _c container protected attribute
         *
         *  @remark template typename can't be shadow(duplicated) with stack typename
         *  using _T, _Container to avoid compiling error
         */
        template <class _T, class _Container>
        friend inline bool
        operator==(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

        template <class _T, class _Container>
        friend inline bool
        operator<(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);
    }; /* class stack */

    /**
     *  @defgroup stack relational operation depend on
     *  internal container realational operation
     */
    template <class T, class Container>
    inline bool
    operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return (lhs._c == rhs._c); }

    template <class T, class Container>
    inline bool
    operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return !(lhs == rhs); }

    template <class T, class Container>
    inline bool
    operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return (lhs._c < rhs._c); }

    template <class T, class Container>
    inline bool
    operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return (rhs < lhs); }

    template <class T, class Container>
    inline bool
    operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return !(lhs > rhs); }

    template <class T, class Container>
    inline bool
    operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    { return !(rhs > lhs); }

} /* namespace ft */

#endif /* __STACK_HPP__ */
