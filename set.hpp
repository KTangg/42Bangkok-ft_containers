/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:32:20 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 21:39:01 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SET_HPP__
# define __SET_HPP__

# include "tree/red_black_tree.hpp"

# include "iterator/iterator.hpp"
# include "iterator/set_iterator.hpp"

# include "utils/utility.hpp"
# include "utils/algorithm.hpp"

namespace ft
{
    template <typename _T, typename _Compare = std::less<_T>, typename _Alloc = std::allocator<_T> >
    class set
    {
        /**
         *  @defgroup Alias for further use
         */
        public:

            typedef _T                                          key_type;
            typedef _T                                          value_type;

            typedef _Compare                                    key_compare;
            typedef _Compare                                    value_compare;
            typedef typename _Alloc::template
                rebind< _RbNode<_T, _T> >::other                allocator_type;
            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef _set_iterator<_T>                           iterator;
            typedef _set_const_iterator<_T>                     const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

            typedef ptrdiff_t                                   difference_type;
            typedef size_t                                      size_type;

        private:
            /**
             *  @brief Map using red-black-tree container under the hood
             */
            allocator_type                                      _alloc;
            key_compare                                         _cmp;
            typename ft::_RbTree<_T, _T, _Compare, _Alloc>      _tree;

            typedef _RbNode<_T, _T>*                            node_ptr;
            typedef const _RbNode<_T, _T>*                      const_node_ptr;

            node_ptr
            _insert(const value_type& _val)
            {
                if (_tree.search(_val) == _tree.leaf())
                    return _tree.insert(ft::make_pair(_val, _val));
                return NULL;
            }

