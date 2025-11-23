#pragma once

#include <cmath>

#include "../vec2.hpp"

namespace ayan::math {

// ----- ----- ---- Constructors ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>::Vec() noexcept : data {NumT(0), NumT(0)} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>::Vec(NumT x, NumT y) noexcept : data {x, y} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<2, NumT>::Vec(std::initializer_list<U> init_list) noexcept : Vec() {
  if (init_list.size() != 2) return;
  auto it = init_list.begin();
  data[0] = *it++;
  data[1] = *it++;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<2, NumT>::Vec(const Vec2<U>& oth) noexcept : data {oth.x(), oth.y()} {}

// ----- ----- ---- Static member funcs ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::UnitX() noexcept {
  return Vec{NumT(1), NumT(0)};
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::UnitY() noexcept {
  return Vec{NumT(0), NumT(1)};
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::Zero() noexcept {
  return Vec{NumT(0), NumT(0)};
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::One() noexcept {
  return Vec{NumT(1), NumT(1)};
}

// ----- ----- ---- Element access ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<2, NumT>::x() noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<2, NumT>::y() noexcept { return data[1]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<2, NumT>::x() const noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<2, NumT>::y() const noexcept { return data[1]; }

// ----- ----- ---- Operators ----- ----- ----
// unary:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::operator-() const noexcept {
  return Vec(-x(), -y());
}

// with assignment:
template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator+=(const Vec2<U>& oth) noexcept {
  data[0] += oth.data[0];
  data[1] += oth.data[1];
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator-=(const Vec<2, NumT>& oth) noexcept {
  data[0] -= oth.data[0];
  data[1] -= oth.data[1];
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator*=(NumT scalar) noexcept {
  data[0] *= scalar;
  data[1] *= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator/=(NumT scalar) noexcept {
  data[0] /= scalar;
  data[1] /= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator*=(const Vec<2, NumT>& oth) noexcept {
  data[0] *= oth.data[0];
  data[1] *= oth.data[1];
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT>& Vec<2, NumT>::operator/=(const Vec<2, NumT>& oth) noexcept {
  data[0] /= oth.data[0];
  data[1] /= oth.data[1];
  return *this;
}

// comparing:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<2, NumT>::operator==(const Vec<2, NumT>& oth) const noexcept {
  return (x() == oth.x() && y() == oth.y());
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<2, NumT>::operator!=(const Vec<2, NumT>& oth) const noexcept {
  return !(*this == oth);
}

// ----- ----- ---- Linear Algebra Operations ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<2, NumT>::dot(const Vec<2, NumT>& oth) const noexcept {
  return x() * oth.x() + y() * oth.y();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<2, NumT>::length() const noexcept {
  return std::sqrt(x() * x() + y() * y());
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<2, NumT>::lengthSquared() const noexcept {
  return x() * x() + y() * y();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<2, NumT> Vec<2, NumT>::perpendicular() const noexcept {
  return Vec{-y(), x()};
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
Vec<2, NumT> Vec<2, NumT>::normalize() const noexcept {
  NumT len = length();
  if (len > NumT{0}) {
    return *this / len;
  }
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<2, NumT>::distance(const Vec<2, NumT>& oth) const noexcept {
  return (*this - oth).length();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<2, NumT>::distance_squared(const Vec<2, NumT>& oth) const noexcept {
  return (*this - oth).length_squared();
}

// ----- ----- ---- Utility functional methods ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <typename Func> requires std::invocable<Func, NumT>
constexpr auto Vec<2, NumT>::map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
  -> Vec2<std::invoke_result<Func, NumT>>
{
  return Vec2<std::invoke_result<Func, NumT>>(
    func(x()), func(y())
  );
}

// ----- ----- ---- Structure Binding Support ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr NumT& Vec<2, NumT>::get() noexcept {
  static_assert(Index < 2, "Index out of bounds for Vec2");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr const NumT& Vec<2, NumT>::get() const noexcept {
  static_assert(Index < 2, "Index out of bounds for Vec2");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
}

// ----- ----- ---- Binary operators ----- ----- ----
template<typename NumT>
constexpr Vec2<NumT> operator+(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept {
  return Vec2<NumT>{a.x() + b.x(), a.y() + b.y()};
}

template<typename NumT>
constexpr Vec2<NumT> operator-(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept {
  return Vec2<NumT>{a.x() - b.x(), a.y() - b.y()};
}

template<typename NumT>
constexpr Vec2<NumT> operator*(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept {
  return Vec2<NumT>{a.x() * b.x(), a.y() * b.y()};
}

template<typename NumT>
constexpr Vec2<NumT> operator/(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept {
  return Vec2<NumT>{a.x() / b.x(), a.y() / b.y()};
}

template<typename NumT>
constexpr Vec2<NumT> operator*(const Vec2<NumT>& vec, NumT scalar) noexcept {
  return Vec2<NumT>{vec.x() * scalar, vec.y() * scalar};
}

template<typename NumT>
constexpr Vec2<NumT> operator*(NumT scalar, const Vec2<NumT>& vec) noexcept {
  return Vec2<NumT>{vec.x() * scalar, vec.y() * scalar};
}

template<typename NumT>
constexpr Vec2<NumT> operator/(const Vec2<NumT>& vec, NumT scalar) noexcept {
  return Vec2<NumT>{vec.x() / scalar, vec.y() / scalar};
}

} // namespace ayan::math
