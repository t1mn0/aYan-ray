#pragma once

#include "../mat4.hpp"

namespace ayan::math {

// ----- ----- ---- Constructors ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat<4, 4, NumT>::Mat() noexcept : columns {
  Vec4<NumT>::UnitX(),
  Vec4<NumT>::UnitY(),
  Vec4<NumT>::UnitZ(),
  Vec4<NumT>::UnitW()
} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat<4, 4, NumT> Mat<4, 4, NumT>::Identity() noexcept {
  return Mat();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat<4, 4, NumT>::Mat(
  const Vec4<NumT>& col0, const Vec4<NumT>& col1,
  const Vec4<NumT>& col2, const Vec4<NumT>& col3) noexcept
: columns{col0, col1, col2, col3} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Mat<4, 4, NumT>::Mat(std::initializer_list<U> init_list) noexcept : Mat() {
  if (init_list.size() != 16) return;
  auto it = init_list.begin();

  columns[0].x() = *it; ++it;
  columns[1].x() = *it; ++it;
  columns[2].x() = *it; ++it;
  columns[3].x() = *it; ++it;

  columns[0].y() = *it; ++it;
  columns[1].y() = *it; ++it;
  columns[2].y() = *it; ++it;
  columns[3].y() = *it; ++it;

  columns[0].z() = *it; ++it;
  columns[1].z() = *it; ++it;
  columns[2].z() = *it; ++it;
  columns[3].z() = *it; ++it;

  columns[0].w() = *it; ++it;
  columns[1].w() = *it; ++it;
  columns[2].w() = *it; ++it;
  columns[3].w() = *it; ++it;
}

// ----- ----- ---- Element Access ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t index> requires (index < 4)
constexpr Vec4<NumT> Mat<4, 4, NumT>::row() noexcept {
  Vec4<NumT> vec;
  switch (index){
    case 0: {
      vec.x() = columns[0].x();
      vec.y() = columns[1].x();
      vec.z() = columns[2].x();
      vec.w() = columns[3].x();
      break;
    }
    case 1: {
      vec.x() = columns[0].y();
      vec.y() = columns[1].y();
      vec.z() = columns[2].y();
      vec.w() = columns[3].y();
      break;
    }
    case 2: {
      vec.x() = columns[0].z();
      vec.y() = columns[1].z();
      vec.z() = columns[2].z();
      vec.w() = columns[3].z();
      break;
    }
    case 3: {
      vec.x() = columns[0].w();
      vec.y() = columns[1].w();
      vec.z() = columns[2].w();
      vec.w() = columns[3].w();
      break;
    }
  }

  return vec;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t index> requires (index < 4)
constexpr Vec4<NumT>& Mat<4, 4, NumT>::col() noexcept{
  return columns[index];
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t index> requires (index < 4)
constexpr const Vec4<NumT>& Mat<4, 4, NumT>::col() const noexcept {
  return columns[index];
}

// ----- ----- ---- Operators ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::operator+(const Mat4<NumT>& oth) const noexcept {
  return Mat(
    columns[0] + oth.columns[0],
    columns[1] + oth.columns[1],
    columns[2] + oth.columns[2],
    columns[3] + oth.columns[3]
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::operator-(const Mat4<NumT>& oth) const noexcept {
  return Mat(
    columns[0] - oth.columns[0],
    columns[1] - oth.columns[1],
    columns[2] - oth.columns[2],
    columns[3] - oth.columns[3]
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::operator*(NumT scalar) const noexcept {
  return Mat(
    columns[0] * scalar,
    columns[1] * scalar,
    columns[2] * scalar,
    columns[3] * scalar
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::operator/(NumT scalar) const noexcept {
  return Mat(
    columns[0] / scalar,
    columns[1] / scalar,
    columns[2] / scalar,
    columns[3] / scalar
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT>& Mat<4, 4, NumT>::operator+=(const Mat4<NumT>& oth) noexcept {
  columns[0] += oth.columns[0];
  columns[1] += oth.columns[1];
  columns[2] += oth.columns[2];
  columns[3] += oth.columns[3];
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT>& Mat<4, 4, NumT>::operator*=(const Mat4<NumT>& oth) noexcept {
  *this = *this * oth;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::operator==(const Mat4<NumT>& oth) const noexcept {
  return columns[0] == oth.columns[0] &&
  columns[1] == oth.columns[1] &&
  columns[2] == oth.columns[2] &&
  columns[3] == oth.columns[3];
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::operator!=(const Mat4<NumT>& oth) const noexcept {
  return !(*this == oth);
}

// ----- ----- ---- Linear Algebra Operations ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::operator*(const Mat4<NumT>& oth) const noexcept {
  Mat result;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      result(i, j) = row(i).dot(oth.col(j));
    }
  }
  return result;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Mat<4, 4, NumT>::determinant() const noexcept {
  NumT const& m00 = columns[0][0], m01 = columns[1][0], m02 = columns[2][0], m03 = columns[3][0];
  NumT const& m10 = columns[0][1], m11 = columns[1][1], m12 = columns[2][1], m13 = columns[3][1];
  NumT const& m20 = columns[0][2], m21 = columns[1][2], m22 = columns[2][2], m23 = columns[3][2];
  NumT const& m30 = columns[0][3], m31 = columns[1][3], m32 = columns[2][3], m33 = columns[3][3];

  return
    m30 * m21 * m12 * m03 - m20 * m31 * m12 * m03 - m30 * m11 * m22 * m03 +
    m10 * m31 * m22 * m03 + m20 * m11 * m32 * m03 - m10 * m21 * m32 * m03 -
    m30 * m21 * m02 * m13 + m20 * m31 * m02 * m13 + m30 * m01 * m22 * m13 -
    m00 * m31 * m22 * m13 - m20 * m01 * m32 * m13 + m00 * m21 * m32 * m13 +
    m30 * m11 * m02 * m23 - m10 * m31 * m02 * m23 - m30 * m01 * m12 * m23 +
    m00 * m31 * m12 * m23 + m10 * m01 * m32 * m23 - m00 * m11 * m32 * m23 -
    m20 * m11 * m02 * m33 + m10 * m21 * m02 * m33 + m20 * m01 * m12 * m33 -
    m00 * m21 * m12 * m33 - m10 * m01 * m22 * m33 + m00 * m11 * m22 * m33;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Mat4<NumT> Mat<4, 4, NumT>::transpose() const noexcept {
  auto m = Mat4<NumT>(
    row<0>(), row<1>(), row<2>(), row<3>()
  );
  *this = m;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Mat<4, 4, NumT>::trace() const noexcept {
  // TODO: Implement trace calculation
  return NumT{};
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::is_identity() const noexcept {
  // TODO: Implement identity check
  return false;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::is_diagonal() const noexcept {
  // TODO: Implement diagonal check
  return false;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::is_symmetric() const noexcept {
  // TODO: Implement symmetric check
  return false;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Mat<4, 4, NumT>::is_orthogonal() const noexcept {
  // TODO: Implement orthogonal check
  return false;
}

// ----- ----- ---- Utility functional methods ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <typename Func> requires std::invocable<Func, NumT>
constexpr auto Mat<4, 4, NumT>::map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
  -> Mat4<std::invoke_result_t<Func, NumT>>
{
  return Mat<4, 4, std::invoke_result_t<Func, NumT>>(
    columns[0].map(func),
    columns[1].map(func),
    columns[2].map(func),
    columns[3].map(func)
  );
}

// ----- ----- ---- External Operators ---- ----- ----
template<typename NumT>
constexpr Mat4<NumT> operator*(NumT scalar, const Mat4<NumT>& mat) noexcept {
  return mat * scalar;
}

} // namespace ayan::math
