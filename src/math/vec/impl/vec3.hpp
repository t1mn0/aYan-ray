#pragma once

#include <cmath>

#include "../vec3.hpp"

namespace ayan::math {

// ----- ----- ---- Constructors ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>::Vec() noexcept : data{NumT(0), NumT(0), NumT(0)} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>::Vec(NumT x, NumT y, NumT z) noexcept : data{x, y, z} {}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<3, NumT>::Vec(std::initializer_list<U> init_list) noexcept : Vec() {
  if (init_list.size() != 3) return;
  auto it = init_list.begin();
  data[0] = *it++;
  data[1] = *it++;
  data[2] = *it;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<3, NumT>::Vec(const Vec3<U>& oth) noexcept
  : data{oth.x(), oth.y(), oth.z()} {}

// ----- ----- ---- Static member funcs ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::UnitX() noexcept {
  return Vec(NumT(1), NumT(0), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::UnitY() noexcept {
  return Vec(NumT(0), NumT(1), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::UnitZ() noexcept {
  return Vec(NumT(0), NumT(0), NumT(1));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::Zero() noexcept {
  return Vec(NumT(0), NumT(0), NumT(0));
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::One() noexcept {
  return Vec(NumT(1), NumT(1), NumT(1));
}

// ----- ----- ---- Element access ---- ----- -----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<3, NumT>::x() noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<3, NumT>::y() noexcept { return data[1]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT& Vec<3, NumT>::z() noexcept { return data[2]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<3, NumT>::x() const noexcept { return data[0]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<3, NumT>::y() const noexcept { return data[1]; }

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr const NumT& Vec<3, NumT>::z() const noexcept { return data[2]; }

// ----- ----- ---- Operators ----- ----- ----
// unary:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::operator-() const noexcept {
  return Vec(-x(), -y(), -z());
}

// with assignment:
template<typename NumT> requires (detail::ValidNumType<NumT>)
template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator+=(const Vec3<U>& oth) noexcept {
  data[0] += oth.x();
  data[1] += oth.y();
  data[2] += oth.z();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator-=(const Vec<3, NumT>& oth) noexcept {
  data[0] -= oth.x();
  data[1] -= oth.y();
  data[2] -= oth.z();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator*=(NumT scalar) noexcept {
  data[0] *= scalar;
  data[1] *= scalar;
  data[2] *= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator/=(NumT scalar) noexcept {
  data[0] /= scalar;
  data[1] /= scalar;
  data[2] /= scalar;
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator*=(const Vec<3, NumT>& oth) noexcept {
  data[0] *= oth.x();
  data[1] *= oth.y();
  data[2] *= oth.z();
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT>& Vec<3, NumT>::operator/=(const Vec<3, NumT>& oth) noexcept {
  data[0] /= oth.x();
  data[1] /= oth.y();
  data[2] /= oth.z();
  return *this;
}

// comparing:
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<3, NumT>::operator==(const Vec<3, NumT>& oth) const noexcept {
  return (
    data[0] == oth.data[0] &&
    data[1] == oth.data[1] &&
    data[2] == oth.data[2]);
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr bool Vec<3, NumT>::operator!=(const Vec<3, NumT>& oth) const noexcept {
  return !(*this == oth);
}

// ----- ----- ---- Linear Algebra Operations ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<3, NumT>::dot(const Vec<3, NumT>& oth) const noexcept {
  return x() * oth.x() + y() * oth.y() + z() * oth.z();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<3, NumT>::length() const noexcept {
  return std::sqrt(x() * x() + y() * y() + z() * z());
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<3, NumT>::length_squared() const noexcept {
  return x() * x() + y() * y() + z() * z();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
Vec<3, NumT> Vec<3, NumT>::normalize() const noexcept {
  NumT len = length();
  if (len > NumT{0}) {
    return *this / len;
  }
  return *this;
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
NumT Vec<3, NumT>::distance(const Vec<3, NumT>& oth) const noexcept {
  return (*this - oth).length();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<3, NumT>::distance_squared(const Vec<3, NumT>& oth) const noexcept {
  return (*this - oth).length_squared();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr Vec<3, NumT> Vec<3, NumT>::cross(const Vec<3, NumT>& oth) const noexcept {
  return Vec(
    y() * oth.z() - z() * oth.y(),
    z() * oth.x() - x() * oth.z(),
    x() * oth.y() - y() * oth.x()
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
constexpr NumT Vec<3, NumT>::triple(const Vec<3, NumT>& b, const Vec<3, NumT>& c) const noexcept {
  return dot(cross(b), c);
}

// ----- ----- ---- Utility functional methods ----- ----- ----
template<typename NumT> requires (detail::ValidNumType<NumT>)
template <typename Func> requires std::invocable<Func, NumT>
constexpr auto Vec<3, NumT>::map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
  -> Vec3<std::invoke_result_t<Func, NumT>>
{
  return Vec3<std::invoke_result_t<Func, NumT>>(
    func(x()), func(y()), func(z())
  );
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr NumT& Vec<3, NumT>::get() noexcept {
  static_assert(Index < 3, "Index out of bounds for Vec3");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
  else if constexpr (Index == 2) return z();
}

template<typename NumT> requires (detail::ValidNumType<NumT>)
template <size_t Index>
constexpr const NumT& Vec<3, NumT>::get() const noexcept {
  static_assert(Index < 3, "Index out of bounds for Vec3");
  if constexpr (Index == 0) return x();
  else if constexpr (Index == 1) return y();
  else if constexpr (Index == 2) return z();
}

// ----- ----- ---- Binary operators ----- ----- ----
template<typename NumT>
constexpr Vec3<NumT> operator+(const Vec3<NumT>& a, const Vec3<NumT>& b) noexcept {
  return Vec3<NumT>{a.x() + b.x(), a.y() + b.y(), a.z() + b.z()};
}

template<typename NumT>
constexpr Vec3<NumT> operator-(const Vec3<NumT>& a, const Vec3<NumT>& b) noexcept {
  return Vec3<NumT>{a.x() - b.x(), a.y() - b.y(), a.z() - b.z()};
}

template<typename NumT>
constexpr Vec3<NumT> operator*(const Vec3<NumT>& a, const Vec3<NumT>& b) noexcept {
  return Vec3<NumT>{a.x() * b.x(), a.y() * b.y(), a.z() * b.z()};
}

template<typename NumT>
constexpr Vec3<NumT> operator/(const Vec3<NumT>& a, const Vec3<NumT>& b) noexcept {
  return Vec3<NumT>{a.x() / b.x(), a.y() / b.y(), a.z() / b.z()};
}

template<typename NumT>
constexpr Vec3<NumT> operator*(const Vec3<NumT>& vec, NumT scalar) noexcept {
  return Vec3<NumT>{vec.x() * scalar, vec.y() * scalar, vec.z() * scalar};
}

template<typename NumT>
constexpr Vec3<NumT> operator*(NumT scalar, const Vec3<NumT>& vec) noexcept {
  return vec * scalar;
}

template<typename NumT>
constexpr Vec3<NumT> operator/(const Vec3<NumT>& vec, NumT scalar) noexcept {
  return Vec3<NumT>{vec.x() / scalar, vec.y() / scalar, vec.z() / scalar};
}

} // namespace ayan::math
