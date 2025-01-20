#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>

namespace nutsloop::nlog::types {

const std::filesystem::path nutsloop_dir = std::filesystem::path(std::getenv("HOME")) / ".nutsloop";
const std::filesystem::path nutsloop_logs_directory = nutsloop_dir / "logs";

struct log_settings_t {

  std::string ident;
  std::string filename;
  bool active{};
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> session_header;

  log_settings_t() = default;
  log_settings_t( std::string ident_,
    std::string filename_,
    const bool active_,
    const std::optional<std::filesystem::path>& directory_,
    const std::optional<std::string>& session_header_ ) :
      ident{std::move( ident_ )},
      filename{std::move( filename_ )},
      active{ active_ },
      directory{ directory_ },
      session_header{ session_header_ } {

    if ( directory.has_value() ) {
      determine_directory_absolute_path_();
      absolute_path_ = directory.value() / filename;
    } else {
      absolute_path_ = nutsloop_logs_directory / filename;
    }
  }

  [[nodiscard]] std::filesystem::path absolute_path(){
    if ( absolute_path_.empty() ) {
      if ( directory.has_value() ) {
        determine_directory_absolute_path_();
        return directory.value() / filename;
      }
      return nutsloop_logs_directory / filename;
    }
    return absolute_path_;
  }

private:

  std::filesystem::path absolute_path_{};
  void determine_directory_absolute_path_() {

    if ( ! directory.value().is_absolute() ) {
      const std::filesystem::path current_working_directory = std::filesystem::canonical( std::filesystem::current_path() );
      directory.value() = current_working_directory / directory.value();
    }
  }
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
