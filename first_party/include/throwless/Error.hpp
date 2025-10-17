#pragma once

#include <string>
#include <concepts>

namespace tmn::err {

//* <--- The basic concept that the "Error" type structure should correspond to --->
template<typename E>
concept Error = requires(const E& e, E&& moved) {
  // `.err_msg()` - method for providing an error message in form of `std::string`:
  { e.err_msg() } -> std::convertible_to<std::string>;
  // `.what()` - method for providing an error message in form of `const char*`;
  // this method allows you to ensure compatibility with the std::exception interface:
  { e.what() } -> std::convertible_to<const char*>;

  // basic requirements for objects `Error<E>`:
  requires std::destructible<E>;
  requires std::copy_constructible<E> || std::move_constructible<E>;
  requires std::equality_comparable<E>;
};

} // namespace tmn::err;
