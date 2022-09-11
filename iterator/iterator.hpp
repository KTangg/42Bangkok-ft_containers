/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:38:41 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/06 17:39:09 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ITERATOR_HPP__
# define __ITERATOR_HPP__

# include "../utils/type_traits.hpp"
# include "iterator_base.hpp"

namespace ft
{
    using ft::iterator_traits;
    using ft::iterator;
    /**
     *  @brief Iterator adapter
     *
     */
    template <typename _Iterator, typename _Container>
        class normal_iterator
        {
            /// Attributes of Iterator
            protected:
                _Iterator   _base;

                /// Alias for iterator_traits
                typedef iterator_traits<_Iterator>      _traits;

            /**
             *  @defgroup Alias for type in this Iterator class
             *  you don't want to type ft::iterator_traits<_Iterator>::value_type
             *  all the time
             *
             */

            public:
                typedef _Iterator                           iterator_type;
                typedef typename _traits::iterator_category iterator_category;
                typedef typename _traits::value_type        value_type;
                typedef typename _traits::difference_type   difference_type;
                typedef typename _traits::reference         reference;
                typedef typename _traits::pointer           pointer;

                /// Default constructor assign _current to be default Iterator
                normal_iterator() : _base(_Iterator()) {}

                /// Constructor with given value of _Iterator type
                explicit
                normal_iterator(const _Iterator& i) : _base(i) {}

                /// Copy constructor
                normal_iterator(const normal_iterator& src) {*this = src;}

                // Allow iterator to const_iterator conversion
                template<typename _Iter>
                normal_iterator(const normal_iterator<_Iter,
                typename ft::enable_if<
                (std::__are_same<_Iter, typename _Container::pointer>::__value),
                _Container>::type>& src) : _base(src.base()) { }

                /// Assignment operator overload
                normal_iterator& operator=(const normal_iterator &rhs)
                {
                    this->_base = rhs._base;
                    return *this;
                }

                /// Deconstructor
                virtual ~normal_iterator() {}

                /// Getter function
                const iterator_type& base(void) const
                { return _base; }

                /**
                 *  @defgroup Implementation for Forward iterator
                 *  operations requirement
                 */

                /// Dereference operator
                reference operator*() const
                { return *_base; }

                /// Dereference operator
                pointer operator->() const
                { return _base; }

                /// Pre-increment operator
                normal_iterator& operator++()
                {
                    ++_base;
                    return *this;
                }

                /// Post-increment operator
                normal_iterator operator++(int)
                { return normal_iterator(_base++); }

                /**
                 *  @defgroup Implementation for Bidirectional iterator
                 *  operations requirement
                 */

                /// Pre-decrement
                normal_iterator& operator--()
                {
                    --_base;
                    return *this;
                }

                // Post-decrement
                normal_iterator operator--(int)
                { return normal_iterator(_base--); }

                /**
                 *  @defgroup Implementation for Random-access iterator
                 *  operations requirement
                 */

                /// Offset dereference
                reference operator[](difference_type n) const
                { return _base[n]; }

                /// Compound increment assign
                normal_iterator& operator+=(difference_type n)
                { _base += n; return *this; }

                /// Compound decrement assign
                normal_iterator& operator-=(difference_type n)
                { _base -= n; return (*this); }

                /// Arithmetic operator + and -
                normal_iterator operator+(difference_type n) const
                { return normal_iterator(_base + n); }

                normal_iterator operator-(difference_type n) const
                { return normal_iterator(_base - n); }

        }; // class Iterator

    /**
     *  @brief Comparison Operation on normal_iterator
     *  @p L is stand for left, R stand for right
     */

