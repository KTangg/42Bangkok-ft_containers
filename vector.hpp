/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 20:31:13 by spoolpra          #+#    #+#             */
/*   Updated: 2022/08/28 20:42:22 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_VECTOR_HPP__
# define __FT_VECTOR_HPP__

# include <iostream>
# include <memory>
# include <limits>
# include <exception>
# include <sstream>
# include "iterator/iterator.hpp"
# include "utils/type_traits.hpp"

namespace ft {
    template < class T, class Alloc = std::allocator<T> >
    class vector
    {
        public:

            /**
             *  @defgroup Alias for variable type
             */
            typedef T                                           value_type;
            typedef Alloc                                       allocator_type;
            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef size_t                                      size_type;
            typedef ptrdiff_t                                   difference_type;

            typedef ft::Iterator<pointer, vector>               iterator;
            typedef ft::Iterator<const_pointer, vector>         const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

            /**
             *  @defgroup Attribute for container
             */
        private:
            allocator_type  _alloc;
            pointer         _m_start;
            pointer         _m_finish;
            pointer         _m_end_of_storage;

            class alloc_length_error : public std::exception
            {
                public:
                    virtual const char* what(void) const throw()
                    { return "cannot create ft:vector larger than max_size()"; }
            };
            pointer _m_allocate(size_type n)
            {
                if (n > _alloc.max_size())
                    throw alloc_length_error();
                return n != 0 ? _alloc.allocate(n) : pointer();
            }

            void _m_deallocate(void)
            {
                if (_m_start)
                    _alloc.deallocate(_m_start, _m_end_of_storage - _m_start);
            }

            void _m_reallocate(size_type n)
            {
                pointer _m_start_tmp = _m_allocate(n);
                pointer _m_finish_tmp = std::uninitialized_copy(_m_start, _m_finish, _m_start_tmp);
                pointer _m_end_of_storage_tmp = _m_start_tmp + n;
                clear();
                _m_deallocate();
                _m_start = _m_start_tmp;
                _m_finish = _m_finish_tmp;
                _m_end_of_storage = _m_end_of_storage_tmp;
            }

            void _m_grow(void)
            {
                size_type cap = capacity();
                if (cap == 0)
                    _m_reallocate(1);
                else
                {
                    if (capacity() > (max_size() / 2))
                        _m_reallocate(max_size());
                    else
                        _m_reallocate(capacity() * 2);
                }
            }

            void _m_create_storage(size_type n)
            {
                _m_start = _m_allocate(n);
                _m_finish = _m_start;
                _m_end_of_storage = _m_start + n;
            }

            template <typename InputIterator>
            void _m_initial_range(InputIterator first, InputIterator last,
                false_type)
            {
                size_type n = last - first;
                _m_start = _m_allocate(n);
                _m_end_of_storage = _m_start + n;
                _m_finish = std::uninitialized_copy(first, last, _m_start);
            }

            template <typename Integral>
            void _m_initial_range(Integral n, Integral val,
                true_type)
            {
                size_type size_n = static_cast<size_type>(n);
                _m_start = _m_allocate(size_n);
                _m_end_of_storage = _m_start + size_n;
                _m_finish = std::uninitialized_fill_n(_m_start, n, val);
            }

            template <typename InputIterator>
            void _m_assign_range(InputIterator first, InputIterator last,
                false_type)
            {
                size_type n = last - first;
                if (n > capacity())
                {
                    vector<T> tmp(first, last, _alloc);
                    _m_swap(tmp);
                }
                else if (n > size())
                {
                    iterator it = begin();
                    for (; it != end(); it++, first++)
                    {
                        *it = *first;
                    }
                    for (; first != last; it++, first++)
                    {
                        _alloc.construct(it.getBase(), *first);
                    }
                    _m_finish = _m_start + n;
                }
                else
                {
                    iterator it = begin();
                    for (; first != last; it++, first++)
                    {
                        *it = *first;
                    }
                    _m_erase_at_end(it.getBase());
                }
            }

            template <typename Integral>
            void _m_assign_range(Integral n, Integral val,
                true_type)
            {
                _m_fill_assign(n, val);
            }

            template <typename InputIterator>
            void _m_copy_range_to_begin(InputIterator first, InputIterator last)
            {
                iterator dest = begin();
                for (InputIterator it = first; it != last; it++, dest++)
                {
                    *dest = *it;
                }
            }

            void _m_fill_assign(size_type n, value_type val)
            {
                if (n > capacity())
                {
                    vector<T> tmp(n, val);
                    _m_swap(tmp);
                }
                else if (n > size())
                {
                    size_type fill_n = n - size();
                    std::fill(begin(), end(), val);
                    std::uninitialized_fill_n(_m_finish, fill_n, val);
                    _m_finish = _m_start + n;
                }
                else
                {
                    iterator fill_end = std::fill_n(begin(), n, val);
                    _m_erase_at_end(fill_end.getBase());
                }
            }

            template <typename ForwardIterator>
            void _m_fill_element_n(ForwardIterator dest, size_type n, value_type val)
            {
                for (size_type i = 0; i < n; i++, dest++)
                {
                    _alloc.construct(dest, val);
                }
                _m_finish = dest;
            }

            iterator _m_insert_rval(iterator position, value_type val)
            {
                size_type n = position - cbegin();
                if (_m_finish != _m_end_of_storage)
                {
                    if (position == cend())
                    {
                        _alloc.construct(_m_finish, val);
                        _m_finish++;
                    }
                    else
                    {
                        _m_insert_aux(position, val);
                    }
                }
                else
                {
                    _m_grow();
                    _m_insert_aux(_m_start + n, val);
                }
                return iterator(_m_start + n);
            }

            void _m_insert_aux(iterator position, value_type val)
            {
                if (position == end())
                {
                    _alloc.construct(_m_finish, val);
                    _m_finish++;
                    return ;
                }
                _alloc.construct(_m_finish, *(_m_finish - 1));
                _m_finish++;
                for (reverse_iterator it = rbegin(); it != reverse_iterator(position + 1); it++)
                {
                    *it = *(it + 1);
                }
                *position = val;
            }

            void _m_insert_fill(iterator position, size_type n, const value_type& val)
            {
                if (n == 0)
                    return ;
                size_type idx = position - cbegin();
                if (n + size() > max_size())
                    throw std::length_error("vector::fill_insert");
                if (n + size() > capacity())
                {
                    size_type new_size;
                    if (capacity() > max_size() / 2)
                        new_size = max_size();
                    else
                        new_size = std::max(n + size(), capacity() * 2);
                    _m_reallocate(new_size);
                    _m_insert_fill_aux(begin() + idx, n, val);
                }
                else
                    _m_insert_fill_aux(position, n, val);
            }

            void _m_insert_fill_aux(iterator position, size_type n, const value_type val)
            {
                if (position == end())
                {
                    for (size_type i = 0; i < n; i++)
                    {
                        _alloc.construct(_m_finish, val);
                        _m_finish++;
                    }
                    return ;
                }
                reverse_iterator r_it = rbegin();
                pointer new_end = _m_finish + n - 1;
                for (; new_end != _m_finish - 1;)
                {
                    _alloc.construct(new_end, *r_it++);
                    new_end--;
                }
                for (; r_it != reverse_iterator(position); r_it++)
                {
                    *new_end = *r_it;
                    new_end--;
                }
                _m_finish = _m_finish + n;
                std::fill_n(position, n, val);
            }

            template <typename InputIterator>
            void _m_insert_range(iterator position, InputIterator first, InputIterator last)
            {
                if (first == last)
                    return ;
                size_type n = last - first;
                size_type idx = position - begin();
                if (n + size() > max_size())
                    throw std::length_error("vector::fill_insert");
                if (n + size() > capacity())
                {
                    size_type new_size;
                    if (capacity() > max_size() / 2)
                        new_size = max_size();
                    else
                        new_size = std::max(n + size(), capacity() * 2);
                    _m_reallocate(new_size);
                    _m_insert_range_aux(begin() + idx, first, last);
                }
                else
                    _m_insert_range_aux(position, first, last);
            }

            template <typename InputIterator>
            void _m_insert_range_aux(iterator position, InputIterator first,
                InputIterator last)
            {
                if (position == end())
                {
                    for (; first != last; first++)
                    {
                        _alloc.construct(_m_finish, *first);
                        _m_finish++;
                    }
                    return ;
                }
                size_type n = last - first;
                reverse_iterator r_it = rbegin();
                pointer new_end = _m_finish + n - 1;
                for (; new_end != _m_finish - 1;)
                {
                    _alloc.construct(new_end, *r_it++);
                    new_end--;
                }
                for (; r_it != reverse_iterator(position); r_it++)
                {
                    *new_end = *r_it;
                    new_end--;
                }
                _m_finish = _m_finish + n;
                for (InputIterator it = first; it != last; it++)
                {
                    *position = *it;
                    position++;
                }
            }

            void _m_erase_at_end(pointer pos)
            {
                if (size_type n = _m_finish - pos)
                {
                    std::_Destroy(pos, _m_finish, _alloc);
                    _m_finish = pos;
                }
            }

            void _m_swap(vector<T>& other)
            {
                pointer tmp;

                tmp = _m_start;
                _m_start = other._m_start;
                other._m_start = tmp;
                tmp = _m_finish;
                _m_finish = other._m_finish;
                other._m_finish = tmp;
                tmp = _m_end_of_storage;
                _m_end_of_storage = other._m_end_of_storage;
                other._m_end_of_storage = _m_end_of_storage;
            }

            void _throw_out_of_range(size_type n, size_type size)
            {
                std::stringstream ss;
                ss << "vector::_M_range_check: __n (which is " << n << ")";
                ss << " >= this->size() (which is " << size << ")";
                throw std::out_of_range(ss.str());
            }

        public:
            /**
             *  @defgroup Coplien's form (Constructor, Deconstructor, Assign)
             */

            /// Default constructor
            explicit vector(const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _m_start(), _m_finish(), _m_end_of_storage() {}

            /// Fill constructor
            explicit vector(size_type n, const value_type& val = value_type(),
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc)
            {
                _m_create_storage(n);
                _m_finish = std::uninitialized_fill_n(_m_start, n, val);
            }

            /// Copy constructor
            vector(const vector& src) : _alloc(src._alloc)
            {
                _m_create_storage(src.size());
                _m_finish = std::uninitialized_copy(src.begin(), src.end(), _m_start);
            }

            /// Range Constructor
            template <typename InputIterator>
            vector(InputIterator first, InputIterator last,
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc)
            {
                // Checking if it is integral type
                typedef typename ft::is_integral<InputIterator>::type integral;
                _m_initial_range(first, last,
                            integral());
            }

            /// Assignment operator
            vector& operator=(const vector& src)
            {
                clear();
                _m_deallocate();
                _m_create_storage(src.size());
                _m_finish = std::uninitialized_copy(src.begin(), src.end(), _m_start);
                return *this;
            }

            /// Deconstructor
            virtual ~vector()
            {
                clear();
                _m_deallocate();
            }

            /**
             *  @defgroup Iterators member function
             */

            iterator begin(void)
            { return iterator(_m_start); }

            const_iterator begin(void) const
            { return const_iterator(_m_start); }

            iterator end(void)
            { return iterator(_m_finish); }

            const_iterator end(void) const
            { return const_iterator(_m_finish); }

            reverse_iterator rbegin(void)
            { return reverse_iterator(end()); }

            const_reverse_iterator rbegin(void) const
            { return const_reverse_iterator(end()); }

            reverse_iterator rend(void)
            { return reverse_iterator(begin()); }

            const_reverse_iterator rend(void) const
            { return const_reverse_iterator(begin()); }

            const_iterator cbegin(void) const
            { return const_iterator(_m_start); }

            const_iterator cend(void) const
            { return const_iterator(_m_finish); }

            const_reverse_iterator crbegin(void) const
            { return const_reverse_iterator(end()); }

            const_reverse_iterator crend(void) const
            { return const_reverse_iterator(begin()); }

            /**
             *  @defgroup Capacity method of vector
             */

            size_type size(void) const
            { return _m_finish - _m_start; }

            size_type max_size(void) const
            {
                size_type diff_max = std::numeric_limits<ptrdiff_t>::max() / sizeof(value_type);
                size_type alloc_max = _alloc.max_size();
                return std::min(diff_max, alloc_max);
            }

            size_type capacity(void) const
            { return _m_end_of_storage - _m_start; }

            void resize(size_type n, value_type val = value_type())
            {
                if (n < size())
                    _m_erase_at_end(_m_start + n);
                else if (n > size())
                {
                    if (n <= capacity())
                    {
                        size_type fill_n = n - size();
                        std::uninitialized_fill_n(_m_finish, fill_n, val);
                        _m_finish = _m_finish + fill_n;
                    }
                    else
                    {
                        size_type old_size = size();
                        vector<T> tmp(n);
                        tmp._m_copy_range_to_begin(begin(), end());
                        std::uninitialized_fill(tmp.begin() + old_size, tmp.end(), val);
                        *this = tmp;
                    }
                }
            }

            bool empty(void) const
            { return size() == 0 ? true : false; }

            void reserve(size_type n)
            {
                if (n <= capacity() || n == 0)
                    return ;
                _m_reallocate(n);
            }

            void shrink_to_fit(void)
            {
                size_type n = size();
                if (capacity() == n)
                    return ;
                _m_reallocate(n);
            }

            /**
             *  @defgroup Element access
             */
            reference operator[](size_type n)
            { return *(_m_start + n); }

            const_reference operator[](size_type n) const
            { return *(_m_start + n); }

            reference at(size_type n)
            {
                if (n >= size())
                    _throw_out_of_range(n, size());
                return (*this)[n];
            }

            const_reference at(size_type n) const
            {
                if (n >= size())
                    _throw_out_of_range(n, size());
                return (*this)[n];
            }

            reference front(void)
            { return *begin(); }

            const_reference front(void) const
            { return *begin(); }

            reference back(void)
            { return *(end() - 1); }

            const_reference back(void) const
            { return *(end() - 1); }

            pointer data(void)
            { return _m_start; }

            const_pointer data(void) const
            { return _m_start; }

            /**
             *  @defgroup Modifier member-function
             */

            /**
             *  @brief Assign vector element with given value or range of value
             */
            template<typename InputIterator>
            void assign(InputIterator first, InputIterator last)
            {
                // Check whether it's an integral type or not
                typedef typename ft::is_integral<InputIterator>::type integral;
                _m_assign_range(first, last, integral());
            }

            void assign(size_type n, const value_type& val)
            { _m_fill_assign(n, val); }

            /**
             *  @brief Push the element to the end of vector
             */

            void push_back(const value_type& val)
            {
                if (size() < capacity())
                {
                    _alloc.construct(_m_finish, val);
                    _m_finish = _m_finish + 1;
                }
                else
                {
                    _m_grow();
                    _alloc.construct(_m_finish, val);
                    _m_finish = _m_finish + 1;
                }
            }

            /**
             *  @brief Delete the element at the end of vector (No protect need)
             */

            void pop_back(void)
            { _m_erase_at_end(_m_finish - 1); }

            /**
             *  @brief Insert an element to vector
             */

            iterator insert(iterator position, const value_type& val)
            { return _m_insert_rval(position, val); }

            /**
             *  @brief Insert element to range of vector
             */

            void insert(iterator position, size_type n, const value_type& val)
            { _m_insert_fill(position, n, val); }

            /**
             *  @brief Insert range of element to vector
             */

            template <typename InputIterator>
            void insert(iterator position, InputIterator first, InputIterator last,
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL)
            { _m_insert_range(position, first, last); }

            /// Destroy all components in vector
            void clear(void)
            { _m_erase_at_end(_m_start); }
    };
}

#endif /* __FT_VECTOR_HPP__ */
