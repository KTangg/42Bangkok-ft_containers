/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 20:31:13 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/05 00:19:36 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_VECTOR_HPP__
# define __FT_VECTOR_HPP__
# include <iostream>
# include <memory>
# include <limits>
# include <algorithm>
# include <exception>
# include <sstream>
# include "iterator/iterator.hpp"
# include "utils/type_traits.hpp"
# include "utils/algorithm.hpp"

namespace ft {
    /**
     *  @class Vector<T>
     *
     *  @brief Vector container using array data structure but with capacity instead of size
     *  always equal to capacity. Capacity reduce frequency of reallocation occured when insert
     *  new element to Vector which is painpoint of array while keep big O(1) when index and insert data.
     *
     *  @tparam T type of element to be collected in this vector
     *  @tparam Alloc allocator type to be use to allocate <T> default to std::allocator<T>
     *
     *  @parblock
     *  @a -> attribute
     *  @a _alloc is allocator to allocate/construct/destroy/dellocate class T
     *  @a _start is pointer to starting point of data collection
     *  @a _finish is pointer to position after last elements
     *  @a _end is end of allocated memory of vector
     *
     *  @def _M_ as Modified
     *  these functions will modify the value in current vector
     *
     *  @def _S_ as Static not statis member function bun unchange one
     *  these functions won't do any thing to current vector just grab some value
     *
     *  @ref linux standard vector
     *  @remark This file wrote down by me trying to understanding what under the hood of Vector container
     *  it's contained a lot of function from linux standard vector but some are modified to be more understandable
     */
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

            typedef ft::normal_iterator<pointer, vector>               iterator;
            typedef ft::normal_iterator<const_pointer, vector>         const_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;

            /**
             *  @defgroup Attribute for container
             */
        private:
            allocator_type  _alloc;
            pointer         _start;
            pointer         _finish;
            pointer         _end;

            /**
             *  @brief Copy all data from src to *this
             *  @warning 2 vector will point to the same location
             */
            void _M_copy_data(const vector& src)
            {
                _start = src._start;
                _finish = src._finish;
                _end = src._end;
            }

            /**
             *  @brief Swap the data contain with given vector
             */
            void _M_swap_data(vector& other)
            {
                vector tmp;
                tmp._M_copy_data(*this);
                _M_copy_data(other);
                other._M_copy_data(tmp);
            }

            /**
             *  @brief Allocate memory @a n size
             *
             *  @a _start to the pointer return from allocation
             *  @a _finish to _start since no element constructed
             *  @a _end to _start + n as the first pointer
             *  after allocated memory block
             *
             *  @warning this function won't check if n is larger than
             *  maximum T entity that could be allocate
             *
             *  @exception std::allocator will throw bad_alloc if it can't
             *  allocate given amount of memory
             */
            void _M_create_storage(size_t n)
            {
                _start = _alloc.allocate(n);
                _finish = _start;
                _end = _start + n;
            }

            /**
             *  @brief Deallocate all memory in current vector
             *
             *  @warning This function isn't destroy the object before
             *  please destroy constructed object before deallocate
             */
            void _M_deallocate()
            {
                if (_start)
                    _alloc.deallocate(_start, _end - _start);
            }

            /**
             *  @brief Reallocate and coied all memory to be size @a n
             *
             *  @param n new vector capacity
             *
             *  @warning reallocate to n < than old one is undefine
             */
            void _M_reallocate(size_t n)
            {
                pointer _tmp_start = _alloc.allocate(n);
                pointer _tmp_finish = _tmp_start;
                pointer _tmp_end = _tmp_start + n;

                _tmp_finish = std::uninitialized_copy(begin(), end(), iterator(_tmp_start)).base();
                clear();
                _M_deallocate();
                _start = _tmp_start;
                _finish = _tmp_finish;
                _end = _tmp_end;
            }

