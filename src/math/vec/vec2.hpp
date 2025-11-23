#pragma once

#include <concepts>
#include <initializer_list>

#include "fwd.hpp"

namespace ayan::math {

template<typename NumT> requires (detail::ValidNumType<NumT>)
class Vec<2, NumT> {
private: // Fields:
  CacheFriendlyArr<NumT,2> data; // x, y components of the vector;

public: // Member functions:
  // ----- ----- ---- Constructors ---- ----- -----
  constexpr Vec() noexcept;
  constexpr Vec(NumT x, NumT y) noexcept;

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec(std::initializer_list<U> init_list) noexcept;

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  explicit constexpr Vec(const Vec2<U>& oth) noexcept;

  // ----- ----- ---- Static member funcs ---- ----- -----
  static constexpr Vec UnitX() noexcept;
  static constexpr Vec UnitY() noexcept;
  static constexpr Vec Zero() noexcept;
  static constexpr Vec One() noexcept;

  // ----- ----- ---- Element access ---- ----- -----
  constexpr NumT& x() noexcept;
  constexpr NumT& y() noexcept;
  constexpr const NumT& x() const noexcept;
  constexpr const NumT& y() const noexcept;

  // ----- ----- ---- Operators ----- ----- ----
  // unary:
  constexpr Vec operator-() const noexcept;

  // with assignment:
  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec& operator+=(const Vec2<U>& oth) noexcept;

  constexpr Vec& operator-=(const Vec& oth) noexcept;
  constexpr Vec& operator*=(NumT scalar) noexcept;
  constexpr Vec& operator/=(NumT scalar) noexcept;
  constexpr Vec& operator*=(const Vec& oth) noexcept;
  constexpr Vec& operator/=(const Vec& oth) noexcept;

  // comparing:
  constexpr bool operator==(const Vec& oth) const noexcept;
  constexpr bool operator!=(const Vec& oth) const noexcept;

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr NumT dot(const Vec& oth) const noexcept;
  NumT length() const noexcept;
  constexpr NumT lengthSquared() const noexcept;
  constexpr Vec perpendicular() const noexcept;
  Vec normalize() const noexcept;
  NumT distance(const Vec& oth) const noexcept;
  constexpr NumT distance_squared(const Vec& oth) const noexcept;

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Vec2<std::invoke_result<Func, NumT>>;

  // ----- ----- ---- Structure Binding Support ----- ----- ----
  template <size_t Index>
  constexpr NumT& get() noexcept;

  template <size_t Index>
  constexpr const NumT& get() const noexcept;
};

// ----- ----- ---- Binary operators ----- ----- ----
template<typename NumT>
constexpr Vec2<NumT> operator+(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator-(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator*(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator/(const Vec2<NumT>& a, const Vec2<NumT>& b) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator*(const Vec2<NumT>& vec, NumT scalar) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator*(NumT scalar, const Vec2<NumT>& vec) noexcept;

template<typename NumT>
constexpr Vec2<NumT> operator/(const Vec2<NumT>& vec, NumT scalar) noexcept;

} // namespace ayan::math;

// ----- ----- ---- Structure Binding Support ----- ----- ----

namespace std {

template<typename NumT>
struct tuple_size<ayan::math::Vec2<NumT>>  : integral_constant<size_t, 2> {};

template<size_t Index, typename NumT>
struct tuple_element<Index, ayan::math::Vec2<NumT>> {
  static_assert(Index < 2, "Index out of bounds for Vec2");
  using type = NumT;
};

} // namespace std


#include "impl/vec2.hpp"
