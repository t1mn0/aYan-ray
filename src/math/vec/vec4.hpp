#pragma once

#include <cmath>
#include <concepts>
#include <initializer_list>

#include "fwd.hpp"

namespace ayan::math {

// 4-dimensional vector:
template<typename NumT> requires (validate::ValidNumType<NumT>)
using Vec4 = Vec<4, NumT>;

template<typename NumT> requires (validate::ValidNumType<NumT>)
class Vec<4, NumT> {
private: // Fields:
  CacheFriendlyArr<NumT, 4> data; // x, y, z, w components of the vector;

public: // Member functions:
  // ----- ----- ---- Constructors ---- ----- -----
  constexpr Vec() noexcept : data{NumT(0), NumT(0), NumT(0), NumT(0)} {}

  constexpr Vec(NumT x, NumT y, NumT z, NumT w = NumT(1)) noexcept
    : data{x, y, z, w} {}

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec(std::initializer_list<U> init_list) noexcept : Vec() {
    if (init_list.size() != 4) return;
    auto it = init_list.begin();
    data[0] = *it++;
    data[1] = *it++;
    data[2] = *it++;
    data[3] = *it;
  }

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  explicit constexpr Vec(const Vec4<U>& oth) noexcept
    : data{oth.x(), oth.y(), oth.z(), oth.w()} {}

  // ----- ----- ---- Static member funcs ---- ----- -----
  static constexpr Vec4<NumT> UnitX() noexcept {
    return Vec4(NumT(1), NumT(0), NumT(0), NumT(0));
  }

  static constexpr Vec4<NumT> UnitY() noexcept {
    return Vec4(NumT(0), NumT(1), NumT(0), NumT(0));
  }

  static constexpr Vec4<NumT> UnitZ() noexcept {
    return Vec4(NumT(0), NumT(0), NumT(1), NumT(0));
  }

  static constexpr Vec4<NumT> UnitW() noexcept {
    return Vec4(NumT(0), NumT(0), NumT(0), NumT(1));
  }

  static constexpr Vec4<NumT> Zero() noexcept {
    return Vec4(NumT(0), NumT(0), NumT(0), NumT(0));
  }

  static constexpr Vec4<NumT> One() noexcept {
    return Vec4(NumT(1), NumT(1), NumT(1), NumT(1));
  }

  // ----- ----- ---- Element access ---- ----- -----
  constexpr NumT& x() noexcept { return data[0]; }
  constexpr NumT& y() noexcept { return data[1]; }
  constexpr NumT& z() noexcept { return data[2]; }
  constexpr NumT& w() noexcept { return data[3]; }

  constexpr const NumT& x() const noexcept { return data[0]; }
  constexpr const NumT& y() const noexcept { return data[1]; }
  constexpr const NumT& z() const noexcept { return data[2]; }
  constexpr const NumT& w() const noexcept { return data[3]; }

  // ---- ----- ----- Swizzling operations ---- ----- -----
  // [TODO]: maybe delete it later if it will be useless; or add all posible permutations;
  // Idk, it looks something like:
  // ...
  // case 1: return 1;
  // case 2: return 0;
  // case 3: return 1;
  // ...

  constexpr Vec4<NumT> xyzw() const noexcept { return *this; }
  constexpr Vec4<NumT> xywz() const noexcept { return Vec4<NumT>(x(), y(), w(), z()); }
  constexpr Vec4<NumT> xzyw() const noexcept { return Vec4<NumT>(x(), z(), y(), w()); }
  constexpr Vec4<NumT> xzwy() const noexcept { return Vec4<NumT>(x(), z(), w(), y()); }
  constexpr Vec4<NumT> xwyz() const noexcept { return Vec4<NumT>(x(), w(), y(), z()); }
  constexpr Vec4<NumT> xwzy() const noexcept { return Vec4<NumT>(x(), w(), z(), y()); }

  constexpr Vec4<NumT> yxzw() const noexcept { return Vec4<NumT>(y(), x(), z(), w()); }
  constexpr Vec4<NumT> yxwz() const noexcept { return Vec4<NumT>(y(), x(), w(), z()); }
  constexpr Vec4<NumT> yzxw() const noexcept { return Vec4<NumT>(y(), z(), x(), w()); }
  constexpr Vec4<NumT> yzwx() const noexcept { return Vec4<NumT>(y(), z(), w(), x()); }
  constexpr Vec4<NumT> ywxz() const noexcept { return Vec4<NumT>(y(), w(), x(), z()); }
  constexpr Vec4<NumT> ywzx() const noexcept { return Vec4<NumT>(y(), w(), z(), x()); }

  constexpr Vec4<NumT> zxyw() const noexcept { return Vec4<NumT>(z(), x(), y(), w()); }
  constexpr Vec4<NumT> zxwy() const noexcept { return Vec4<NumT>(z(), x(), w(), y()); }
  constexpr Vec4<NumT> zyxw() const noexcept { return Vec4<NumT>(z(), y(), x(), w()); }
  constexpr Vec4<NumT> zywx() const noexcept { return Vec4<NumT>(z(), y(), w(), x()); }
  constexpr Vec4<NumT> zwxy() const noexcept { return Vec4<NumT>(z(), w(), x(), y()); }
  constexpr Vec4<NumT> zwyx() const noexcept { return Vec4<NumT>(z(), w(), y(), x()); }

