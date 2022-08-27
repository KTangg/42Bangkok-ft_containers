#ifndef __TYPE_TRAITS_HPP__
# define __TYPE_TRAITS_HPP__

namespace ft
{

    /**
     *  @brief Define member typedef only if bool constant is true
     */
    template <bool, typename T = void>
        struct enable_if {};

    template <typename T>
        struct enable_if<true, T>
        { typedef T type; };

    /**
     *  @brief Define integral_constant true false type
     */
    template <typename T, T v>
        struct integral_constant
        {
            static const T              value = v;
            typedef T                       value_type;
            typedef integral_constant<T, v> type;
        };

    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

    /**
     *  @brief Check whether given type is integral or not
     */


    template<typename>
        struct _is_integral : public false_type {};

    template<>
        struct _is_integral<bool> : public true_type {};

    template<>
        struct _is_integral<char> : public true_type {};

    template<>
        struct _is_integral<signed char> : public true_type {};

    template<>
        struct _is_integral<unsigned char> : public true_type {};

    template<>
        struct _is_integral<wchar_t> : public true_type {};

    template<>
        struct _is_integral<short> : public true_type {};

    template<>
        struct _is_integral<unsigned short> : public true_type {};

    template<>
        struct _is_integral<int> : public true_type {};

    template<>
        struct _is_integral<unsigned int> : public true_type {};

    template<>
        struct _is_integral<long> : public true_type {};

    template<>
        struct _is_integral<unsigned long> : public true_type {};

    template<>
        struct _is_integral<long long> : public true_type {};

    template<>
        struct _is_integral<unsigned long long> : public true_type {};

    /// is_integral
    template <typename T>
        struct is_integral
        : public _is_integral<T>::type {};
}

#endif /* __TYPE_TRAITS_HPP__ */
