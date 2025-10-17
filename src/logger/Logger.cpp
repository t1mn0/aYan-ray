#include "Logger.hpp"
#include "../config/Config.hpp"
#include "LogErr.hpp"
#include "throwless/Result.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

namespace ayan::log {

Logger& Logger::Instance() {
  static Logger instance;
  return instance;
}

Logger::Logger() : current_level(LogLevel::DEBUG), is_initialized(false) {}

Logger::~Logger() {
  shutdown();
}

tmn::Result<bool, err::LogErr> Logger::init() {
  if (is_initialized) return tmn::Result<bool, err::LogErr>::Err("Logger has already been initiated");

  auto& config = cfg::Config::Instance();

  auto log_level_result = config.get_param_as_string("log_level");
  std::string log_level_as_str = "";

  if (log_level_result.is_ok()){
    log_level_as_str = log_level_result.unwrap_value();
    if (log_level_as_str == "INFO") current_level = LogLevel::INFO;
    else if (log_level_as_str == "WARNING") current_level = LogLevel::WARNING;
    else if (log_level_as_str == "ERROR") current_level = LogLevel::ERROR;
    else current_level = LogLevel::DEBUG;
  }
  else {
    log_level_as_str = "DEBUG";
  }

  log_to_console = config.get_param_as_string("log_to_console").is_ok();
  log_to_file = config.get_param_as_string("log_to_file").is_ok();

  logfile_path = config.get_param_as_string("log_file_path").unwrap_value_or(std::string(PROJECT_ROOT_PATH) + std::string("main.log"));

  if (log_to_file) {
    logfile_stream.open(logfile_path, std::ios::app);
    if (!logfile_stream.is_open()) {
      std::cerr << "Cannot open log file: " << logfile_path << std::endl;
      log_to_file = false;
    }
  }

  is_initialized = true;

  info("＼(＾▽＾)／   Logger initialized!");

  return tmn::Result<bool, err::LogErr>::Ok(true);
}

void Logger::shutdown() {
  info("(－.－)...zzz    Logger shutting down");
  if (logfile_stream.is_open()) {
    logfile_stream.close();
  }
  is_initialized = false;
}

void Logger::debug(const std::string& message, const std::string& file, int line) {
  log(LogLevel::DEBUG, message, file, line);
}

void Logger::info(const std::string& message, const std::string& file, int line) {
  log(LogLevel::INFO, message, file, line);
}

void Logger::warning(const std::string& message, const std::string& file, int line) {
  log(LogLevel::WARNING, message, file, line);
}

void Logger::error(const std::string& message, const std::string& file, int line) {
  log(LogLevel::ERROR, message, file, line);
}

void Logger::log(LogLevel level, const std::string& message, const std::string& file, int line) {
  if (!is_initialized || !is_should_log(level)) return;

  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream log_stream;
  log_stream << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << " [" << level_to_str(level) << "] ";

  #ifdef DEBUG
    if (!file.empty()) {
      log_stream << "[" << file << ":" << line << "] ";
    }
  #endif

  log_stream << message;

  std::string log_msg = log_stream.str();

  if (log_to_console) {
    std::cout << log_msg << std::endl;
  }

  if (log_to_file && logfile_stream.is_open()) {
    logfile_stream << log_msg << std::endl;
    logfile_stream.flush();
  }
}

std::string Logger::level_to_str(LogLevel level) const {
  switch (level) {
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR: return "ERROR";
    default: return "DEBUG";
  }
}

bool Logger::is_should_log(LogLevel level) const {
  return level >= current_level;
}

void Logger::set_level(LogLevel level) {
  current_level = level;
}

LogLevel Logger::get_level() const {
  return current_level;
}

} // namespace ayan::log;
