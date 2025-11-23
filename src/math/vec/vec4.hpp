#pragma once

#include <concepts>
#include <initializer_list>

#include "fwd.hpp"

namespace ayan::math {

// 4-dimensional vector:
template<typename NumT> requires (detail::ValidNumType<NumT>)
using Vec4 = Vec<4, NumT>;

template<typename NumT> requires (detail::ValidNumType<NumT>)
class Vec<4, NumT> {
private: // Fields:
  CacheFriendlyArr<NumT, 4> data; // x, y, z, w components of the vector;

public: // Member functions:
  // ----- ----- ---- Constructors ---- ----- -----
  constexpr Vec() noexcept;
  constexpr Vec(NumT x, NumT y, NumT z, NumT w = NumT(1)) noexcept;

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec(std::initializer_list<U> init_list) noexcept;

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  explicit constexpr Vec(const Vec4<U>& oth) noexcept;

  // ----- ----- ---- Static member funcs ---- ----- -----
  static constexpr Vec4<NumT> UnitX() noexcept;
  static constexpr Vec4<NumT> UnitY() noexcept;
  static constexpr Vec4<NumT> UnitZ() noexcept;
  static constexpr Vec4<NumT> UnitW() noexcept;
  static constexpr Vec4<NumT> Zero() noexcept;
  static constexpr Vec4<NumT> One() noexcept;

  // ----- ----- ---- Element access ---- ----- -----
  constexpr NumT& x() noexcept;
  constexpr NumT& y() noexcept;
  constexpr NumT& z() noexcept;
  constexpr NumT& w() noexcept;
  constexpr const NumT& x() const noexcept;
  constexpr const NumT& y() const noexcept;
  constexpr const NumT& z() const noexcept;
  constexpr const NumT& w() const noexcept;

  // ----- ----- ---- Operators ----- ----- ----
  // unary:
  constexpr Vec operator-() const noexcept;

  // with assignment:
  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec& operator+=(const Vec4<U>& oth) noexcept;

  constexpr Vec& operator-=(const Vec4<NumT>& oth) noexcept;
  constexpr Vec& operator*=(NumT scalar) noexcept;
  constexpr Vec& operator/=(NumT scalar) noexcept;
  constexpr Vec& operator*=(const Vec4<NumT>& oth) noexcept;
  constexpr Vec& operator/=(const Vec4<NumT>& oth) noexcept;

  // comparing:
  constexpr bool operator==(const Vec& oth) const noexcept;
  constexpr bool operator!=(const Vec& oth) const noexcept;

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr NumT dot(const Vec4<NumT>& oth) const noexcept;
  NumT length() const;
  constexpr NumT length_squared() const noexcept;
  Vec4<NumT> normalize() const;
  NumT distance(const Vec4<NumT>& oth) const;
  constexpr NumT distance_squared(const Vec4<NumT>& oth) const noexcept;

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Vec4<std::invoke_result_t<Func, NumT>>;

  template <size_t Index>
  constexpr NumT& get() noexcept;

  template <size_t Index>
  constexpr const NumT& get() const noexcept;
};

// ----- ----- ---- Binary operators ----- ----- ----
template<typename NumT>
constexpr Vec4<NumT> operator+(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator-(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator*(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator/(const Vec4<NumT>& a, const Vec4<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator*(const Vec4<NumT>& vec, NumT scalar) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator*(NumT scalar, const Vec4<NumT>& vec) noexcept;

template<typename NumT>
constexpr Vec4<NumT> operator/(const Vec4<NumT>& vec, NumT scalar) noexcept;

} // namespace ayan::math

namespace std {

template<typename NumT>
struct tuple_size<ayan::math::Vec4<NumT>>  : integral_constant<size_t, 4> {};

template<size_t Index, typename NumT>
struct tuple_element<Index, ayan::math::Vec4<NumT>> {
  static_assert(Index < 4, "Index out of bounds for Vec4");
  using type = NumT;
};

} // namespace std

#include "impl/vec4.hpp"
