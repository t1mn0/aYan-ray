#pragma once

#include <cstdint>

#include <type_traits>

namespace ayan::math::validate {

template <uint8_t N>
concept ValidLen = (N >= 2 && N <= 4);

template <typename T>
concept ValidNumType = std::is_arithmetic_v<T>;

} // namespace ayan::math;
