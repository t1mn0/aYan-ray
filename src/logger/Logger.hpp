#pragma once

#include <throwless/Result.hpp>
#include "LogErr.hpp"

#include <string>
#include <fstream>

namespace ayan::log {

enum class LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

class Logger {
private: // fields:
  LogLevel current_level;
  std::ofstream logfile_stream;

  bool log_to_console;
  bool log_to_file;
  std::string logfile_path;

  bool is_initialized;

public: // methods:
  static Logger& Instance();

  tmn::Result<bool, err::LogErr> init();
  void shutdown();

  void debug(const std::string& message, const std::string& file = "", int line = 0);
  void info(const std::string& message, const std::string& file = "", int line = 0);
  void warning(const std::string& message, const std::string& file = "", int line = 0);
  void error(const std::string& message, const std::string& file = "", int line = 0);

  void set_level(LogLevel level);
  LogLevel get_level() const;

private:
  Logger();
  ~Logger();

  void log(LogLevel level, const std::string& message, const std::string& file, int line);
  std::string level_to_str(LogLevel level) const;
  bool is_should_log(LogLevel level) const;

};

// DEBUG is a compile-time macro. Assigned at the compilation stage!
#ifdef DEBUG
  #define LOG_DEBUG(message) Logger::Instance().debug(message, __FILE__, __LINE__)
  #define LOG_INFO(message) Logger::Instance().info(message, __FILE__, __LINE__)
  #define LOG_WARNING(message) Logger::Instance().warning(message, __FILE__, __LINE__)
  #define LOG_ERROR(message) Logger::Instance().error(message, __FILE__, __LINE__)
#else
  #define LOG_DEBUG(message)
  #define LOG_INFO(message) Logger::Instance().info(message)
  #define LOG_WARNING(message) Logger::Instance().warning(message)
  #define LOG_ERROR(message) Logger::Instance().error(message)
#endif

} // namespace ayan::log;