            /**
             *  @brief Pushing element starting from first to last
             *
             *  @remark Input Iterator couldn't use last - first to determine
             *  size of the vector so we need to go one by one. Operation - is not define
             *
             *  @ref https://cplusplus.com/reference/iterator/InputIterator/
             */
            template <typename InputIterator>
            void _M_range_initialize(InputIterator first, InputIterator last,
                std::input_iterator_tag)
            {
                if (!_start && first != last)
                    _M_create_storage(1);
                try {
                    for (; first != last; ++first)
                    {
                        push_back(*first);
                    }
                } catch(...) {
                    clear();
                    throw;
                }
            }

            /**
             *  @brief Copy elements from first to last with allocated space
             *  size equal to last - first
             *
             *  @remark Standard vector using forward_iterator_tag but I can't
             *  distinguist the different between the above template
             */
            template <typename RandomIterator>
            void _M_range_initialize(RandomIterator first, RandomIterator last,
                std::random_access_iterator_tag)
            {
                const size_type n = last - first;
                _start = _alloc.allocate(_S_check_init_len(n));
                _end = _start + n;
                _finish = std::uninitialized_copy(first, last, _start);
            }

            /**
             *  @brief Insert @a val to given position with @a n copied
             *
             *  @param position first inserting position
             *  @param n number of element insert
             *  @param val value insert
             *
             *  @warning filling position after _finish will result in undefined bahavior
             */
            void _M_fill_insert(iterator position, size_type n, value_type val)
            {
                if (n == 0)
                    return ;
                // Leftover capacity is enough
                if (size_type(_end - _finish) >= n)
                {
                    size_type offset;
                    size_type n_move = end() - position;
                    if (n_move > n)
                    {
                        offset = n_move - n;
                        _finish = std::uninitialized_copy(position + offset, end(), end()).base();
                        std::copy_backward(position, position + offset, position + n_move);
                        std::fill_n(position, n, val);
                    }
                    else
                    {
                        iterator _old_finish(_finish);
                        offset = n - n_move;
                        _finish = std::uninitialized_copy(position, end(), position + n).base();
                        std::fill(position, _old_finish, val);
                        std::uninitialized_fill_n(_old_finish, offset, val);
                    }
                }
                // Capacity not enough need to grow
                 else
                {
                    const size_type idx = position.base() - _start;
                    _M_reallocate(_S_check_insert_len(n, "vector::_M_fill_insert"));
                    _M_fill_insert(begin() + idx, n, val);
                }
            }

            /**
             *  @brief Insert range of input iterator into current vector
             *
             *  @param pos position to begin insert in vector
             *  @param first begin of input iterator
             *  @param last end of input iterator
             */
            template <typename InputIterator>
            void _M_range_insert(iterator pos, InputIterator first,
                InputIterator last, std::input_iterator_tag)
            {
                if (pos == end())
                {
                    for (; first != last; ++first)
                        _M_fill_insert(end(), 1, *first);
                }
                else if (first != last)
                {
                    vector tmp(first, last);
                    insert(pos, first, last);
                }
            }
            /**
             *  @brief Insert range of random access iterator into current vector
             *
             *  @param pos position to begin insert in vector
             *  @param first begin of iterator
             *  @param last end of iterator
             */
            template <typename RandomIterator>
            void _M_range_insert(iterator pos, RandomIterator first,
                RandomIterator last, std::random_access_iterator_tag)
            {
                if (first == last)
                    return ;
                const size_type n = last - first;
                // enough capacity to insert
                if (size_type(_end - _finish) >= n)
                {
                    reverse_iterator r_old(rbegin());
                    reverse_iterator r_new(iterator(_finish + n));
                    for (; r_new < rbegin(); ++r_new, ++r_old)
                        _alloc.construct(&(*r_new), *r_old);
                    for (; r_new < reverse_iterator(pos); ++r_new, ++r_old)
                        *r_new = *r_old;
                    for (; first != last; ++first, ++pos)
                        *pos = *first;
                    _finish = _finish + n;
                }
                else
                {
                    const size_type idx = pos - begin();
                    _M_reallocate(_S_check_insert_len(n, "vector::_M_range_insert"));
                    _M_range_insert(begin() + idx, first, last,
                        ft::_iterator_category(first));
                }
            }

