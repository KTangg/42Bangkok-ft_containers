/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:52:28 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 21:37:13 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SET_ITERATOR_HPP__
# define __SET_ITERATOR_HPP__

#include "../tree/red_black_tree.hpp"

namespace ft
{

    template<typename _T>
    struct _set_iterator
    {
        typedef const _T                                    value_type;
        typedef const _T&                                   reference;
        typedef const _T*                                   pointer;

        typedef std::bidirectional_iterator_tag             iterator_category;
        typedef ptrdiff_t                                   difference_type;

        typedef _set_iterator<_T>                           _self;
        typedef _RbNode<_T, _T>*                            node_ptr;

        /**
         *  @brief Attribute inside Iterator
         */
        node_ptr    _node;

        /**
         *  @brief Default constructor
         */
        _set_iterator()
        : _node() { }

        /**
         *  @brief Initialize constructor
         */
        explicit
        _set_iterator(node_ptr node)
        : _node(node) { }

        reference
        operator*() const
        { return _node->key(); }

        pointer
        operator->() const
        { return &_node->key(); }

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

    template<typename _T>
    struct _set_const_iterator : _set_iterator<_T>
    {
        typedef _set_iterator<_T>                           iterator;
        typedef typename iterator::pointer                  pointer;
        typedef typename iterator::reference                reference;

        typedef _set_const_iterator<_T>                     _self;
        typedef const _RbNode<_T, _T>*                      node_ptr;

        /**
         *  @brief Attribute inside Iterator
         */
        node_ptr    _node;

        _set_const_iterator()
        : _node() { }

        explicit
        _set_const_iterator(node_ptr node)
        : _node(node) { }

        _set_const_iterator(const iterator& it)
        : _node(it._node) { }

        iterator
        _const_cast() const
        { return iterator(const_cast<typename iterator::node_ptr>(_node)); }

        reference
        operator*() const
        { return _node->key(); }

        pointer
        operator->() const
        { return &_node->key(); }

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
        operator==(const _self& lhs, const iterator& rhs)
        { return lhs._node == rhs._node; }

        friend bool
        operator==(const iterator& lhs, const _self& rhs)
        { return lhs._node == rhs._node; }

        friend bool
        operator!=(const _self& lhs, const _self& rhs)
        { return lhs._node != rhs._node; }

        friend bool
        operator!=(const _self& lhs, const iterator& rhs)
        { return lhs._node != rhs._node; }

        friend bool
        operator!=(const iterator& lhs, const _self& rhs)
        { return lhs._node != rhs._node; }

    }; /* _set_const_iterator */

} /* namespace ft */

#endif /* __SET_ITERATOR_HPP__ */
