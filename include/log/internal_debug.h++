#pragma once

// MARK: (log) internal_debug pre-processor
#ifndef DEBUG_LOG
// HINT: default value if not defined elsewhere
#define DEBUG_LOG true

#endif

#if DEBUG_LOG == true

#warning "DEBUG_LOG is enabled"

#include "../types.h++"
#include "../util/level.h++"

#include <atomic>
#include <fstream>
#include <memory>

namespace nutsloop::nlog {

using namespace nlog::types;

class internal_debug {

public:
  internal_debug();
  ~internal_debug();
  std::ofstream &stream(const char *file, int line_number = 0,
                        Level c = INFO) const;

private:
  std::atomic<bool> tmp_file_already_set_{false};
  std::atomic<std::string *> tmp_file_path_{};
  std::unique_ptr<std::ofstream> tmp_file_stream_{nullptr};

  void activate_();
  [[nodiscard]] bool tmp_file_reassign_or_cleanup_();
  [[nodiscard]] std::string generate_new_session_header_() const;
  void tmp_file_create_();

  bool stream_is_open_() const;
};

} // namespace nutsloop::nlog

#endif
