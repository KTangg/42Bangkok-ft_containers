/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:32:20 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 19:58:47 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MAP_HPP__
# define __MAP_HPP__

# include "tree/red_black_tree.hpp"

# include "iterator/iterator.hpp"
# include "iterator/red_black_iterator.hpp"

# include "utils/utility.hpp"
# include "utils/algorithm.hpp"

namespace ft
{
    template < typename _Key, typename _T, typename _Compare = std::less<_Key>,
        typename _Alloc = std::allocator< typename ft::pair<_Key, _T> > >
    class map
    {
        /**
         *  @defgroup Alias for further use
         */
        public:
            class _ValueCompare;

            typedef _Key                                        key_type;
            typedef _T                                          mapped_type;
            typedef typename ft::pair<const _Key, _T>           value_type;

            typedef _Compare                                    key_compare;
            typedef _ValueCompare                               value_compare;
            typedef typename _Alloc::template
                rebind< _RbNode<_Key, _T> >::other                allocator_type;
            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef _Rb_iterator<_Key, _T>                      iterator;
            typedef _Rb_const_iterator<_Key, _T>                const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

            typedef ptrdiff_t                                   difference_type;
            typedef size_t                                      size_type;

        /**
         *  @def _ValueCompare nested class
         *
         *  @ref https://cplusplus.com/reference/map/map/value_comp/
         */
        class _ValueCompare
        : public std::binary_function<value_type, value_type, bool>
        {
            public:
                friend class    map;

            protected:
                _Compare        _cmp;
                _ValueCompare(_Compare c) : _cmp(c) { }

            public:
                typedef bool            result_type;
                typedef value_type      first_argument_type;
                typedef value_type      second_argument_type;
                bool operator() (const value_type& lhs, const value_type& rhs) const
                {
                    return _cmp(lhs.first, rhs.first);
                }
        };

        private:
            /**
             *  @brief Map using red-black-tree container under the hood
             */
            allocator_type                                       _alloc;
            key_compare                                          _cmp;
            typename ft::_RbTree<_Key, _T, _Compare, _Alloc>     _tree;

            typedef _RbNode<_Key, _T>*                          node_ptr;
            typedef const _RbNode<_Key, _T>*                    const_node_ptr;

            node_ptr
            _insert(const value_type& _val)
            {
                if (_tree.search(_val.first) == _tree.leaf())
                    return _tree.insert(_val);
                return NULL;
            }

        public:
            /**
             *  @brief Default constructor
             */
            explicit
            map(const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _cmp(comp), _tree(comp, alloc) { }

            /**
             *  @brief Range constructor
             */
            template <class InputIterator>
            map (InputIterator first, InputIterator last,
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
            map(const map& src)
            : _alloc(src.get_allocator()), _cmp(src.key_comp()), _tree()
            { *this = src; }

            /**
             *  @brief Deconstructor
             */
            ~map() { }

            /**
             *  @brief Assignment operator
             */
            map& operator=(const map& src)
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
                return const_iterator(const_cast< _RbNode<_Key, _T>* >(_tree.root()->maximum()->increment()));
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

            mapped_type&
            operator[](const key_type& k)
            {
                if (_tree.search(k) == _tree.leaf())
                    _insert(ft::make_pair<key_type, mapped_type>(k, mapped_type()));
                return _tree.search(k)->value();
            }

            mapped_type&
            at(const key_type& k)
            {
                if (_tree.search(k) == _tree.leaf())
                    throw std::out_of_range("map: key is not in map");
                return _tree.search(k)->value();
            }

            const mapped_type&
            at(const key_type& k) const
            {
                if (_tree.search(k) == _tree.leaf())
                    throw std::out_of_range("map: key is not in map");
                return _tree.search(k)->value();
            }

            pair<iterator, bool>
            insert(const value_type& val)
            {
                node_ptr _node = _tree.search(val.first);
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
                node_ptr _node = _tree.search(val.first);
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
            erase(const key_type& k)
            {
                if (_tree.erase(k))
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
            swap(map& x)
            { _tree.swap(x._tree); }

            void
            clear()
            { _tree.clear(); }

            iterator find(const key_type& k)
            { return iterator(_tree.search(k)); }

            const_iterator find(const key_type& k) const
            { return const_iterator(_tree.search(k)); }

            size_type
            count(const key_type& k) const
            {
                if (_tree.search(k) == _tree.leaf())
                    return 0;
                return 1;
            }

            iterator lower_bound(const key_type& k)
            {
                iterator it = begin();
                for ( ; _cmp(it->first, k) && it != end(); ++it)
                    ;
                return it;
            }

            const_iterator lower_bound(const key_type& k) const
            {
                const_iterator it = begin();
                for ( ; _cmp(it->first, k) && it != end(); ++it)
                    ;
                return it;
            }

            iterator upper_bound(const key_type& k)
            {
                iterator it = begin();
                for ( ; !_cmp(k, it->first) && it != end(); ++it)
                    ;
                return it;
            }

            const_iterator upper_bound(const key_type& k) const
            {
                const_iterator it = begin();
                for ( ; !_cmp(k, it->first) && it != end(); ++it)
                    ;
                return it;
            }

            typename ft::pair<iterator, iterator>
            equal_range(const key_type& k)
            { return ft::make_pair(lower_bound(k), upper_bound(k)); }

            typename ft::pair<const_iterator, const_iterator>
            equal_range(const key_type& k) const
            { return ft::make_pair(lower_bound(k), upper_bound(k)); }

            template <typename Key, typename T, typename Compare, typename Alloc>
            friend bool operator==(const map<Key, T, Compare, Alloc> &lhs,
                        const map<Key, T, Compare, Alloc> &rhs);

            template <typename Key, typename T, typename Compare, typename Alloc>
            friend bool operator<(const map<Key, T, Compare, Alloc> &lhs,
                        const map<Key, T, Compare, Alloc> &rhs);

    }; /* class map */

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator==(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    {
        if (lhs._tree.size() == rhs._tree.size())
            return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
        return false;
    }

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator<(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(),
            rhs.begin(), rhs.end());
    }

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator!=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    { return !(lhs == rhs); }

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator>(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    { return rhs < lhs; }

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator<=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    { return !(rhs < lhs); }

    template <typename Key, typename T, typename Compare, typename Alloc>
    bool operator>=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs)
    { return !(lhs < rhs); }

    template <typename Key, typename T, typename Compare, typename Alloc>
    inline void
    swap(map<Key, T, Compare, Alloc> &lhs,
        map<Key, T, Compare, Alloc> &rhs)
    { lhs.swap(rhs); }

} /* namespace ft */

#endif /*__MAP_HPP__ */
