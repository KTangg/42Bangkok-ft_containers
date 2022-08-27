/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 20:31:13 by spoolpra          #+#    #+#             */
/*   Updated: 2022/08/27 16:50:06 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_VECTOR_HPP__
# define __FT_VECTOR_HPP__

# include <iostream>
# include <memory>
# include <limits>
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
            void _m_copy_range_to_begin(InputIterator first, InputIterator last)
            {
                iterator dest = begin();
                for (InputIterator it = first; it != last; it++, dest++)
                {
                    *dest = *it;
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

            void _m_erase_at_end(pointer pos)
            {
                if (size_type n = _m_finish - pos)
                {
                    std::_Destroy(pos, _m_finish, _alloc);
                    _m_finish = pos;
                }
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
                pointer _m_start_tmp = _m_allocate(n);
                pointer _m_finish_tmp = std::uninitialized_copy(_m_start, _m_finish, _m_start_tmp);
                pointer _m_end_of_storage_tmp = _m_start_tmp + n;
                clear();
                _m_deallocate();
                _m_start = _m_start_tmp;
                _m_finish = _m_finish_tmp;
                _m_end_of_storage = _m_end_of_storage_tmp;
            }

            /// Destroy all components in vector
            void clear(void)
            { _m_erase_at_end(_m_start); }
    };
}

#endif /* __FT_VECTOR_HPP__ */
