#ifndef __ITERATOR_HPP__
# define __ITERATOR_HPP__

# include "iterator_base.hpp"

namespace ft
{
    /**
     *  @brief Iterator adapter
     *
     */
    template <typename _Iterator, typename _Container>
        class Iterator
        {
            /// Attributes of Iterator
            protected:
                _Iterator   _base;

                /// Alias for iterator_traits
                typedef ft::iterator_traits<_Iterator>      _traits;

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
                Iterator() : _base(_Iterator()) {}

                /// Constructor with given value of _Iterator type
                Iterator(const _Iterator &i) : _base(i) {}

                /// Copy constructor
                Iterator(const Iterator &src) {*this = src;}

                /// Assignment operator overload
                Iterator    &operator=(const Iterator &rhs)
                {
                    this->_base = rhs._base;
                    return (*this);
                }

                /// Deconstructor
                virtual ~Iterator() {}

                /// Getter function
                const iterator_type&    getBase(void) const
                {
                    return (_base);
                }
                /**
                 *  @defgroup Implementation for Forward iterator
                 *  operations requirement
                 */

                /// Dereference operator
                reference   operator*() const
                {
                    return *_base;
                }

                /// Dereference operator
                pointer     operator->() const
                {
                    return _base;
                }

                /// Pre-increment operator
                Iterator&   operator++()
                {
                    _base++;
                    return (*this);
                }

                /// Post-increment operator
                Iterator    operator++(int)
                {
                    return (Iterator(_base++));
                }

                /**
                 *  @defgroup Implementation for Bidirectional iterator
                 *  operations requirement
                 */

                /// Pre-decrement
                Iterator&   operator--()
                {
                    _base--;
                    return (*this);
                }

                // Post-decrement
                Iterator    operator--(int)
                {
                    return (Iterator(_base--));
                }

                /**
                 *  @defgroup Implementation for Random-access iterator
                 *  operations requirement
                 */

                /// Offset dereference
                reference   operator[](difference_type n) const
                {
                    return (_base[n]);
                }

                /// Compound increment assign
                Iterator&   operator+=(difference_type n)
                {
                    _base += n;
                    return (*this);
                }

                /// Compound decrement assign
                Iterator&   operator-=(difference_type n)
                {
                    _base -= n;
                    return (*this);
                }

                /// Arithmetic operator + and -
                Iterator    operator+(difference_type n) const
                {
                    return (Iterator(_base + n));
                }

                Iterator    operator-(difference_type n) const
                {
                    return (Iterator(_base - n));
                }

        }; // class Iterator

    /// Equality/Inequality comparison
    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator==(const Iterator<_IteratorL, _Container> &lhs,
            const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() == rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator==(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return (lhs.getBase() == rhs.getBase()); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator!=(const Iterator<_IteratorL, _Container> &lhs,
             const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() != rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator!=(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return (lhs.getBase() != rhs.getBase()); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline typename Iterator<_IteratorL, _Container>::difference_type
        operator-(const Iterator<_IteratorL, _Container>& lhs,
            const Iterator<_IteratorR, _Container>& rhs)
        { return (lhs.getBase() - rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline Iterator<_Iterator, _Container>
        operator+(typename Iterator<_Iterator, _Container>::difference_type n,
            const Iterator<_Iterator, _Container>& it)
        { return (Iterator<_Iterator, _Container>(it.getBase() + n)); }

    /// Inequality comparisons(<, >, <= and >=)
    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator<(const Iterator<_IteratorL, _Container> &lhs,
            const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() < rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator<(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return (lhs.getBase() < rhs.getBase()); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator>(const Iterator<_IteratorL, _Container> &lhs,
            const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() > rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator>(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return (lhs.getBase() > rhs.getBase()); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator<=(const Iterator<_IteratorL, _Container> &lhs,
            const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() <= rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator<=(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return (lhs.getBase() <= rhs.getBase()); }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
        inline bool
        operator>=(const Iterator<_IteratorL, _Container> &lhs,
            const Iterator<_IteratorR, _Container> &rhs)
        { return (lhs.getBase() >= rhs.getBase()); }

    template <typename _Iterator, typename _Container>
        inline bool
        operator>=(const Iterator<_Iterator, _Container> &lhs,
            const Iterator<_Iterator, _Container> &rhs)
        { return lhs.getBase() >= rhs.getBase(); }

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

                explicit reverse_iterator(const iterator_type& it) : _base(it) {}

            /**
             *  @brief Copy constructor and Assignment are normal
             */
                reverse_iterator(const reverse_iterator& src) { *this = src; }

                reverse_iterator&   operator=(const reverse_iterator& rhs)
                {
                    _base = rhs.base;
                    return (*this);
                }

            /**
             *  @brief Getter of reverse_iterator
             *  @return Iterator used for underlying work
             */
                const iterator_type&    getBase(void) const {return (_base);}

            /**
             *  @brief Dereference reverse pointer will do on --_base
             *
             */
                reference   operator*() const
                {
                    iterator_type tmp = _base;
                    --tmp;
                    return *(--tmp);
                }

                pointer operator->() const
                {
                    iterator_type tmp = _base;
                    --tmp;
                    return _It_to_pointer(tmp);
                }

            private:
                template <typename T>
                    static T*   _It_to_pointer(T* p) { return p; }

                template <typename T>
                    static pointer  _It_to_pointer(T t) { return t.operator->(); }

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
                {
                    return reverse_iterator(_base - n);
                }

                reverse_iterator& operator+=(difference_type n)
                {
                    _base -= n;
                    return *this;
                }

                reverse_iterator operator-(difference_type n) const
                {
                    return reverse_iterator(_base + n);
                }

                reverse_iterator operator-=(difference_type n)
                {
                    _base += n;
                    return *this;
                }

                reference operator[](difference_type n) const
                {
                    return *(*this + n);
                }
        };
    /**
     *  @defgroup Comparison of reverse iterator
     *
     */
    template <typename _Iterator>
        inline bool
        operator==(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() == rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator==(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() == rhs.getBase(); }

    template <typename _Iterator>
        inline bool
        operator<(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() < rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator<(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() < rhs.getBase(); }

    template <typename _Iterator>
        inline bool
        operator!=(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() != rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator!=(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() != rhs.getBase(); }

    template <typename _Iterator>
        inline bool
        operator>(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() > rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator>(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() > rhs.getBase(); }

    template <typename _Iterator>
    inline bool
        operator<=(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() <= rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator<=(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() <= rhs.getBase(); }

    template <typename _Iterator>
        inline bool
        operator>=(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return lhs.getBase() >= rhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline bool
        operator>=(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return lhs.getBase() >= rhs.getBase(); }

/**
 *  @brief distance between two iterator
 */

    template <typename _Iterator>
        inline typename reverse_iterator<_Iterator>::difference_type
        operator-(const reverse_iterator<_Iterator>& lhs,
                const reverse_iterator<_Iterator>& rhs)
        { return rhs.getBase() - lhs.getBase(); }

    template <typename _IteratorL, typename _IteratorR>
        inline typename reverse_iterator<_IteratorL>::difference_type
        operator-(const reverse_iterator<_IteratorL>& lhs,
                const reverse_iterator<_IteratorR>& rhs)
        { return rhs.getBase() - lhs.getBase(); }
} // namespace

#endif /* __ITERATOR_HPP__ */