        public:
            /**
             *  @brief Default constructor
             */
            explicit
            set(const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _cmp(comp), _tree(comp, alloc) { }

            /**
             *  @brief Range constructor
             */
            template <class InputIterator>
            set(InputIterator first, InputIterator last,
                const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _cmp(comp), _tree(comp, alloc)
            {
                for (; first != last; ++first)
                    _insert(*first);
            }

            /**
             *  @brief Copy constructor
             */
            set(const set& src)
            : _alloc(src.get_allocator()), _cmp(src.key_comp()), _tree()
            { *this = src; }

            /**
             *  @brief Deconstructor
             */
            ~set() { }

            /**
             *  @brief Assignment operator
             */
            set& operator=(const set& src)
            {
                _tree.clear();
                for (const_iterator it = src.begin(); it != src.end(); ++it)
                    _insert(*it);
                return *this;
            }

            /**
             *  @brief Getter function
             */
            key_compare key_comp() const
            { return key_compare(_cmp); }

            value_compare value_comp() const
            { return value_compare(_cmp); }

            allocator_type get_allocator() const
            { return allocator_type(_alloc); }

            /**
             *  @brief Return minimum element of tree
             */
            iterator
            begin()
            { return iterator(_tree.root()->minimum()); }

            const_iterator
            begin() const
            { return const_iterator(_tree.root()->minimum()); }

            iterator
            end()
            {
                if (empty())
                    return begin();
                return iterator(_tree.root()->maximum()->increment());
            }

            const_iterator
            end() const
            {
                if (empty())
                    return begin();
                return const_iterator(_tree.root()->maximum()->increment());
            }

            reverse_iterator
            rbegin()
            { return reverse_iterator(end()); }

            const_reverse_iterator
            rbegin() const
            { return const_reverse_iterator(end()); }

            reverse_iterator
            rend()
            { return reverse_iterator(begin()); }

            const_reverse_iterator
            rend() const
            { return reverse_iterator(begin()); }

            bool
            empty() const
            { return _tree.size() == 0; }

            size_type
            size() const
            { return _tree.size(); }

            size_type
            max_size() const
            { return _alloc.max_size(); }

            pair<iterator, bool>
            insert(const value_type& val)
            {
                node_ptr _node = _tree.search(val);
                if (_node == _tree.leaf())
                {
                    _node = _insert(val);
                    return ft::make_pair<iterator, bool>(iterator(_node), true);
                }
                return ft::make_pair<iterator, bool>(iterator(_node), false);
            }

            iterator
            insert(iterator position, const value_type& val)
            {
                (void) position;
                node_ptr _node = _tree.search(val);
                if (_node == _tree.leaf())
                {
                    _node = _insert(val);
                    return iterator(_node);
                }
                return iterator(_node);
            }

            template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                for (; first != last; ++first)
                    _insert(*first);
            }

            void
            erase(iterator position)
            { _tree.erase(position.base()); }

            size_type
            erase(const value_type& val)
            {
                if (_tree.erase(val))
                    return 1;
                return 0;
            }

            void
            erase(iterator first, iterator last)
            {
                for (iterator it; first != last;)
                {
                    it = first;
                    ++first;
                    _tree.erase(it.base());
                }
            }

            void
            swap(set& x)
            { _tree.swap(x._tree); }

            void
            clear()
            { _tree.clear(); }

            iterator
            find(const value_type& val)
            { return iterator(_tree.search(val)); }

            const_iterator
            find(const value_type& val) const
            { return const_iterator(_tree.search(val)); }

            size_type
            count(const value_type& val) const
            {
                if (_tree.search(val) == _tree.leaf())
                    return 0;
                return 1;
            }

            iterator lower_bound(const value_type& val)
            {
                iterator it = begin();
                for ( ; _cmp(*it, val) && it != end(); ++it)
                    ;
                return it;
            }

            const_iterator lower_bound(const value_type& val) const
            {
                const_iterator it = begin();
                for ( ; _cmp(*it, val) && it != end(); ++it)
                    ;
                return it;
            }

            iterator upper_bound(const value_type& val)
            {
                iterator it = begin();
                for ( ; !_cmp(val, *it) && it != end(); ++it)
                    ;
                return it;
            }

            const_iterator upper_bound(const value_type& val) const
            {
                const_iterator it = begin();
                for ( ; !_cmp(val, *it) && it != end(); ++it)
                    ;
                return it;
            }

            typename ft::pair<iterator, iterator>
            equal_range(const value_type& val)
            { return ft::make_pair(lower_bound(val), upper_bound(val)); }

            typename ft::pair<const_iterator, const_iterator>
            equal_range(const value_type& val) const
            { return ft::make_pair(lower_bound(val), upper_bound(val)); }

            template <typename T, typename Compare, typename Alloc>
            friend bool operator==(const set<T, Compare, Alloc> &lhs,
                        const set<T, Compare, Alloc> &rhs);

            template <typename T, typename Compare, typename Alloc>
            friend bool operator<(const set<T, Compare, Alloc> &lhs,
                        const set<T, Compare, Alloc> &rhs);

    }; /* class set */

    template <typename T, typename Compare, typename Alloc>
    bool operator==(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    {
        if (lhs._tree.size() == rhs._tree.size())
            return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
        return false;
    }

    template <typename T, typename Compare, typename Alloc>
    bool operator<(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(),
            rhs.begin(), rhs.end());
    }

    template <typename T, typename Compare, typename Alloc>
    bool operator!=(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    { return !(lhs == rhs); }

    template <typename T, typename Compare, typename Alloc>
    bool operator>(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    { return rhs < lhs; }

    template <typename T, typename Compare, typename Alloc>
    bool operator<=(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    { return !(rhs < lhs); }

    template <typename T, typename Compare, typename Alloc>
    bool operator>=(const set<T, Compare, Alloc> &lhs,
                const set<T, Compare, Alloc> &rhs)
    { return !(lhs < rhs); }

    template <typename T, typename Compare, typename Alloc>
    inline void
    swap(set<T, Compare, Alloc> &lhs,
        set<T, Compare, Alloc> &rhs)
    { lhs.swap(rhs); }

} /* namespace ft */

#endif /*__SET_HPP__ */
