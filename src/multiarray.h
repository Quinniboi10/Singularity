#pragma once

#include <array>

namespace multiarray_internal {
    template <typename T, unsigned kN, unsigned... kNs>
    struct MultiArrayImpl {
        using Type = std::array<typename MultiArrayImpl<T, kNs...>::Type, kN>;
    };

    template <typename T, unsigned kN>
    struct MultiArrayImpl<T, kN> {
        using Type = std::array<T, kN>;
    };
}

template <typename T, unsigned... kNs>
using MultiArray = typename multiarray_internal::MultiArrayImpl<T, kNs...>::Type;
