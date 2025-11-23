#pragma once

#include <concepts>
#include <initializer_list>
#include <cmath>

#include "fwd.hpp"

namespace ayan::math {

// template specializtions for planar structure with zero overhead by inheritance:
template<typename NumT> requires (validate::ValidNumType<NumT>)
class Vec<2, NumT> {
private: // Fields:
  CacheFriendlyArr<NumT,2> data; // x, y components of the vector;

public: // Member functions:
  // ----- ----- ---- Constructors ---- ----- -----
  constexpr Vec() noexcept : data {NumT(0), NumT(0)} {};
  constexpr Vec(NumT x, NumT y) noexcept : data {x, y} {}

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec(std::initializer_list<U> init_list) noexcept : Vec() {
    if (init_list.size() != 2) return;
    auto it = init_list.begin();
    data[0] = *it++;
    data[1] = *it++;
  }

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  explicit constexpr Vec(const Vec2<U>& oth) noexcept : data {oth.x(), oth.y()} {}

  // ----- ----- ---- Static member funcs ---- ----- -----
  static constexpr Vec2<NumT> UnitX() noexcept {
    return Vec2{NumT(1), NumT(0)};
  }

  static constexpr Vec2<NumT> UnitY() noexcept {
    return Vec2{NumT(0), NumT(1)};
  }

  static constexpr Vec2<NumT> Zero() noexcept {
    return Vec2{NumT(0), NumT(0)};
  }

  static constexpr Vec2<NumT> One() noexcept {
    return Vec2{NumT(1), NumT(1)};
  }

  // ----- ----- ---- Element access ---- ----- -----
  constexpr NumT& x() noexcept { return data[0]; }
  constexpr NumT& y() noexcept { return data[1]; }
  constexpr const NumT& x() const noexcept { return data[0]; }
  constexpr const NumT& y() const noexcept { return data[1]; }

  // ----- ----- ---- Operators ----- ----- ----
  // unary:
  constexpr Vec operator-() const noexcept { return Vec(-x(), -y()); }

  // with assignment:
  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec& operator+=(const Vec2<U>& oth) noexcept {
    data[0] += oth.data[0];
    data[1] += oth.data[1];
    return *this;
  }

  constexpr Vec& operator-=(const Vec& oth) noexcept {
    data[0] -= oth.data[0];
    data[1] -= oth.data[1];
    return *this;
  }

  constexpr Vec& operator*=(NumT scalar) noexcept {
    data[0] *= scalar;
    data[1] *= scalar;
    return *this;
  }

  constexpr Vec& operator/=(NumT scalar) noexcept {
    data[0] /= scalar;
    data[1] /= scalar;
    return *this;
  }

  constexpr Vec& operator*=(const Vec& oth) noexcept {
    data[0] *= oth.data[0];
    data[1] *= oth.data[1];
    return *this;
  }

  constexpr Vec& operator/=(const Vec& oth) noexcept {
    data[0] /= oth.data[0];
    data[1] /= oth.data[1];
    return *this;
  }

  // comparing:
  constexpr bool operator==(const Vec& oth) const noexcept {
    return (x() == oth.x() && y() == oth.y());
  }

  constexpr bool operator!=(const Vec& oth) const noexcept {
    return !(*this == oth);
  }

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr NumT dot(const Vec2<NumT>& oth) const noexcept {
    return x() * oth.x() + y() * oth.y();
  }

  NumT length() const noexcept {
    return std::sqrt(x() * x() + y() * y());
  }

  constexpr NumT lengthSquared() const noexcept {
    return x() * x() + y() * y();
  }

  constexpr Vec2<NumT> perpendicular() const noexcept {
    return Vec2{-y(), x()};
  }

  Vec2<NumT> normalize() const noexcept {
    NumT len = length();
    if (len > NumT{0}) {
      return *this / len;
    }
    return *this;
  }

  NumT distance(const Vec2<NumT>& oth) const noexcept {
    return (*this - oth).length();
  }

  constexpr NumT distance_squared(const Vec2<NumT>& oth) const noexcept {
    return (*this - oth).length_squared();
  }

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Vec2<std::invoke_result<Func, NumT>>
  {
    return Vec2<std::invoke_result<Func, NumT>>(
      func(x()), func(y())
    );
  }

  // ----- ----- ---- Structure Binding Support ----- ----- ----
  template <size_t Index>
  constexpr NumT& get() noexcept {
    static_assert(Index < 2, "Index out of bounds for Vec2");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
  }

  template <size_t Index>
  constexpr const NumT& get() const noexcept {
    static_assert(Index < 2, "Index out of bounds for Vec2");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
  }
};

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
