#pragma once

#include <concepts>
#include <type_traits>
#include <stdexcept>
#include <utility>

#include "Error.hpp"

namespace tmn {

// ===== ===== ===== ===== ===== DECLARATION ===== ===== ===== ===== =====

template<typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
class Result {
private: //* substructures:
  enum class State {
    OkState,
    ErrState,
    Uninitialized
  };

private: //* fields:
  State state;

  union {
    T ok_val;
    E err_val;
  };

public: //* methods:
  //*   <--- constructors, (~)ro5, destructor --->
  Result() = delete;
  Result(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
  Result(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
  Result& operator=(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
  Result& operator=(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);

  template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  Result(const Result<U, F>& oth) noexcept(std::is_nothrow_constructible_v<T, U> && std::is_nothrow_constructible_v<E, F>);

  template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  Result(Result<U, F>&& oth) noexcept(std::is_nothrow_constructible_v<T, U> && std::is_nothrow_constructible_v<E, F>);

  template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  Result& operator=(const Result<U, F>& oth) noexcept(std::is_nothrow_assignable_v<T, U> && std::is_nothrow_assignable_v<E, F>);

  template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  Result& operator=(Result<U, F>&& oth) noexcept(std::is_nothrow_assignable_v<T, U> && std::is_nothrow_assignable_v<E, F>);

  ~Result();

  // Conversions (cast) :
  // true if ok_val in union; false if err_val;
  explicit operator bool() const noexcept;

  //*   <--- static mnemonic methods that call the constructor from an argument --->
  static Result Ok(const T& val) noexcept   requires (!std::is_void_v<T>);
  static Result Ok(T&& val) noexcept        requires (!std::is_void_v<T>);
  static Result Err(const E& error) noexcept;
  static Result Err(E&& error) noexcept;

  template<typename... Args> requires std::constructible_from<T, Args...>
  static Result Ok(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>);

  template<typename... Args> requires std::constructible_from<E, Args...>
  static Result Err(Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args...>);

  //*   <--- specialized algorithms & methods  --->
  bool is_ok() const noexcept;
  bool is_err() const noexcept;

  T& unwrap_value_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
  const T& unwrap_value_or(const T& val) const noexcept(std::is_nothrow_copy_constructible_v<T>);
  T& unwrap_value();
  const T& unwrap_value() const;
  T unwrap_value_or_default() const noexcept requires std::default_initializable<T>;

  E& unwrap_err();
  const E& unwrap_err() const;

private: //* methods:
  void swap(Result<T,E>& oth)
    noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_swappable_v<E> && std::is_nothrow_move_constructible_v<E>);

  //* private constructors:
  //*   <--- constructors that are called by static methods Ok(val), Err(error) --->
  Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
  Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>);
  Result(const E& err) noexcept(std::is_nothrow_copy_constructible_v<E>);
  Result(E&& err) noexcept(std::is_nothrow_move_constructible_v<E>);

private: //* friends:
  friend void swap(Result<T,E>& first, Result<T,E>& second) noexcept(noexcept(first.swap(second))) {
    first.swap(second);
  }

}; // class Result;


// ===== ===== ===== ===== ===== DEFINITIONS ===== ===== ===== ===== =====

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>)
  : state(State::OkState), ok_val(val) {}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>)
  : state(State::OkState), ok_val(std::move(val)) {}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(const E& err) noexcept(std::is_nothrow_copy_constructible_v<E>)
  : state(State::ErrState), err_val(err) {}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(E&& err) noexcept(std::is_nothrow_move_constructible_v<E>)
  : state(State::ErrState), err_val(std::move(err)) {}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E> Result<T, E>::Ok(const T& val) noexcept requires (!std::is_void_v<T>) {
  return Result(val);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E> Result<T, E>::Ok(T&& val) noexcept requires (!std::is_void_v<T>) {
  return Result(val);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E> Result<T, E>::Err(const E& err) noexcept {
  return Result(err);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E> Result<T, E>::Err(E&& err) noexcept {
  return Result(std::move(err));
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename... Args> requires std::constructible_from<T, Args...>
Result<T,E> Result<T, E>::Ok(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
  T v {std::forward<Args>(args)...};
  return Result(v);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename... Args> requires std::constructible_from<E, Args...>
Result<T, E> Result<T, E>::Err(Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args...>) {
  E e {std::forward<Args>(args)...};
  return Result(e);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(const Result& oth)
  noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
{
  if (oth.state == State::OkState) {
    new (&ok_val) T(oth.ok_val);
    state = State::OkState;
  }
  else if (oth.state == State::ErrState) {
    new (&err_val) E(oth.err_val);
    state = State::ErrState;
  }
  else if (oth.state == State::Uninitialized) {
    state = State::Uninitialized;
  }
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::Result(Result&& oth)
  noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
{
  if (oth.is_ok()) {
    new (&ok_val) T(std::move(oth.ok_val));
    state = State::OkState;
    oth.ok_val.~T();
    oth.state = State::Uninitialized;
  }
  else {
    new (&err_val) E(std::move(oth.err_val));
    state = State::ErrState;
    oth.err_val.~E();
    oth.state = State::Uninitialized;
  }
}

template<typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
Result<T, E>::Result(const Result<U, F>& oth)
  noexcept(std::is_nothrow_constructible_v<T, U> && std::is_nothrow_constructible_v<E, F>)
{
  if (oth.is_ok()) {
    new (&ok_val) T(oth.unwrap());
    state = State::OkState;
  }
  else {
    new (&err_val) E(oth.unwrap_err());
    state = State::ErrState;
  }
}

template<typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
Result<T, E>::Result(Result<U, F>&& oth)
  noexcept(std::is_nothrow_constructible_v<T, U> && std::is_nothrow_constructible_v<E, F>)
{
  if (oth.is_ok()) {
    new (&ok_val) T(std::move(oth).unwrap_value());
    state = State::OkState;
  }
  else {
    new (&err_val) E(std::move(oth).unwrap_err());
    state = State::ErrState;
  }
}

template<typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
auto Result<T, E>::operator=(const Result<U, F>& oth)
  noexcept(std::is_nothrow_assignable_v<T, U> && std::is_nothrow_assignable_v<E, F>)
  -> Result<T, E>&
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(oth.ok_val);
  }
  else if (state == State::ErrState) {
    new (&err_val) E(oth.err_val);
  }

  return *this;
}

template<typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
template<typename U, typename F> requires std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
auto Result<T, E>::operator=(Result<U, F>&& oth)
  noexcept(std::is_nothrow_assignable_v<T, U> && std::is_nothrow_assignable_v<E, F>)
  -> Result<T, E>&
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(std::move(oth.ok_val));
    oth.ok_val.~T();
  }
  else if (state == State::ErrState) {
    new (&err_val) E(std::move(oth.err_val));
    oth.err_val.~E();
  }

  return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E>& Result<T, E>::operator=(const Result<T,E>& oth)
  noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(oth.ok_val);
  }
  else if (state == State::ErrState) {
    new (&err_val) E(oth.err_val);
  }

  return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T,E>& Result<T,E>::operator=(Result<T,E>&& oth)
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(std::move(oth.ok_val));
    oth.ok_val.~T();
  }
  else if (state == State::ErrState) {
    new (&err_val) E(std::move(oth.err_val));
    oth.err_val.~E();
  }

  return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::~Result() {
  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
void Result<T, E>::swap(Result<T, E> &oth)
    noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> &&
             std::is_nothrow_swappable_v<E> && std::is_nothrow_move_constructible_v<E>)
{
  if (this == &oth) return;

  Result<T,E> temp(std::move(*this));
  *this = std::move(oth);
  oth = std::move(temp);
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
Result<T, E>::operator bool() const noexcept{
  return is_ok();
}

//*   <--- specialized algorithms & methods  --->

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
bool Result<T,E>::is_ok() const noexcept {
  return state == State::OkState;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
bool Result<T,E>::is_err() const noexcept {
  return state == State::ErrState;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
T& Result<T,E>::unwrap_value_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>) {
  return is_ok() ? ok_val : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
const T& Result<T,E>::unwrap_value_or(const T& val) const
  noexcept(std::is_nothrow_copy_constructible_v<T>)
{
  return is_ok() ? ok_val : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
T& Result<T, E>::unwrap_value() {
  if (is_err()) {
    throw std::runtime_error(err_val.err_msg());
  }
  else if (state == State::Uninitialized) {
    throw std::runtime_error("Accessing moved-from Result");
  }

  return ok_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
const T& Result<T, E>::unwrap_value() const {
  if (is_err()) {
    throw std::runtime_error(err_val.err_msg());
  }
  return ok_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
T Result<T, E>::unwrap_value_or_default() const noexcept requires std::default_initializable<T> {
  return is_ok() ? ok_val : T{};
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
E& Result<T, E>::unwrap_err() {
  if (is_ok()) {
    throw std::runtime_error("Result<T, E> does not contain Error");
  }
  return err_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && err::Error<E>)
const E& Result<T, E>::unwrap_err() const {
  if (is_ok()) {
    throw std::runtime_error("Result<T, E> does not contain Error");
  }
  return err_val;
}

} // namespace tmn;
