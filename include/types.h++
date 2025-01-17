#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>
namespace nutsloop::nlog::types {

struct log_settings_t {
  std::string ident;
  std::string filename;
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> session_header;
  bool active;
};

struct log_t {
  log_settings_t settings;
  std::ofstream stream;
  bool running;
};

enum Level {
  INFO,
  WARN,
  ERROR,
  NONE
};

using log_registry_t = std::unordered_map<std::string, log_t>;
}
