/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spoolpra <spoolpra@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:47:26 by spoolpra          #+#    #+#             */
/*   Updated: 2022/09/06 19:13:53 by spoolpra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

namespace ft
{
    template <class T1, class T2>
    struct pair
    {
        typedef T1  first_type;
        typedef T2  second_type;

        first_type  first;
        second_type second;

        /**
         *  @brief Default constructor set @a first, @a second to default
         */
        pair()
        : first(), second() { }

        /**
         *  @brief Copy constuctor
         */
        template <class U, class V>
        pair(const pair<U, V>& src)
        : first(src.first), second(src.second) { }

        /**
         *  @brief Initialize constuctor
         */
        pair(const first_type& a, const second_type& b)
        : first(a), second(b) { }

        /**
         *  @brief Assignment operator
         */
        pair& operator=(const pair& rhs)
        {
            first = rhs.first;
            second = rhs.second;
            return *this;
        }

        /**
         *  @brief swap first, second with other pair
         */
        void swap(pair& other)
        {
            first_type tmp_first = first;
            second_type tmp_second = second;

            first = other.first;
            second = other.second;
            other.first = tmp_first;
            other.second = tmp_second;
        }

    }; /* struct pair */

    template <class T1, class T2>
    inline bool
    operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return lhs.first == rhs.first && lhs.second == rhs.second; }

    template <class T1, class T2>
    inline bool
    operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return !(lhs == rhs); }

    /**
     *  @brief If first of left less than right return true
     *  else if first of left equal right return left second < right second
     *  else false
     */
    template <class T1, class T2>
    inline bool
    operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return lhs.first < rhs.first
        || (!(rhs.first < lhs.first) && lhs.second < rhs.second); }

    template <class T1, class T2>
    inline bool
    operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return !(lhs < rhs); }

    template <class T1, class T2>
    inline bool
    operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return rhs < lhs; }

    template <class T1, class T2>
    inline bool
    operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return !(rhs < lhs); }

    /**
     *  @brief swap content between two pair
     */
    template <class T1, class T2>
    void swap(pair<T1, T2>& x, pair<T1, T2>& y)
    { x.swap(y); }

    /**
     *  @brief Create new pair
     */
    template <class T1, class T2>
    inline pair<T1, T2> make_pair(T1 x, T2 y)
    { return pair<T1, T2>(x, y); }

} /* namespace ft */

#endif /* __UTILITY_HPP__ */
