#pragma once

#include <cmath>

#include "../vec4.hpp"

namespace ayan::math {

// ----- ----- ---- Constructors ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>::Vec() noexcept : data{NumT(0), NumT(0), NumT(0), NumT(0)} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>::Vec(NumT x, NumT y, NumT z, NumT w) noexcept
  : data{x, y, z, w} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<4, NumT>::Vec(std::initializer_list<U> init_list) noexcept : Vec() {
  if (init_list.size() != 4) return;
  auto it = init_list.begin();
  data[0] = *it++;
  data[1] = *it++;
  data[2] = *it++;
  data[3] = *it;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<4, NumT>::Vec(const Vec4<U>& oth) noexcept
  : data{oth.x(), oth.y(), oth.z(), oth.w()} {}

// ----- ----- ---- Static member funcs ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::UnitX() noexcept {
  return Vec4<NumT>(NumT(1), NumT(0), NumT(0), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::UnitY() noexcept {
  return Vec4<NumT>(NumT(0), NumT(1), NumT(0), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::UnitZ() noexcept {
  return Vec4<NumT>(NumT(0), NumT(0), NumT(1), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::UnitW() noexcept {
  return Vec4<NumT>(NumT(0), NumT(0), NumT(0), NumT(1));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::Zero() noexcept {
  return Vec4<NumT>(NumT(0), NumT(0), NumT(0), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec4<NumT> Vec<4, NumT>::One() noexcept {
  return Vec4<NumT>(NumT(1), NumT(1), NumT(1), NumT(1));
}

// ----- ----- ---- Element access ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<4, NumT>::x() noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<4, NumT>::y() noexcept { return data[1]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<4, NumT>::z() noexcept { return data[2]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<4, NumT>::w() noexcept { return data[3]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<4, NumT>::x() const noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<4, NumT>::y() const noexcept { return data[1]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<4, NumT>::z() const noexcept { return data[2]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<4, NumT>::w() const noexcept { return data[3]; }

// ----- ----- ---- Operators ----- ----- ----
// unary:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT> Vec<4, NumT>::operator-() const noexcept {
  return Vec(-x(), -y(), -z(), -w());
}

// with assignment:
template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator+=(const Vec4<U>& oth) noexcept {
  data[0] += oth.x();
  data[1] += oth.y();
  data[2] += oth.z();
  data[3] += oth.w();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator-=(const Vec4<NumT>& oth) noexcept {
  data[0] -= oth.x();
  data[1] -= oth.y();
  data[2] -= oth.z();
  data[3] -= oth.w();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator*=(NumT scalar) noexcept {
  data[0] *= scalar;
  data[1] *= scalar;
  data[2] *= scalar;
  data[3] *= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator/=(NumT scalar) noexcept {
  data[0] /= scalar;
  data[1] /= scalar;
  data[2] /= scalar;
  data[3] /= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator*=(const Vec4<NumT>& oth) noexcept {
  data[0] *= oth.x();
  data[1] *= oth.y();
  data[2] *= oth.z();
  data[3] *= oth.w();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<4, NumT>& Vec<4, NumT>::operator/=(const Vec4<NumT>& oth) noexcept {
  data[0] /= oth.x();
  data[1] /= oth.y();
  data[2] /= oth.z();
  data[3] /= oth.w();
  return *this;
}

// comparing:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<4, NumT>::operator==(const Vec<4, NumT>& oth) const noexcept {
  return (data[0] == oth.data[0] &&
          data[1] == oth.data[1] &&
          data[2] == oth.data[2] &&
          data[3] == oth.data[3]);
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<4, NumT>::operator!=(const Vec<4, NumT>& oth) const noexcept {
  return !(*this == oth);
}

// ----- ----- ---- Linear Algebra Operations ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<4, NumT>::dot(const Vec4<NumT>& oth) const noexcept {
  return x() * oth.x() + y() * oth.y() + z() * oth.z() + w() * oth.w();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<4, NumT>::length() const {
  return std::sqrt(length_squared());
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<4, NumT>::length_squared() const noexcept {
  return dot(*this);
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
Vec4<NumT> Vec<4, NumT>::normalize() const {
  NumT len = length();
  if (len > NumT{0}) {
    return *this / len;
  }
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<4, NumT>::distance(const Vec4<NumT>& oth) const {
  return (*this - oth).length();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<4, NumT>::distance_squared(const Vec4<NumT>& oth) const noexcept {
  return (*this - oth).length_squared();
}

// ----- ----- ---- Utility functional methods ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <typename Func> requires std::invocable<Func, NumT>
constexpr auto Vec<4, NumT>::map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
  -> Vec4<std::invoke_result_t<Func, NumT>>
{
  return Vec4<std::invoke_result_t<Func, NumT>>(
    func(x()), func(y()), func(z()), func(w())
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr NumT& Vec<4, NumT>::get() noexcept {
  static_assert(Index < 4, "Index out of bounds for Vec4");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
  else if constexpr (Index == 2) return z();
  else if constexpr (Index == 3) return w();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr const NumT& Vec<4, NumT>::get() const noexcept {
  static_assert(Index < 4, "Index out of bounds for Vec4");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
  else if constexpr (Index == 2) return z();
  else if constexpr (Index == 3) return w();
}

// ----- ----- ---- Binary operators ----- ----- ----
template<typename NumT>
constexpr Vec4<NumT> operator+(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept {
  return Vec4<NumT>{a.x() + b.x(), a.y() + b.y(), a.z() + b.z(), a.w() + b.w()};
}

template<typename NumT>
constexpr Vec4<NumT> operator-(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept {
  return Vec4<NumT>{a.x() - b.x(), a.y() - b.y(), a.z() - b.z(), a.w() - b.w()};
}

template<typename NumT>
constexpr Vec4<NumT> operator*(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept {
  return Vec4<NumT>{a.x() * b.x(), a.y() * b.y(), a.z() * b.z(), a.w() * b.w()};
}

template<typename NumT>
constexpr Vec4<NumT> operator/(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept {
  return Vec4<NumT>{a.x() / b.x(), a.y() / b.y(), a.z() / b.z(), a.w() / b.w()};
}

template<typename NumT>
constexpr Vec4<NumT> operator*(const Vec4<NumT>& vec, NumT scalar) noexcept {
  return Vec4<NumT>{vec.x() * scalar, vec.y() * scalar, vec.z() * scalar, vec.w() * scalar};
}

template<typename NumT>
constexpr Vec4<NumT> operator*(NumT scalar, const Vec4<NumT>& vec) noexcept {
  return vec * scalar;
}

template<typename NumT>
constexpr Vec4<NumT> operator/(const Vec4<NumT>& vec, NumT scalar) noexcept {
  return Vec4<NumT>{vec.x() / scalar, vec.y() / scalar, vec.z() / scalar, vec.w() / scalar};
}

} // namespace ayan::math
