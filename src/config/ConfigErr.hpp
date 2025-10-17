#pragma once

#include <throwless/Error.hpp>

#include <string>

namespace ayan::cfg::err {

struct ConfigErr {
protected:
  std::string msg_;

public:
  ConfigErr() = default;
  ConfigErr(const std::string& msg) : msg_(msg) {}
  ConfigErr(const char* msg) : msg_(msg) {}

  template <typename...Args> requires std::constructible_from<std::string, Args...>
  ConfigErr(Args&&... args) : msg_(args...) {}

  virtual ~ConfigErr() = default;

  virtual std::string err_msg() const { return msg_; }
  virtual const char* what() const noexcept { return msg_.c_str(); }

  virtual bool operator==(const ConfigErr& oth) const noexcept {
    return msg_ == oth.msg_;
  }
};

static_assert(tmn::err::Error<ConfigErr>, "ConfigErr must satisfy Error concept");

// mnemonically convenient names for errors:
using ConfigParamNotFoundErr = ConfigErr;
using InvalidConfigFileErr = ConfigErr;
using ConfigParamConversionErr = ConfigErr;

} // namespace ayan::cfg::err;
