#include "Config.hpp"
#include "ConfigErr.hpp"

#include <fstream>
#include <algorithm>

namespace ayan::cfg {

// ------------------------- Config Public Methods -------------------------

using namespace ayan::cfg::err;
using namespace tmn;

Config& Config::Instance() {
  static Config instance;
  return instance;
}

Result<bool, InvalidConfigFileErr> Config::load_params_from_file(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return Result<bool, InvalidConfigFileErr>::Err(
      InvalidConfigFileErr("Cannot open config file: " + filename)
    );
  }

  std::string line;
  int line_number = 0;

  while (std::getline(file, line)) {
    line_number++;

    // skip empty lines and comments:
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    if (line.empty() || line[0] == '#' || line[0] == ';') {
      continue;
    }

    // parsing "key=value" format lines:
    size_t delimiter_pos = line.find('=');
    if (delimiter_pos == std::string::npos) {
      return Result<bool, InvalidConfigFileErr>::Err(
          InvalidConfigFileErr("Invalid format at line " + std::to_string(line_number) + ": missing '=' delimiter")
      );
    }

    std::string key = line.substr(0, delimiter_pos);
    std::string value = line.substr(delimiter_pos + 1);

    if (key.empty()) {
      return Result<bool, InvalidConfigFileErr>::Err(
        InvalidConfigFileErr("Empty key at line " + std::to_string(line_number))
      );
    }

    params_map[key] = value;
  }

  file.close();
  return Result<bool, InvalidConfigFileErr>::Ok(true);
}

Result<std::string, ConfigErr> Config::get_param_as_string(const std::string& param_name) {
  auto it = params_map.find(param_name);
  if (it == params_map.end()) {
    return Result<std::string, ConfigErr>::Err(
      ConfigParamNotFoundErr("Parameter not found: " + param_name)
    );
  }
  return Result<std::string, ConfigErr>::Ok(it->second);
}

Result<int, ConfigErr> Config::get_param_as_int(const std::string& param_name) {
  auto string_result = get_param_as_string(param_name);
  if (string_result.is_err()) {
    return Result<int, ConfigParamNotFoundErr>::Err(string_result.unwrap_err());
  }

  std::string value = string_result.unwrap_value();
  try {
    int result = std::stoi(value);
    return Result<int, ConfigErr>::Ok(result);
  }
  catch (const std::exception& e) {
    return Result<int, ConfigParamConversionErr>::Err(
      ConfigParamConversionErr("Cannot convert parameter '" + param_name + "' to int: " + e.what())
    );
  }
}

Result<double, ConfigParamNotFoundErr> Config::get_param_as_double(const std::string& param_name) {
  auto string_result = get_param_as_string(param_name);
  if (string_result.is_err()) {
    return Result<double, ConfigParamNotFoundErr>::Err(string_result.unwrap_err());
  }

  std::string value = string_result.unwrap_value();
  try {
    double result = std::stod(value);
    return Result<double, ConfigErr>::Ok(result);
  }
  catch (const std::exception& e) {
    return Result<double, ConfigParamNotFoundErr>::Err(
      ConfigParamConversionErr("Cannot convert parameter '" + param_name + "' to double: " + e.what())
    );
  }
}

Result<bool, ConfigParamNotFoundErr> Config::get_param_as_bool(const std::string& param_name) {
  auto string_result = get_param_as_string(param_name);
  if (string_result.is_err()) {
    return Result<bool, ConfigParamNotFoundErr>::Err(string_result.unwrap_err());
  }

  std::string value = string_result.unwrap_value();

  std::string lower_value = value;
  std::transform(lower_value.begin(), lower_value.end(), lower_value.begin(), ::tolower);

  if (lower_value == "true" || lower_value == "1" || lower_value == "yes" || lower_value == "on") {
    return Result<bool, ConfigErr>::Ok(true);
  }
  else if (lower_value == "false" || lower_value == "0" || lower_value == "no" || lower_value == "off") {
    return Result<bool, ConfigErr>::Ok(false);
  }
  else {
    return Result<bool, ConfigParamNotFoundErr>::Err(
      ConfigParamConversionErr("Cannot convert parameter '" + param_name + "' to bool. Value: " + value)
    );
  }
}

} // namespace ayan::cfg;
