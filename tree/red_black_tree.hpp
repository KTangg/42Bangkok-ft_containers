/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_black_tree.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:40:07 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/11 17:45:39 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RED_BLACK_TREE_HPP__
# define __RED_BLACK_TREE_HPP__

# include "red_black_node.hpp"

namespace ft
{
    template < typename _Key, typename _T, typename Compare = std::less<_Key>,
        typename _Alloc = std::allocator< typename ft::pair<_Key, _T> > >
    class _RbTree
    {
        /**
         *  @defgroup Alias for further use
         */
        public:
            typedef typename ft::_RbNode<_Key, _T>          node_type;
            typedef typename ft::_RbNode<_Key, _T>*         node_ptr;
            typedef const typename ft::_RbNode<_Key, _T>*   const_node_ptr;

            typedef _Key                                key_type;
            typedef _T                                  mapped_type;
            typedef typename ft::pair<const _Key, _T>   value_type;
            typedef Compare                             key_compare;
            typedef size_t                              size_type;

            typedef typename _Alloc::template
                rebind<node_type>::other  allocator_type;

        private:
            /**
             *  @brief Attibute in _RbTree
             *  @a _alloc allocator for node
             *  @a _f_cmp function to compare key
             *  @a _root for root node in tree
             *  @a _leaf null node with black color
             */
            allocator_type  _alloc;
            key_compare     _f_cmp;
            size_type       _size;

            node_ptr        _root;
            node_ptr        _leaf;

            /**
             *  @brief Create new _RbNode
             *
             *  @return node_ptr to new node created
             */
            node_ptr
            _create_node(void)
            {
                node_ptr _node;

                _node = _alloc.allocate(1);
                _alloc.construct(_node, node_type());
                _node->_color = _black;
                _node->_leaf = NULL;

                return _node;
            }

            /**
             *  @brief Create new _RbNode
             *
             *  @param _data value to be store in new node
             *
             *  @return node_ptr to new node created
             */
            node_ptr
            _create_node(const value_type& _data)
            {
                node_ptr _node;

                _node = _alloc.allocate(1);
                _alloc.construct(_node, node_type(_data));
                _node->_left = _leaf;
                _node->_right = _leaf;
                _node->_leaf = _leaf;

                return _node;
            }

            /**
             *  @brief Destroy and Deallocate node
             *
             *  @param _node pointer to node that will be deallocated
             */
            void
            _deallocate_node(node_ptr _node)
            {
                if (!_node)
                    return ;
                _alloc.destroy(_node);
                _alloc.deallocate(_node, 1);
            }

            /**
             *  @brief Update leaf to have it child point to max and min node in tree
             */
            void
            _update_leaf(void)
            {
                if (size() == 0)
                {
                    _leaf->_left = NULL;
                    _leaf->_right = NULL;
                    return ;
                }
                _leaf->_left = _root->maximum();
                _leaf->_right = _root->minimum();
            }

            /**
             *  @brief Check wheter is this node is right child
             *  false on root node
             */
            static bool
            _S_is_right(node_ptr _node)
            {
                if (!_node->_parent)
                    return false;
                return (_node->_parent->_right == _node);
            }

            /**
             *  @brief Check wheter is this node is left child
             *  false on root node
             */
            static bool
            _S_is_left(node_ptr _node)
            {
                if (!_node->_parent)
                    return false;
                return (_node->_parent->_left == _node);
            }

            /**
             *  @brief Search for node that key equal to given key
             *
             *  @param _node pointer to begin of search node
             *  @param _key key to be searched
             *
             *  @return node_ptr to the node that contain key value or _leaf node
             */
            node_ptr
            _search_tree(node_ptr _node, key_type _key)
            {
                if (_node == _leaf)
                    return _node;
                key_type _node_key = _node->key();
                if (_key == _node_key)
                    return _node;
                if (_f_cmp(_key, _node_key))
                    return _search_tree(_node->_left, _key);
                return _search_tree(_node->_right, _key);
            }

            const_node_ptr
            _search_tree(node_ptr _node, key_type _key) const
            {
                if (_node == _leaf)
                    return _node;
                key_type _node_key = _node->key();
                if (_key == _node_key)
                    return _node;
                if (_f_cmp(_key, _node_key))
                    return _search_tree(_node->_left, _key);
                return _search_tree(_node->_right, _key);
            }

            void _right_rotate(node_ptr _node)
            {
                node_ptr _child = _node->_left;
                _node->_left = _child->_right;
                if (_child->_right != _leaf)
                    _child->_right->_parent = _node;
                _child->_parent = _node->_parent;
                if (_node->_parent == NULL)
                    _root = _child;
                else if (_S_is_right(_node))
                    _node->_parent->_right = _child;
                else
                    _node->_parent->_left = _child;
                _child->_right = _node;
                _node->_parent = _child;
            }

