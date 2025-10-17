#pragma once

#include <throwless/Result.hpp>
#include "ConfigErr.hpp"

#include <string>
#include <unordered_map>

namespace ayan::cfg {

class Config {
private: // fields:
  // config_map is storage for all config params in the 'key-value' form.
  // using map (not explicit config fields) will allow you not to
  // rewrite the code of the entire class, if necessary, add a new parameter:
  // it will be enough to rewrite the parser component for initialization:
  std::unordered_map<std::string, std::string> params_map;

private: // methods:
  Config() = default;

public: // methods:
  // I think that for naming static methods, due to the possibility of using them
  // not as methods of specific object, we can use UpperCamelCase,
  // similarly for naming classes:
  static Config& Instance();

  // [TODO]: add validation for config fields:
  // now: load_params_from_file -> parse lines -> init them in Config;
  // [TODO]:  load_params_from_file -> parse lines -> validate it (sift out invalid fields) -> init them in Config;
  tmn::Result<bool, err::InvalidConfigFileErr> load_params_from_file(const std::string& filename);

  auto get_param_as_string(const std::string& param_name) -> tmn::Result<std::string, err::ConfigErr>;
  auto get_param_as_int(const std::string& param_name) -> tmn::Result<int, err::ConfigErr>;
  auto get_param_as_double(const std::string& param_name) -> tmn::Result<double, err::ConfigErr>;
  auto get_param_as_bool(const std::string& param_name) -> tmn::Result<bool, err::ConfigErr>;

}; // class Config;

} // namespace ayan::cfg;
