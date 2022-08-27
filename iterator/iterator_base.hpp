#ifndef __ITERATOR_BASE_HPP__
# define __ITERATOR_BASE_HPP__

# include <memory>
# include <cstddef>

namespace ft
{
    /**
     *  @brief  Common %iterator class.
     *
     *  This class does nothing but define nested typedefs.  %Iterator classes
     *  can inherit from this class to save some work.  The typedefs are then
     *  used in specializations and overloading.
     *
     */
    template <typename _Category,  typename _Tp, typename _Distance = ptrdiff_t,
              typename _Pointer = _Tp*, typename _Reference = _Tp&>
        struct iterator
        {
            /// iterator_tags
            typedef _Category   iterator_category;
            /// Type point to by iterator
            typedef _Tp         value_type;
            /// Type that represent distance between iterators
            typedef _Distance   difference_type;
            /// Type represents a pointer-to-value_type
            typedef _Pointer    pointer;
            /// Type represents a reference-to-value_type
            typedef _Reference  reference;
        };
    /**
     *  @defgroup  Tags for iterators
     *
     *  This classes does nothing but define type of iterator
     *  It used to distinguish different iterators based on what they are
     *
     */
    /// Input iterators
    struct input_iterator_tag {};

    /// Output iterators
    struct output_iterator_tah {};

    /// Forward iterators that also include input iterator operations
    struct forward_iterator_tag : public input_iterator_tag {};

    /// Bidirectional iterators also include forward iterator operations
    struct bidirectional_iterator_tag : public forward_iterator_tag {};

    /// Random-access iterator also include bidirectional iterator operations
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    /**
     *  @brief  Traits class for iterators.
     *
     *  This class does nothing but define nested typedefs.  The general
     *  version simply @a forwards the nested typedefs from the Iterator
     *  argument.  Specialized versions for pointers and pointers-to-const
     *  provide tighter, more correct semantics.
     */
    template <typename _Iterator>
        struct iterator_traits
        {
            typedef typename _Iterator::iterator_category   iterator_category;
            typedef typename _Iterator::value_type          value_type;
            typedef typename _Iterator::difference_type     difference_type;
            typedef typename _Iterator::pointer             pointer;
            typedef typename _Iterator::reference           reference;
        };

    /// Partial template for pointer
    template <typename _Tp>
        struct iterator_traits<_Tp*>
        {
            typedef random_access_iterator_tag      iterator_category;
            typedef _Tp                             value_type;
            typedef ptrdiff_t                       difference_type;
            typedef _Tp*                            pointer;
            typedef _Tp&                            reference;
        };

    /// Partial template for const pointer
    template <typename _Tp>
        struct iterator_traits<const _Tp*>
        {
            typedef random_access_iterator_tag      iterator_category;
            typedef _Tp                             value_type;
            typedef ptrdiff_t                       difference_type;
            typedef const _Tp*                      pointer;
            typedef const _Tp&                      reference;
        };
}

#endif /* __ITERATOR_BASE_HPP__ */
