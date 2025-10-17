#include "src/config/Config.hpp"
#include "src/logger/Logger.hpp"

#include <iostream>

std::string config_path = std::string(PROJECT_ROOT_PATH) + "/src/config/config.ini";

int main(){
  auto& config = ayan::cfg::Config::Instance();
  if (config.load_params_from_file(config_path).is_err()){
    std::cerr << "Failed config initialization: "
      << config.load_params_from_file(config_path).unwrap_err().err_msg() << std::endl;
    return 0;
  }

  auto& logger = ayan::log::Logger::Instance();

  if (logger.init().is_err()){
    std::cerr << "Failed logger initialization" << std::endl;
  }

  ayan::log::LOG_INFO("1234567890");

  logger.shutdown();

  return 0;
}