            template <typename InputIterator>
            void _M_assign_aux(InputIterator first, InputIterator last,
                std::input_iterator_tag)
            {
                iterator cursor = begin();
                for (; first != last && cursor != end();)
                {
                    *cursor = *first;
                    ++cursor;
                    ++first;
                }
                if (first == last)
                    _M_erase_at_end(cursor.base());
                else
                    insert(end(), first, last);
            }

            template <typename RandomIterator>
            void _M_assign_aux(RandomIterator first, RandomIterator last,
                std::random_access_iterator_tag)
            {
                const size_type len = last - first;
                if (len < size())
                {
                    iterator pos = std::copy(first, last, begin());
                    _M_erase_at_end(pos.base());
                }
                else
                {
                    RandomIterator offset = first + size();
                    std::copy(first, offset, begin());
                    insert(end(), offset, last);
                }
            }

            /**
             *  @brief Delete element from _finish to @a pos
             *
             *  @param pos pointer to new _finish of this vector
             */
            void _M_erase_at_end(pointer pos)
            {
                for (iterator it = iterator(pos); it != end(); ++it)
                    _alloc.destroy(it.base());
                _finish = pos;
            }

            /**
             *  @brief Find max instance that <T> could be constuct
             *
             *  @return Minimum between numeric limit and allocation limit
             */
            size_type
            _S_max_size(void) const
            {
                const size_t numeric_max =
                    std::numeric_limits<ptrdiff_t>::max() / sizeof(T);
                const size_t alloc_max = _alloc.max_size();
                return std::min(numeric_max, alloc_max);
            }

            /**
             *  @brief Check if @a n is larger than max_size() or not
             *
             *  @param n size to be checked
             *
             *  @exception throw std::length_error in case of n larger than max_size()
             */
            size_type
            _S_check_init_len(size_type n) const
            {
                if (n > _S_max_size())
                    throw std::length_error("cannot create std::vector larger than max_size()");
                return n;
            }

            /**
             *  @brief Check if it is possible to insert @a n elements
             *  to current vector
             *
             *  @param n size that want to insert
             *  @param e error print string
             *
             *  @return new vector capacity that capable to insert @a n elements
             *
             *  @exception throw std::length_error(e) in case size() + n > max_size()
             */
            size_type _S_check_insert_len(size_type n, const char* e) const
            {
                if (max_size() - size() < n)
                    throw std::length_error(e);
                const size_type len = size() + std::max(size(), n);
                // In case of overflow return max_size() else len
                return (len < size() || len > max_size()) ? max_size() : len;
            }

            /**
             *  @brief Check wheter @a n is in vector boundary
             *
             *  @param n Position to be checked
             *
             *  @exception std::out_of_range in case @a n is out of bound
             */
            void _S_check_index(size_type n) const
            {
                if (n >= size())
                {
                    std::stringstream ss;
                    ss << "vector::_M_range_check: __n (which is " << n << ") >= this->size() ";
                    ss << "(which is " << size() << ")";
                    throw std::out_of_range(ss.str());
                }
            }

        public:

            /**
             *  @brief Get clone of allocator from vector
             *  @return allocator_type with copied entity
             */
            allocator_type get_allocator(void) const
            { return allocator_type(_alloc); }

