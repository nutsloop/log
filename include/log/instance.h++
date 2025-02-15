#pragma once

#include "types.h++"

namespace nutsloop::nlog {

using namespace nutsloop::nlog::types;

// TODO: DEBUG_LOG
class instance{
public:
  explicit instance( log_t* log );

  // MARK: (log_instance) log_settings_t
  [[nodiscard]] std::string ident() const;
  // HINT: not implemented yet.
  [[nodiscard]] std::string filename() const = delete;
  [[nodiscard]] bool active() const = delete;
  [[nodiscard]] std::optional<std::filesystem::path> directory() const = delete;
  [[nodiscard]] std::optional<std::string> header() const = delete;
  [[nodiscard]] std::optional<std::filesystem::path> absolute_path() const = delete;

  // MARK: (log_instance) log_t
  [[nodiscard]] std::ostream& ostream() const;
  // HINT: not implemented yet.
  [[nodiscard]] bool running() const = delete;

  // MARK: (log_instance) manipulation

  void start() const;
  void stop() const;
  void flush() const;

private:
  log_t* log_;
};

}
