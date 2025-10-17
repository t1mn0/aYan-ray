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

struct AnyErr {
protected:
  std::string msg_;

public:
  AnyErr() = default;
  AnyErr(const std::string& msg) : msg_(msg) {}
  AnyErr(const char* msg) : msg_(msg) {}

  template <typename...Args> requires std::constructible_from<std::string, Args...>
  AnyErr(Args&&... args) : msg_(args...) {}

  virtual ~AnyErr() = default;

  virtual std::string err_msg() const { return msg_; }
  virtual const char* what() const noexcept { return msg_.c_str(); }

  virtual bool operator==(const AnyErr& oth) const noexcept {
    return msg_ == oth.msg_;
  }
};

static_assert(tmn::err::Error<AnyErr>, "AnyErr must satisfy Error concept");

} // namespace tmn::err;