            /**
             *  @brief Default constructor empty vector with given allocator type
             *  if not given set to std::allocator<T>
             *
             *  @param alloc allocator to be use in vector
             */
            vector(const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _start(), _finish(), _end() { }

            /**
             *  @brief Fill constructor @a n copies of @a val
             *
             *  @param n size of vector
             *  @param val value initiate of vector element if not given
             *  set val to default value of <T>
             *  @param alloc allocator of type <T> using std::allocator as default
             *
             *  @exception if @a n > max_size() throw std::length_error
             */
            explicit
            vector(size_type n, const value_type& val = value_type(),
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc)
            {
                _M_create_storage(_S_check_init_len(n));
                _finish = std::uninitialized_fill_n(_start, n, val);
            }

            /**
             *  @brief Copy constructor deep copy from its source
             *
             *  @param src source to be copied
             *
             *  @remark All the elements of @a src are deep copied.
             *  The unused capacity in @a src won't be copied
             */
            vector(const vector& src)
            : _alloc(src.get_allocator())
            {
                _M_create_storage(src.size());
                _finish = std::uninitialized_copy(src.begin(), src.end(), _start);
            }

            /**
             *  @brief Range constructor copied from first to last position
             *
             *  @param first start of Input Iterator
             *  @param last end of Input Iterator
             *  @param alloc allocator type
             *
             *  @warning given different object iterator @a first and
             *  @a last will result in undefine behavior
             *
             *  @remark Compiler can't distinguish between InputIterator and integral type
             *  so enable if template is required to help them.
             */
            template <typename InputIterator>
            vector(InputIterator first, InputIterator last,
                const allocator_type& alloc = allocator_type(),
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL)
            : _alloc(alloc), _start(), _finish(), _end()
            { _M_range_initialize(first, last, ft::_iterator_category(first)); }

            /**
             *  @brief Deconstructor clear the constructed elements
             *  deallocate all memory used by vector
             */
            ~vector()
            {
                clear();
                _M_deallocate();
            }

            /**
             *  @brief Assignment operator
             *
             *  @param rhs Vector to be copied
             *
             *  @remark this work almost the same as copy constructor but clearing
             *  vector memory before copy is mandatory
             */
            vector& operator=(const vector& rhs)
            {
                clear();
                _M_deallocate();
                _M_create_storage(rhs.size());
                _finish = std::uninitialized_copy(rhs.begin(), rhs.end(), _start);
                return *this;
            }

            /**
             *  @brief Given first iterator to this vector data
             *
             *  @return iterator that point to _start with read/write access
             */
            iterator begin()
            { return iterator(_start); }

            /**
             *  @brief Given first const iterator to this vector data
             *
             *  @return iterator that point to _start with read only access
             */
            const_iterator begin() const
            { return const_iterator(_start); }

            /**
             *  @brief Given last iterator to this vector data
             *
             *  @return iterator that point to one past _finish with read/write access
             */
            iterator end(void)
            { return iterator(_finish); }

            /**
             *  @brief Given last const iterator to this vector data
             *
             *  @return iterator that point to one past _finish with read only access
             */
            const_iterator end(void) const
            { return const_iterator(_finish); }

            /**
             *  @brief Given first reverse_iterator to this vector data
             *
             *  @return reverse_iterator that point to _finish with read/write access
             */
            reverse_iterator rbegin(void)
            { return reverse_iterator(end()); }

            /**
             *  @brief Given first const reverse_iterator to this vector data
             *
             *  @return reverse_iterator that point to _finish with read only access
             */
            const_reverse_iterator rbegin(void) const
            { return const_reverse_iterator(end()); }

            /**
             *  @brief Given last reverse_iterator to this vector data
             *
             *  @return reverse_iterator that point to one before _start with read/write access
             */
            reverse_iterator rend(void)
            { return reverse_iterator(begin()); }

            /**
             *  @brief Given last const reverse_iterator to this vector data
             *
             *  @return reverse_iterator that point to one before _start with read only access
             */
            const_reverse_iterator rend(void) const
            { return reverse_iterator(begin()); }

            /**
             *  @brief Giving current size of this vector
             *
             *  @return size_type elements count in vector
             */
            size_type size(void) const
            { return size_type(_finish - _start); }

            /**
             *  @brief Giving maximum size possible of this vector
             *
             *  @return size_type of largest number of elements possible to allocate
             */
            size_type max_size(void) const
            { return _S_max_size(); }

            /**
             *  @brief Resize vector to size() equal @a n element
             *  if n < size() -> reduce to its first @a n element
             *  if size() < n -> insert @a val at the end until reach size @a n
             *
             *  @param n new vector size
             *  @param val value to be inserted if not specified use default <T>
             *
             *  @remark if n > capacity() realloc vector
             */
            void resize(size_type n, value_type val = value_type())
            {
                if (n > size())
                    _M_fill_insert(end(), n - size(), val);
                else if (n < size())
                    _M_erase_at_end(_start + n);
            }

            /**
             *  @brief Vector capacity is maximum elements can hold without reallocate
             *
             *  @return size_type capacity of current vector
             */
            size_type capacity(void) const
            { return size_type(_end - _start); }

            /**
             *  @brief Finding wheter the vector is empty
             *
             *  @return true if size() is 0, false otherwisw
             */
            bool empty(void) const
            { return begin() == end(); }

            /**
             *  @brief Requests that the vector capacity be at least enough to contain @a n elements.
             *  If @a n is > capacity() reallocate will occured, otherwise do nothing
             *
             *  @remark this function has no effect on vector size and elements
             */
            void reserve(size_type n)
            {
                if (n > capacity())
                    _M_reallocate(_S_check_init_len(n));
            }

            /**
             *  @brief Indexing <T> position @a n in vector container with read/write access
             *
             *  @return reference of element at position @a n
             *
             *  @remark similar to vector::at, except operator[] not do boundary check
             *
             *  @warning calling n out of range causing undefined behavior
             */
            reference operator[](size_type n)
            { return *(_start + n); }

            /**
             *  @brief Indexing <T> position @a n in vector container with read access
             *
             *  @return const_reference of element at position @a n
             *
             *  @remark similar to vector::at, except operator[] not do boundary check
             *
             *  @warning calling n out of range causing undefined behavior
             */
            const_reference operator[](size_type n) const
            { return *(_start + n); }

            /**
             *  @brief Indexing <T> position @a n in vector container with read/write access
             *  if n > size() throw std::out_of_range
             *
             *  @return reference to element at position @a n
             *
             *  @exception std::out_of_range if n not in vector boundary
             */
            reference at(size_type n)
            {
                _S_check_index(n);
                return *(_start + n);
            }

            /**
             *  @brief Indexing <T> position @a n in vector container with read access
             *  if n > size() throw std::out_of_range
             *
             *  @return reference to element at position @a n
             *
             *  @exception std::out_of_range if n not in vector boundary
             */
            const_reference at(size_type n) const
            {
                _S_check_index(n);
                return *(_start + n);
            }

            /**
             *  @return read/write reference to first value in vector
             */
            reference front(void)
            { return *_start; }

            /**
             *  @return read reference to first value in vector
             */
            const_reference front(void) const
            { return *_start; }

            /**
             *  @return read/write reference to last value in vector
             */
            reference back(void)
            { return *(_finish - 1); }

            /**
             *  @return read reference to last value in vector
             */
            const_reference back(void) const
            { return *(_finish - 1); }

            /**
             *  @return A read/write pointer to the memory array used internally by the vector to store its owned elements.
             */
            pointer data(void)
            { return _start; }

            /**
             *  @return A read pointer to the memory array used internally by the vector to store its owned elements.
             */
            const_pointer data(void) const
            { return _start; }

            /**
             *  @brief Assign new contents to vector, the new contents are elements constructed from
             *  each of the elements in the range between first and last, in the same order.
             *
             *  @param first first element to be assign
             *  @param last stopping iterator postion
             *
             *  @remark Any element held in before are destroyed and replaced.
             *  This will reallocate only if the new vector size surpassed it capacity
             */
            template <class InputIterator>
            void assign(InputIterator first, InputIterator last,
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL)
            { _M_assign_aux(first, last, ft::_iterator_category(first)); }

            /**
             *  @brief Assign new contents to vector, the new contents are n elements,
             *  each initialized to a copy of val.
             *
             *  @param n number of elements to be assign
             *  @param val value of <T> to be assign on elements
             *
             *  @remark Any element held in before are destroyed and replaced.
             *  This will reallocate only if the new vector size surpassed it capacity
             */
            void assign(size_type n, const value_type& val)
            {
                if (n <= size())
                {
                    std::fill_n(begin(), n, val);
                    _M_erase_at_end(_start + n);
                }
                else
                {
                    if (n <= capacity())
                    {
                        const size_type offset = n - size();
                        std::fill(begin(), end(), val);
                        std::uninitialized_fill_n(end(), offset, val);
                        _finish = _start + n;
                    }
                    else
                    {
                        _M_reallocate(_S_check_init_len(n));
                        assign(n, val);
                    }
                }
            }

            /**
             *  @brief Add new element to vector last element. The val is copied to new element
             *
             *  @param val value<T> to be added to vector
             *
             *  @remark Increase size by 1, automatic reallocate if push_back when size = capacity
             */
            void push_back(const value_type& val)
            {
                if (size() < capacity())
                {
                    _alloc.construct(_finish, val);
                    ++_finish;
                }
                else
                {
                    _M_reallocate(_S_check_insert_len(1, "vector::push_back"));
                    _alloc.construct(_finish, val);
                    ++_finish;
                }
            }

            /**
             *  @brief Remove last element from vector decrease size by 1
             */
            void pop_back(void)
            {
                --_finish;
                _alloc.destroy(_finish);
            }

            /**
             *  @brief insert element with @a val value to vector at
             *  position @a position. Move pre-existed element to the right
             *
             *  @param position position to be insert
             *  @param val value to be insert
             *
             *  @return an iterator that point to the first of newly inserted elements
             *
             *  @warning using @a position beyond boundary will be undefined
             */
            iterator insert(iterator position, const value_type& val)
            {
                const size_type idx = position - begin();
                _M_fill_insert(position, 1, val);
                return begin() + idx;
            }

            /**
             *  @brief insert elements with @a val value to vector at
             *  position @a position with @a n copied. Move pre-existed element to the right
             *
             *  @param position first position to be inserted
             *  @param n number of copied to be inserted
             *  @param val value to be inserted
             */
            void insert(iterator position, size_type n, const value_type& val)
            { _M_fill_insert(position, n, val); }

            /**
             *  @brief insert elements to @a position with value from
             *  @a first to @a last iterator
             *
             *  @param position first position to be inserted
             *  @param first begin iterator
             *  @param last end iterator
             */
            template <typename InputIterator>
            void insert(iterator position, InputIterator first, InputIterator last,
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL)
            {
                _M_range_insert(position, first, last,
                    ft::_iterator_category(first));
            }

            /**
             *  @brief Remove element from vector
             *
             *  @param position deleting element
             *
             *  @return iterator pointing to new location of element
             *  follow element erased
             */
            iterator erase(iterator position)
            {
                if (position + 1 != end())
                    std::copy(position + 1, end(), position);
                --_finish;
                _alloc.destroy(_finish);
                return position;
            }

            /**
             *  @brief Remove elements from first to last in vector
             *
             *  @param first start delete position
             *  @param last stop position
             *
             *  @return iterator pointing to new location of element
             *  follow element erased
             */
            iterator erase(iterator first, iterator last)
            {
                if (first == last)
                    return first;
                iterator _new_finish = std::copy(last, end(), first);
                _M_erase_at_end(_new_finish.base());
                return first;
            }

            /**
             *  @brief Swap content inside vector with other
             */
            void swap(vector& other)
            {
                pointer _tmp;

                _tmp = _start;
                _start = other._start;
                other._start = _tmp;

                _tmp = _finish;
                _finish = other._finish;
                other._finish = _tmp;

                _tmp = _end;
                _end = other._end;
                other._end = _tmp;
            }

            /**
             *  @brief Destroy all of elements in current vector
             *  set _finish to _start
             */
            void clear(void)
            { _M_erase_at_end(_start); }
    }; /* class vector */

    template <typename T, typename Alloc>
    bool operator==(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {
        return (lhs.size() == rhs.size()) &&
            ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <typename T, typename Alloc>
    bool operator!=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    { return !(lhs == rhs); }

    template <typename T, typename Alloc>
    bool operator<(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(),
            rhs.begin(), rhs.end());
    }

    template <typename T, typename Alloc>
    bool operator>(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {  return rhs < lhs; }

    template <typename T, typename Alloc>
    bool operator<=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {  return !(rhs < lhs); }

    template <typename T, typename Alloc>
    bool operator>=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {  return !(lhs < rhs); }

    template <typename T, typename Alloc>
    inline void
    swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
    { x.swap(y); }

} /* namespace ft */

#endif /* __FT_VECTOR_HPP__ */
