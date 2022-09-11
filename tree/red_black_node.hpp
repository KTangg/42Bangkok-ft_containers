/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_black_node.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 19:50:40 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 21:20:55 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RED_BLACK_NODE_HPP__
# define __RED_BLACK_NODE_HPP__

# include <memory>
# include "../utils/utility.hpp"

namespace ft
{
    enum _RbColor { _red = false, _black = true };
    template <typename _Key, typename _T>
    struct _RbNode
    {
        /**
         *  @defgroup Alias for further use
         */
        typedef _RbNode*            node_ptr;
        typedef const _RbNode*      const_node_ptr;

        typedef _Key                            key_type;
        typedef _T                              mapped_type;
        typedef typename ft::pair<const _Key, _T>     value_type;

        /**
         *  @defgroup _Rb_node attributes
         */
        _RbColor    _color;
        node_ptr    _parent;
        node_ptr    _left;
        node_ptr    _right;
        node_ptr    _leaf;

        value_type   _data;

        /**
         *  @brief Node default construct
         */
        _RbNode()
        : _color(), _parent(), _left(), _right(), _leaf(), _data() { }

        /**
         *  @brief Initialize construct
         */
        explicit
        _RbNode(const value_type& data, node_ptr parent = 0)
        : _color(), _parent(parent), _left(), _right(),  _leaf(), _data(data) { }

        /**
         *  @brief Node deconstructor
         */
        ~_RbNode() { }

        /**
         *  @brief Getter for node key
         */
        const key_type& key(void) const
        { return _data.first; }

        /**
         *  @brief Getter for node value
         */
        mapped_type& value(void)
        { return _data.second; }

        /**
         *  @brief const Getter for node value
         */
        const mapped_type& value(void) const
        { return _data.second; }

        /**
         *  @brief Check is this is right child or not
         */
        bool
        _is_right(void) const
        {
            if (!_parent)
                return false;
            return _parent->_right == this;
        }

        /**
         *  @brief Check is this is left child or not
         */
        bool
        _is_left(void) const
        {
            if (!_parent)
                return false;
            return _parent->_left == this;
        }

        /**
         *  @brief Find minimum key from this node
         */
        node_ptr
        minimum(void)
        {
            node_ptr _node = this;
            while (_node->_left != _leaf)
                _node = _node->_left;
            return _node;
        }

        /**
         *  @brief Find minimum key from this node
         */
        const_node_ptr
        minimum(void) const
        {
            const_node_ptr _node = this;
            while (_node->_left != _leaf)
                _node = _node->_left;
            return _node;
        }


        /**
         *  @brief Find maximum key from this node
         */
        node_ptr
        maximum(void)
        {
            node_ptr _node = this;
            while (_node->_right != _leaf)
                _node = _node->_right;
            return _node;
        }

        /**
         *  @brief Find maximum key from this node
         */
        const_node_ptr
        maximum(void) const
        {
            const_node_ptr _node = this;
            while (_node->_right != _leaf)
                _node = _node->_right;
            return _node;
        }

        /**
         *  @brief Increment node
         */
        node_ptr
        increment(void)
        {
            node_ptr _node = this;
            if (_node->_right != _leaf)
                return (_node->_right->minimum());
            while (_node->_is_right())
                _node = _node->_parent;
            if (_node->_parent == NULL)
                return _leaf;
            return _node->_parent;
        }

        const_node_ptr
        increment(void) const
        {
            const_node_ptr _node = this;
            if (_node->_right != _leaf)
                return (_node->_right->minimum());
            while (_node->_is_right())
                _node = _node->_parent;
            if (_node->_parent == NULL)
                return _leaf;
            return _node->_parent;
        }

        /**
         *  @brief Decrement node
         */
        node_ptr
        decrement(void)
        {
            node_ptr _node = this;
            if (_node->_left != _leaf)
                return _node->_left->maximum();
            while (_node->_is_left())
                _node = _node->_parent;
            if (_node->_parent == NULL)
                return _leaf;
            return _node->_parent;
        }

        const_node_ptr
        decrement(void) const
        {
            const_node_ptr _node = this;
            if (_node->_left != _leaf)
                return _node->_left->maximum();
            while (_node->_is_left())
                _node = _node->_parent;
            if (_node->_parent == NULL)
                return _leaf;
            return _node->_parent;
        }

    }; /* _RbNode */
} /* namepsace */


#endif
