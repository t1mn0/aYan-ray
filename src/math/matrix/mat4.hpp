#pragma once

#include <concepts>
#include <initializer_list>
#include <array>

#include <ayan/math/vec.hpp>
#include "fwd.hpp"

namespace ayan::math {

template<typename NumT> requires (detail::ValidNumType<NumT>)
class Mat<4, 4, NumT> {
private:
  // column-major storage for cache locality:
  std::array<Vec4<NumT>, 4> columns;

public:
  // ----- ----- ---- Constructors ---- ----- -----
  // Identity Mat as a neutral element in matrices multiplication:
  constexpr Mat() noexcept;
  static constexpr Mat Identity() noexcept;

  constexpr Mat(
    const Vec4<NumT>& col0, const Vec4<NumT>& col1,
    const Vec4<NumT>& col2, const Vec4<NumT>& col3) noexcept;

  template<typename U> requires (std::same_as<U, NumT> || std::convertible_to<U, NumT>)
  constexpr Mat(std::initializer_list<U> init_list) noexcept;

  // ----- ----- ---- Element Access ---- ----- -----
  template <size_t index> requires (index < 4)
  constexpr Vec4<NumT> row() noexcept;

  template <size_t index> requires (index < 4)
  constexpr Vec4<NumT>& col() noexcept;

  template <size_t index> requires (index < 4)
  constexpr const Vec4<NumT>& col() const noexcept;

  // ----- ----- ---- Operators ---- ----- -----
  constexpr Mat4<NumT> operator+(const Mat4<NumT>& oth) const noexcept;
  constexpr Mat4<NumT> operator-(const Mat4<NumT>& oth) const noexcept;
  constexpr Mat4<NumT> operator*(NumT scalar) const noexcept;
  constexpr Mat4<NumT> operator/(NumT scalar) const noexcept;
  constexpr Mat4<NumT>& operator+=(const Mat4<NumT>& oth) noexcept;
  constexpr Mat4<NumT>& operator*=(const Mat4<NumT>& oth) noexcept;
  constexpr bool operator==(const Mat4<NumT>& oth) const noexcept;
  constexpr bool operator!=(const Mat4<NumT>& oth) const noexcept;

  // ----- ----- ---- Linear Algebra Operations ----- ----- ----
  constexpr Mat4<NumT> operator*(const Mat4<NumT>& oth) const noexcept;
  constexpr NumT determinant() const noexcept;
  constexpr Mat4<NumT> transpose() const noexcept;
  constexpr NumT trace() const noexcept;

  // Properties checking:
  constexpr bool is_identity() const noexcept;
  constexpr bool is_diagonal() const noexcept;
  constexpr bool is_symmetric() const noexcept;
  constexpr bool is_orthogonal() const noexcept;

  // ----- ----- ---- Utility functional methods ----- ----- ----
  template <typename Func> requires std::invocable<Func, NumT>
  constexpr auto map(Func&& func) noexcept(std::is_nothrow_invocable_v<Func, NumT>)
    -> Mat4<std::invoke_result_t<Func, NumT>>;
};

// ----- ----- ---- External Operators ---- ----- ----
template<typename NumT>
constexpr Mat4<NumT> operator*(NumT scalar, const Mat4<NumT>& mat) noexcept;

} // namespace ayan::math

#include "impl/mat4.hpp"