    /// Forward iterator requirements
    /// Equality/Inequality comparison
    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator==(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() == rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator==(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() == rhs.base(); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator!=(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() != rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator!=(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() != rhs.base(); }

    /// Random access iterator requirements
    /// >, <, >=, <= comparison
    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() < rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator<(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() < rhs.base(); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() > rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator>(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() > rhs.base(); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<=(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() <= rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator<=(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() <= rhs.base(); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>=(const normal_iterator<_IteratorL, _Container> &lhs,
            const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() >= rhs.base(); }

    template <typename _Iterator, typename _Container>
    inline bool
    operator>=(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() >= rhs.base(); }

    /// Plus operation that number on left side
    template <typename _Iterator, typename _Container>
    inline normal_iterator<_Iterator, _Container>
    operator+(typename normal_iterator<_Iterator, _Container>::difference_type
        n, const normal_iterator<_Iterator, _Container>& rhs)
    { return normal_iterator<_Iterator, _Container>(rhs.base() + n); }

    /// Minus operation between two Iterator
    template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline typename normal_iterator<_IteratorL, _Container>::difference_type
    operator-(const normal_iterator<_IteratorL, _Container>& lhs,
        const normal_iterator<_IteratorR, _Container>& rhs)
    { return lhs.base() - rhs.base(); }

    template<typename _Iterator, typename _Container>
    inline typename normal_iterator<_Iterator, _Container>::difference_type
    operator-(const normal_iterator<_Iterator, _Container>& lhs,
        const normal_iterator<_Iterator, _Container>& rhs)
    { return lhs.base() - rhs.base(); }

    /**
     *  @brief Reverse Iterator adapter
     *
     */
    template<typename _Iterator>
        class reverse_iterator
        : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                    typename iterator_traits<_Iterator>::value_type,
                    typename iterator_traits<_Iterator>::difference_type,
                    typename iterator_traits<_Iterator>::pointer,
                    typename iterator_traits<_Iterator>::reference>
        {
            protected:
                _Iterator _base;

                typedef iterator_traits<_Iterator>  _traits;

            public:
                typedef _Iterator                           iterator_type;
                typedef typename _traits::difference_type   difference_type;
                typedef typename _traits::pointer           pointer;
                typedef typename _traits::reference         reference;

            /**
             *  @brief Default constructor initialized member @p _base
             *  If it is a pointer, that means it is zero-initialized
             */
                reverse_iterator() : _base() {}

                explicit
                reverse_iterator(iterator_type it) : _base(it) {}

            /**
             *  @brief Copy constructor and Assignment are normal
             */
                reverse_iterator(const reverse_iterator& src) { *this = src; }

                reverse_iterator& operator=(const reverse_iterator& rhs)
                {
                    _base = rhs._base;
                    return *this;
                }

            /// Allow const iterator convert
                template <typename _Iter>
                reverse_iterator(const reverse_iterator<_Iter>& src)
                : _base(src.base()) {}

            /**
             *  @brief Getter of reverse_iterator
             *  @return Iterator used for underlying work
             */
                const iterator_type& base(void) const
                {return _base;}

            /**
             *  @brief Dereference reverse pointer will do on --_base
             *
             */
                reference operator*() const
                {
                    iterator_type tmp = _base;
                    return *--tmp;
                }

                pointer operator->() const
                {
                    iterator_type tmp = _base;
                    --tmp;
                    return _It_to_pointer(tmp);
                }

            private:
                template <typename T>
                    static T* _It_to_pointer(T* p) { return p; }

                template <typename T>
                    static pointer _It_to_pointer(T t) { return t.operator->(); }

            /**
             *  @brief reverse iterator move in the opposite
             *  way of it underlying
             */
            public:
                reverse_iterator& operator++()
                {
                    --_base;
                    return *this;
                }

                reverse_iterator operator++(int)
                {
                    reverse_iterator tmp = *this;
                    --_base;
                    return tmp;
                }

                reverse_iterator& operator--()
                {
                    ++_base;
                    return *this;
                }

                reverse_iterator operator--(int)
                {
                    reverse_iterator tmp = *this;
                    ++_base;
                    return tmp;
                }

                reverse_iterator operator+(difference_type n) const
                { return reverse_iterator(_base - n); }

                reverse_iterator& operator+=(difference_type n)
                {
                    _base -= n;
                    return *this;
                }

                reverse_iterator operator-(difference_type n) const
                { return reverse_iterator(_base + n); }

                reverse_iterator operator-=(difference_type n)
                {
                    _base += n;
                    return *this;
                }

                reference operator[](difference_type n) const
                { return *(*this + n); }
        };
    /**
     *  @defgroup Comparison of reverse iterator
     *
     */
    template <typename _Iterator>
    inline bool
    operator==(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() == lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator==(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() == lhs.base(); }

    template<typename _Iterator>
    inline bool
    operator<(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() < lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator<(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() < lhs.base(); }

    template<typename _Iterator>
    inline bool
    operator!=(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() != lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator!=(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() != lhs.base(); }

    template<typename _Iterator>
    inline bool
    operator>(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() > lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator>(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() > lhs.base(); }

    template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() <= lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator<=(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() <= lhs.base(); }

    template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() >= lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline bool
    operator>=(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() >= lhs.base(); }

    /// Different operation
    template <typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& lhs,
            const reverse_iterator<_Iterator>& rhs)
    { return rhs.base() - lhs.base(); }

    template <typename _IteratorL, typename _IteratorR>
    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& lhs,
            const reverse_iterator<_IteratorR>& rhs)
    { return rhs.base() - lhs.base(); }

    // Left hand size plus reverse_iterator
    template <typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type n,
            const reverse_iterator<_Iterator>& rhs)
    { return reverse_iterator<_Iterator>(rhs.base() - n); }

} // ft namespace

#endif /* __ITERATOR_HPP__ */
