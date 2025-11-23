#pragma once

#include <cstddef>
#include <array>

#include "../detail/validate.hpp"

namespace ayan::math {

template<size_t Len, typename NumT = double>
class Vec;

// Temporary solution:
template<typename T, size_t Size>
using CacheFriendlyArr = std::array<T, Size>;

// 2-dimensional vector:
template<typename NumT> requires (detail::ValidNumType<NumT>)
using Vec2 = Vec<2, NumT>;

// 3-dimensional vector:
template<typename NumT> requires (detail::ValidNumType<NumT>)
using Vec3 = Vec<3, NumT>;

// 4-dimensional vector:
template<typename NumT> requires (detail::ValidNumType<NumT>)
using Vec4 = Vec<4, NumT>;

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
using Vec4i = Vec4<int>;

} // namespace ayan::math