  constexpr Vec4<NumT> wxyz() const noexcept { return Vec4<NumT>(w(), x(), y(), z()); }
  constexpr Vec4<NumT> wxzy() const noexcept { return Vec4<NumT>(w(), x(), z(), y()); }
  constexpr Vec4<NumT> wyxz() const noexcept { return Vec4<NumT>(w(), y(), x(), z()); }
  constexpr Vec4<NumT> wyzx() const noexcept { return Vec4<NumT>(w(), y(), z(), x()); }
  constexpr Vec4<NumT> wzxy() const noexcept { return Vec4<NumT>(w(), z(), x(), y()); }
  constexpr Vec4<NumT> wzyx() const noexcept { return Vec4<NumT>(w(), z(), y(), x()); }

  constexpr Vec2<NumT> xy() const noexcept { return Vec2<NumT>(x(), y()); }
  constexpr Vec2<NumT> xz() const noexcept { return Vec2<NumT>(x(), z()); }
  constexpr Vec2<NumT> yz() const noexcept { return Vec2<NumT>(y(), z()); }

  constexpr Vec3<NumT> xyz() const noexcept { return *this; }
  constexpr Vec3<NumT> xzy() const noexcept { return Vec3<NumT>(x(), z(), y()); }
  constexpr Vec3<NumT> yxz() const noexcept { return Vec3<NumT>(y(), x(), z()); }
  constexpr Vec3<NumT> yzx() const noexcept { return Vec3<NumT>(y(), z(), x()); }
  constexpr Vec3<NumT> zxy() const noexcept { return Vec3<NumT>(z(), x(), y()); }
  constexpr Vec3<NumT> zyx() const noexcept { return Vec3<NumT>(z(), y(), x()); }

  // ----- ----- ---- Operators ----- ----- ----
  // unary:
  constexpr Vec operator-() const noexcept {
    return Vec(-x(), -y(), -z(), -w());
  }

  // with assignment:
  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Vec& operator+=(const Vec4<U>& oth) noexcept {
    data[0] += oth.x();
    data[1] += oth.y();
    data[2] += oth.z();
    data[3] += oth.w();
    return *this;
  }

  constexpr Vec& operator-=(const Vec4<NumT>& oth) noexcept {
    data[0] -= oth.x();
    data[1] -= oth.y();
    data[2] -= oth.z();
    data[3] -= oth.w();
    return *this;
  }

  constexpr Vec& operator*=(NumT scalar) noexcept {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;
    return *this;
  }

  constexpr Vec& operator/=(NumT scalar) noexcept {
    data[0] /= scalar;
    data[1] /= scalar;
    data[2] /= scalar;
    data[3] /= scalar;
    return *this;
  }

  constexpr Vec& operator*=(const Vec4<NumT>& oth) noexcept {
    data[0] *= oth.x();
    data[1] *= oth.y();
    data[2] *= oth.z();
    data[3] *= oth.w();
    return *this;
  }

  constexpr Vec& operator/=(const Vec4<NumT>& oth) noexcept {
    data[0] /= oth.x();
    data[1] /= oth.y();
    data[2] /= oth.z();
    data[3] /= oth.w();
    return *this;
  }

  // comparing:
  constexpr bool operator==(const Vec& oth) const noexcept {
    return (data[0] == oth.data[0] &&
            data[1] == oth.data[1] &&
            data[2] == oth.data[2] &&
            data[3] == oth.data[3]);
  }

  constexpr bool operator!=(const Vec& oth) const noexcept {
    return !(*this == oth);
  }

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr NumT dot(const Vec4<NumT>& oth) const noexcept {
    return x() * oth.x() + y() * oth.y() + z() * oth.z() + w() * oth.w();
  }

  NumT length() const {
    return std::sqrt(length_squared());
  }

  constexpr NumT length_squared() const noexcept {
    return dot(*this);
  }

  Vec4<NumT> normalize() const {
    NumT len = length();
    if (len > NumT{0}) {
      return *this / len;
    }
    return *this;
  }

  NumT distance(const Vec4<NumT>& oth) const {
    return (*this - oth).length();
  }

  constexpr NumT distance_squared(const Vec4<NumT>& oth) const noexcept {
    return (*this - oth).length_squared();
  }

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Vec4<std::invoke_result_t<Func, NumT>>
  {
    return Vec4<std::invoke_result_t<Func, NumT>>(
      func(x()), func(y()), func(z()), func(w())
    );
  }

  template <size_t Index>
  constexpr NumT& get() noexcept {
    static_assert(Index < 4, "Index out of bounds for Vec4");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
    else if constexpr (Index == 2) return z();
    else if constexpr (Index == 3) return w();
  }

  template <size_t Index>
  constexpr const NumT& get() const noexcept {
    static_assert(Index < 4, "Index out of bounds for Vec4");
    if constexpr (Index == 0) return x();
    else if constexpr (Index == 1) return y();
    else if constexpr (Index == 2) return z();
    else if constexpr (Index == 3) return w();
  }
};

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

namespace std {

template<typename NumT>
struct tuple_size<ayan::math::Vec4<NumT>>  : integral_constant<size_t, 4> {};

template<size_t Index, typename NumT>
struct tuple_element<Index, ayan::math::Vec4<NumT>> {
  static_assert(Index < 4, "Index out of bounds for Vec4");
  using type = NumT;
};

} // namespace std
