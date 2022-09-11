/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_black_iterator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 13:27:17 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 17:12:08 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RED_BLACK_ITERATOR_HPP__
# define __RED_BLACK_ITERATOR_HPP__

#include "../tree/red_black_tree.hpp"

namespace ft
{
    template<typename _Key, typename _T>
    struct _Rb_iterator
    {
        typedef typename ft::pair<const _Key, _T>       value_type;
        typedef typename ft::pair<const _Key, _T>&      reference;
        typedef typename ft::pair<const _Key, _T>*      pointer;

        typedef std::bidirectional_iterator_tag         iterator_category;
        typedef ptrdiff_t                               difference_type;

        typedef _Rb_iterator<_Key, _T>                  _self;
        typedef _RbNode<_Key, _T>*                      node_ptr;

        /**
         *  @brief Attribute inside Iterator
         */
        node_ptr    _node;

        /**
         *  @brief Default constructor
         */
        _Rb_iterator()
        : _node() { }

        /**
         *  @brief Initialize constructor
         */
        explicit
        _Rb_iterator(node_ptr node)
        : _node(node) { }

        reference
        operator*() const
        { return _node->_data; }

        pointer
        operator->() const
        { return &_node->_data; }

        _self&
        operator++()
        {
            _node = _node->increment();
            return *this;
        }

        _self
        operator++(int)
        {
            _self tmp = *this;
            _node = _node->increment();
            return tmp;
        }

        _self&
        operator--()
        {
            _node = _node->decrement();
            return *this;
        }

        _self
        operator--(int)
        {
            _self tmp = *this;
            _node = _node->decrement();
            return tmp;
        }

        friend bool
        operator==(const _self& lhs, const _self& rhs)
        { return lhs._node == rhs._node; }

        friend bool
        operator!=(const _self& lhs, const _self& rhs)
        { return lhs._node != rhs._node; }

        node_ptr
        base()
        { return _node; }

    }; /* struct _Rb_iterator */

    template<typename _Key, typename _T>
    struct _Rb_const_iterator
    {
        typedef const typename ft::pair<const _Key, _T>       value_type;
        typedef const typename ft::pair<const _Key, _T>&      reference;
        typedef const typename ft::pair<const _Key, _T>*      pointer;

        typedef _Rb_iterator<_Key, _T>                  iterator;

        typedef std::bidirectional_iterator_tag         iterator_category;
        typedef ptrdiff_t                               difference_type;

        typedef _Rb_const_iterator<_Key, _T>            _self;
        typedef const _RbNode<_Key, _T>*                node_ptr;

        /**
         *  @brief Attribute inside Iterator
         */
        node_ptr    _node;

        _Rb_const_iterator()
        : _node() { }

        explicit
        _Rb_const_iterator(node_ptr node)
        : _node(node) { }

        _Rb_const_iterator(const iterator& it)
        : _node(it._node) { }

        iterator
        _const_cast() const
        { return iterator(const_cast<typename iterator::node_ptr>(_node)); }

        reference
        operator*() const
        { return _node->_data; }

        pointer
        operator->() const
        { return &_node->_data; }

        _self&
        operator++()
        {
            _node = _node->increment();
            return *this;
        }

        _self
        operator++(int)
        {
            _self tmp = *this;
            _node = _node->increment();
            return tmp;
        }

        _self&
        operator--()
        {
            _node = _node->decrement();
            return *this;
        }

        _self
        operator--(int)
        {
            _self tmp = *this;
            _node = _node->decrement();
            return tmp;
        }

        node_ptr
        base()
        { return _node; }

        friend bool
        operator==(const _self& lhs, const _self& rhs)
        { return lhs._node == rhs._node; }

        friend bool
        operator!=(const _self& lhs, const _self& rhs)
        { return lhs._node != rhs._node; }

    }; /* _Rb_const_iterator */

} /* namespace ft */

#endif /* __RED_BLACK_ITERATOR_HPP__ */
