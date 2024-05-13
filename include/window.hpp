#ifndef WINDOW_FUNCTION_HPP
#define WINDOW_FUNCTION_HPP

#include "data_types.h"

namespace DSP
{
    inline namespace windowing
    {
        namespace internal
        {
            template <WindowType Type>
            struct window_builder
            {
            };

            template <>
            struct window_builder<WindowType::Rectangular>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Hanning>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Bartlett>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Hamming>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Blackman>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Welch>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::BlackmanNuttall>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::BlackmanHarris>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::FlatTop>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

            template <>
            struct window_builder<WindowType::Triangular>
            {
                template <typename OutputIt>
                constexpr void operator()(OutputIt first, OutputIt last)
                {
                }
            };

        };

        template <WindowType Type, typename OutputIt>
        constexpr void make_window(OutputIt first, OutputIt last)
        {
            return internal::window_builder<Type>{}(first, last);
        }
    };
};

#endif // WINDOW_FUNCTION_HPP