            void _left_rotate(node_ptr _node)
            {
                node_ptr _child = _node->_right;
                _node->_right = _child->_left;
                if (_child->_left != _leaf)
                    _child->_left->_parent = _node;
                _child->_parent = _node->_parent;
                if (_node->_parent == NULL)
                    _root = _child;
                else if (_S_is_left(_node))
                    _node->_parent->_left = _child;
                else
                    _node->_parent->_right = _child;
                _child->_left = _node;
                _node->_parent = _child;
            }

            /**
             *  @brief balancing tree after insert @a node to tree
             *
             *  @param node newly insert node position
             */
            void
            _balance_insert_tree(node_ptr _node)
            {
                node_ptr _uncle;

                while (_node->_parent->_color == _red)
                {
                    // Parent is right node
                    if (_S_is_right(_node->_parent))
                    {
                        _uncle = _node->_parent->_parent->_left;
                        // Red uncle case
                        if (_uncle->_color == _red)
                        {
                            _uncle->_color = _black;
                            _node->_parent->_color = _black;
                            _node->_parent->_parent->_color = _red;
                            _node = _node->_parent->_parent;
                        }
                        // Black uncle case
                        else
                        {
                            if (_S_is_left(_node))
                            {
                                _node = _node->_parent;
                                _right_rotate(_node);
                            }
                            _node->_parent->_color = _black;
                            _node->_parent->_parent->_color = _red;
                            _left_rotate(_node->_parent->_parent);
                        }
                    }
                    // Parent is left child
                    else
                    {
                        _uncle = _node->_parent->_parent->_right;
                        // Red uncle case
                        if (_uncle->_color == _red)
                        {
                            _uncle->_color = _black;
                            _node->_parent->_color = _black;
                            _node->_parent->_parent->_color = _red;
                            _node = _node->_parent->_parent;
                        }
                        else
                        {
                            if (_S_is_right(_node))
                            {
                                _node = _node->_parent;
                                _left_rotate(_node);
                            }
                            _node->_parent->_color = _black;
                            _node->_parent->_parent->_color = _red;
                            _right_rotate(_node->_parent->_parent);
                        }
                    }
                    if (_node == _root)
                        break;
                }
                _root->_color = _black;
            }

            void _clear(node_ptr _node)
            {
                if (_node == _leaf)
                    return ;
                _clear(_node->_left);
                _clear(_node->_right);
                _deallocate_node(_node);
            }

            /**
             *  @brief replace _x subtree with _y
             */
            void _transplant(node_ptr _x, node_ptr _y)
            {
                if (_x->_parent == NULL)
                    _root = _y;
                else if (_S_is_left(_x))
                    _x->_parent->_left = _y;
                else
                    _x->_parent->_right = _y;
                _y->_parent = _x->_parent;
            }

            /**
             *  @brief Delete given node and re-balance the tree
             */
            void _erase_node(node_ptr _z)
            {
                node_ptr _x, _y;

                _y = _z;
                _RbColor _y_old_color = _y->_color;
                if (_z->_left == _leaf)
                {
                    _x = _z->_right;
                    _transplant(_z, _z->_right);
                }
                else if (_z->_right == _leaf)
                {
                    _x = _z->_left;
                    _transplant(_z, _z->_left);
                }
                else
                {
                    _y = _z->_right->minimum();
                    _y_old_color = _y->_color;
                    _x = _y->_right;
                    if (_y->_parent == _z)
                    {
                        _x->_parent = _y;
                    }
                    else
                    {
                        _transplant(_y, _y->_right);
                        _y->_right = _z->_right;
                        _y->_right->_parent = _y;
                    }
                    _transplant(_z, _y);
                    _y->_left = _z->_left;
                    _y->_left->_parent = _y;
                    _y->_color = _z->_color;
                }
                _deallocate_node(_z);
                --_size;
                if (_y_old_color == _black)
                    _balance_delete_tree(_x);
            }

