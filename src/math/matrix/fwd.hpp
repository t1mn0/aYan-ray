#pragma once

#include <cstddef>

namespace ayan::math {

template<size_t Rows, size_t Cols, typename NumT = double>
class Mat;

template<typename T> using Mat2 = Mat<2, 2, T>;
template<typename T> using Mat3 = Mat<3, 3, T>;
template<typename T> using Mat4 = Mat<4, 4, T>;

using Mat2f = Mat2<float>;
using Mat3f = Mat3<float>;
using Mat4f = Mat4<float>;

using Mat2d = Mat2<double>;
using Mat3d = Mat3<double>;
using Mat4d = Mat4<double>;

using Mat2i = Mat2<int>;
using Mat3i = Mat3<int>;
using Mat4i = Mat4<int>;

} // namespace ayan::math
