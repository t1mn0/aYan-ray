#pragma once

#include <type_traits>

namespace ayan::math::detail {

template <typename T>
concept ValidNumType = std::is_arithmetic_v<T>;

} // namespace ayan::math::detail;