            void _balance_delete_tree(node_ptr _node)
            {
                // Sibling node
                node_ptr _s;

                while (_node != _root && _node->_color == _black)
                {
                    if (_S_is_left(_node))
                    {
                        _s = _node->_parent->_right;
                        if (_s->_color == _red)
                        {
                            _s->_color = _black;
                            _node->_parent->_color = _red;
                            _left_rotate(_node->_parent);
                            _s = _node->_parent->_right;
                        }
                        if (_s->_left->_color == _black && _s->_right->_color == _black)
                        {
                            _s->_color = _red;
                            _node = _node->_parent;
                        }
                        else
                        {
                            if (_s->_right->_color == _black)
                            {
                                _s->_left->_color = _black;
                                _s->_color = _red;
                                _right_rotate(_s);
                                _s = _node->_parent->_right;
                            }
                            _s->_color = _node->_parent->_color;
                            _node->_parent->_color = _black;
                            _s->_right->_color = _black;
                            _left_rotate(_node->_parent);
                            _node = _root;
                        }
                    }
                    else
                    {
                        _s = _node->_parent->_left;
                        if (_s->_color == _red)
                        {
                            _s->_color = _black;
                            _node->_parent->_color = _red;
                            _right_rotate(_node->_parent);
                            _s = _node->_parent->_left;
                        }
                        if (_s->_right->_color == _black && _s->_left->_color == _black)
                        {
                            _s->_color = _red;
                            _node = _node->_parent;
                        }
                        else
                        {
                            if (_s->_left->_color == _black)
                            {
                                _s->_right->_color = _black;
                                _s->_color = _red;
                                _left_rotate(_s);
                                _s = _node->_parent->_left;
                            }
                            _s->_color = _node->_parent->_color;
                            _node->_parent->_color = _black;
                            _s->_left->_color = _black;
                            _right_rotate(_node->_parent);
                            _node = _root;
                        }
                    }
                }
                _node->_color = _black;
            }

        public:
            /**
             *  @brief Default constructor
             */
            _RbTree(const key_compare& cmp = key_compare(),
                    const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _f_cmp(cmp), _size()
            { _leaf = _create_node(); _root = _leaf;}

            /**
             *  @brief Deconstructor
             */
            ~_RbTree()
            {
                clear();
                _deallocate_node(_leaf);
            }

            /**
             *  @brief Insert new node with @a val to red-black-tree with proper key position
             *
             *  @remark this function may result in re-balancing the tree
             */
            node_ptr
            insert(const value_type& _val)
            {
                // Ordinary Binary Insert
                node_ptr _node = _create_node(_val);
                ++_size;

                node_ptr _prev = NULL;
                node_ptr _cursor = _root;
                while (_cursor != _leaf)
                {
                    _prev = _cursor;
                    if (_f_cmp(_node->key(), _cursor->key()))
                        _cursor = _cursor->_left;
                    else
                        _cursor = _cursor->_right;
                }
                if (_prev == NULL)
                    _root = _node;
                else if (_f_cmp(_node->key(), _prev->key()))
                {
                    _prev->_left = _node;
                    _node->_parent = _prev;
                }
                else
                {
                    _prev->_right = _node;
                    _node->_parent = _prev;
                }

                // If new node is root node, color it black then nothing change
                if (_node->_parent == NULL)
                {
                    _node->_color = _black;
                    return _node;
                }
                // If parent is root nothing change
                if (_node->_parent->_parent == NULL)
                    return _node;

                // Else balance the tree
                _balance_insert_tree(_node);
                _update_leaf();
                return _node;
            }

            /**
             *  @brief Deleted selected node by given key
             */
            bool
            erase(key_type _key)
            {
                node_ptr _node = _search_tree(_root, _key);
                if (_node == _leaf)
                    return false;
                _erase_node(_node);
                _update_leaf();
                return true;
            }

            /**
             *  @brief Deleted node at selected position
             */
            void
            erase(node_ptr _node)
            {
                if (_node == _leaf)
                    return ;
                _erase_node(_node);
                _update_leaf();
            }

            /**
             *  @brief Getter for root
             */
            node_ptr
            root(void)
            { return _root; }

            /**
             *  @brief const Getter for root
             */
            const_node_ptr
            root(void) const
            { return _root; }

            /**
             *  @brief swap content with other tree
             */
            void swap(_RbTree& x)
            {
                node_ptr tmp;

                tmp = _root;
                _root = x._root;
                x._root = tmp;

                tmp = _leaf;
                _leaf = x._leaf;
                x._leaf = tmp;

                size_t tmp_size;

                tmp_size = _size;
                _size = x._size;
                x._size = tmp_size;

                key_compare tmp_cmp;

                tmp_cmp = _f_cmp;
                _f_cmp = x._f_cmp;
                x._f_cmp = tmp_cmp;
            }

            node_ptr
            leaf(void)
            { return _leaf; }

            const_node_ptr
            leaf(void) const
            { return _leaf; }

            /**
             *  @brief Getter for size
             */
            size_type
            size(void) const
            { return _size; }

            /**
             *  @brief clear all data in tree and deallocate all memory
             *
             *  @remark this won't clear _leaf node
             */
            void
            clear(void)
            {
                _clear(_root);
                _size = 0;
                _root = _leaf;
                _leaf->_left = NULL;
                _leaf->_right = NULL;
            }

            /**
             *  @brief Search for key in tree
             */
            node_ptr
            search(key_type _key)
            { return _search_tree(_root, _key); }

            const_node_ptr
            search(key_type _key) const
            { return _search_tree(_root, _key); }

    }; /* class _RbTree */
} /* namespace ft */

#endif /* __RED_BLACK_TREE_HPP__ */
