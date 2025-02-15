#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>

namespace nutsloop::nlog::types {

const std::filesystem::path nutsloop_dir =
    std::filesystem::path(std::getenv("HOME")) / ".nutsloop";
const std::filesystem::path nutsloop_logs_directory = nutsloop_dir / "logs";

struct log_settings_t {

  log_settings_t() = default;
  log_settings_t(std::string ident, std::string filename, const bool active,
                 const std::optional<std::filesystem::path> &directory,
                 const std::optional<std::string> &session_header)
      : ident_{std::move(ident)}, filename_{std::move(filename)},
        active_{active}, directory_{directory},
        session_header_{session_header} {

    if (directory.has_value()) {
      determine_directory_absolute_path_();
      absolute_path_ = directory.value() / filename_;
    } else {
      absolute_path_ = nutsloop_logs_directory / filename_;
    }
  }

  [[nodiscard]] std::string get_ident() const { return ident_; }

  [[nodiscard]] std::string get_filename() const { return filename_; }

  [[nodiscard]] bool get_active() const { return active_; }
  void set_active(const bool active) { active_ = active; }

  [[nodiscard]] std::optional<std::filesystem::path> get_directory() const {
    return directory_;
  }

  [[nodiscard]] std::optional<std::string> get_session_header() const {
    return session_header_;
  }

  [[nodiscard]] std::filesystem::path absolute_path() {
    if (absolute_path_.empty()) {
      if (directory_.has_value()) {
        determine_directory_absolute_path_();
        return directory_.value() / filename_;
      }
      return nutsloop_logs_directory / filename_;
    }
    return absolute_path_;
  }

private:
  std::string ident_;
  std::string filename_;
  bool active_{};
  std::optional<std::filesystem::path> directory_;
  std::optional<std::string> session_header_;
  std::filesystem::path absolute_path_{};

  void determine_directory_absolute_path_() {

    if (!directory_.value().is_absolute()) {
      const std::filesystem::path current_working_directory =
          std::filesystem::canonical(std::filesystem::current_path());
      directory_.value() = current_working_directory / directory_.value();
    }
  }
};

struct log_t {

  log_settings_t settings;
  std::ofstream stream;

  log_t() = default;
  explicit log_t(log_settings_t settings)
      : settings{std::move(settings)}, stream{std::ofstream()},
        running_{settings.get_active()} {}

  bool is_running() const { return running_; }
  void set_running(const bool running) { running_ = running; }
private:
  bool running_{};

};

using log_registry_t = std::unordered_map<std::string, log_t>;
} // namespace nutsloop::nlog::types
