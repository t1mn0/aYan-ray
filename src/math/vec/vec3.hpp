#pragma once

#include <cmath>
#include <concepts>
#include <initializer_list>

#include "fwd.hpp"

namespace ayan::math {

template<typename NumT> requires (detail::ValidNumType<NumT>)
class Vec<3, NumT> {
private: // Fields:
  CacheFriendlyArr<NumT, 3> data; // x, y, z components of the vector;

public: // Member functions:
  // ----- ----- ---- Constructors ---- ----- -----
  constexpr Vec() noexcept : data{NumT(0), NumT(0), NumT(0)} {}

  constexpr Vec(NumT x, NumT y, NumT z) noexcept : data{x, y, z} {}

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec(std::initializer_list<U> init_list) noexcept : Vec() {
    if (init_list.size() != 3) return;
    auto it = init_list.begin();
    data[0] = *it++;
    data[1] = *it++;
    data[2] = *it;
  }

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  explicit constexpr Vec(const Vec3<U>& oth) noexcept
    : data{oth.x(), oth.y(), oth.z()} {}

  // ----- ----- ---- Static member funcs ---- ----- -----
  static constexpr Vec3<NumT> UnitX() noexcept {
    return Vec3(NumT(1), NumT(0), NumT(0));
  }

  static constexpr Vec3<NumT> UnitY() noexcept {
    return Vec3(NumT(0), NumT(1), NumT(0));
  }

  static constexpr Vec3<NumT> UnitZ() noexcept {
    return Vec3(NumT(0), NumT(0), NumT(1));
  }

  static constexpr Vec3<NumT> Zero() noexcept {
    return Vec3(NumT(0), NumT(0), NumT(0));
  }

  static constexpr Vec3<NumT> One() noexcept {
    return Vec3(NumT(1), NumT(1), NumT(1));
  }

  // ----- ----- ---- Element access ---- ----- -----
  constexpr NumT& x() noexcept { return data[0]; }
  constexpr NumT& y() noexcept { return data[1]; }
  constexpr NumT& z() noexcept { return data[2]; }

  constexpr const NumT& x() const noexcept { return data[0]; }
  constexpr const NumT& y() const noexcept { return data[1]; }
  constexpr const NumT& z() const noexcept { return data[2]; }

  // ----- ----- ---- Swizzling operations ---- ----- -----
  constexpr Vec2<NumT> xy() const noexcept { return Vec2<NumT>(x(), y()); }
  constexpr Vec2<NumT> xz() const noexcept { return Vec2<NumT>(x(), z()); }
  constexpr Vec2<NumT> yz() const noexcept { return Vec2<NumT>(y(), z()); }

  // [TODO]: maybe delete it later if it will be useless; or add all posible permutations;
  constexpr Vec3<NumT> xzy() const noexcept { return Vec3<NumT>(x(), z(), y()); }
  constexpr Vec3<NumT> yxz() const noexcept { return Vec3<NumT>(y(), x(), z()); }
  constexpr Vec3<NumT> yzx() const noexcept { return Vec3<NumT>(y(), z(), x()); }
  constexpr Vec3<NumT> zxy() const noexcept { return Vec3<NumT>(z(), x(), y()); }
  constexpr Vec3<NumT> zyx() const noexcept { return Vec3<NumT>(z(), y(), x()); }

  // ----- ----- ---- Operators ----- ----- ----
  // unary:
  constexpr Vec operator-() const noexcept {
    return Vec(-x(), -y(), -z());
  }

  // with assignment:
  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec& operator+=(const Vec3<U>& oth) noexcept {
    data[0] += oth.x();
    data[1] += oth.y();
    data[2] += oth.z();
    return *this;
  }

  constexpr Vec& operator-=(const Vec3<NumT>& oth) noexcept {
    data[0] -= oth.x();
    data[1] -= oth.y();
    data[2] -= oth.z();
    return *this;
  }

  constexpr Vec& operator*=(NumT scalar) noexcept {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    return *this;
  }

  constexpr Vec& operator/=(NumT scalar) noexcept {
    data[0] /= scalar;
    data[1] /= scalar;
    data[2] /= scalar;
    return *this;
  }

  constexpr Vec& operator*=(const Vec3<NumT>& oth) noexcept {
    data[0] *= oth.x();
    data[1] *= oth.y();
    data[2] *= oth.z();
    return *this;
  }

  constexpr Vec& operator/=(const Vec3<NumT>& oth) noexcept {
    data[0] /= oth.x();
    data[1] /= oth.y();
    data[2] /= oth.z();
    return *this;
  }

  // comparing:
  constexpr bool operator==(const Vec& oth) const noexcept {
    return (data[0] == oth.data[0] &&
            data[1] == oth.data[1] &&
            data[2] == oth.data[2]);
  }

  constexpr bool operator!=(const Vec& oth) const noexcept {
    return !(*this == oth);
  }

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr NumT dot(const Vec3<NumT>& oth) const noexcept {
    return x() * oth.x() + y() * oth.y() + z() * oth.z();
  }

  NumT length() const noexcept {
    return std::sqrt(x() * x() + y() * y() + z() * z());
  }

  constexpr NumT length_squared() const noexcept {
    return x() * x() + y() * y() + z() * z();
  }

  Vec3<NumT> normalize() const noexcept {
    NumT len = length();
    if (len > NumT{0}) {
      return *this / len;
    }
    return *this;
  }

  NumT distance(const Vec3<NumT>& oth) const noexcept {
    return (*this - oth).length();
  }

  constexpr NumT distance_squared(const Vec3<NumT>& oth) const noexcept {
    return (*this - oth).length_squared();
  }

  // Cross product (only for 3D vectors):
  constexpr Vec3<NumT> cross(const Vec3<NumT>& oth) const noexcept {
    return Vec3<NumT>(
      y() * oth.z() - z() * oth.y(),
      z() * oth.x() - x() * oth.z(),
      x() * oth.y() - y() * oth.x()
    );
  }

  // Triple product (scalar result):
  constexpr NumT triple(const Vec3<NumT>& b, const Vec3<NumT>& c) const noexcept {
    return dot(cross(b), c);
  }

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Vec3<std::invoke_result_t<Func, NumT>>
  {
    return Vec3<std::invoke_result_t<Func, NumT>>(
      func(x()), func(y()), func(z())
    );
  }

  template <size_t Index>
  constexpr NumT& get() noexcept {
    static_assert(Index < 3, "Index out of bounds for Vec3");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
    else if constexpr (Index == 2) return z();
  }

  template <size_t Index>
  constexpr const NumT& get() const noexcept {
    static_assert(Index < 3, "Index out of bounds for Vec3");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
    else if constexpr (Index == 2) return z();
  }
};

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

// ----- ----- ---- Structure Binding Support ----- ----- ----

namespace std {

template<typename NumT>
struct tuple_size<ayan::math::Vec3<NumT>>  : integral_constant<size_t, 3> {};

template<size_t Index, typename NumT>
struct tuple_element<Index, ayan::math::Vec3<NumT>> {
  static_assert(Index < 3, "Index out of bounds for Vec3");
  using type = NumT;
};

} // namespace std
