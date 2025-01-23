#pragma once

// MARK: (log) internal_debug pre-processor
#ifndef DEBUG_LOG
// HINT: default value if not defined elsewhere
#define DEBUG_LOG true

#endif

#if DEBUG_LOG == true

#warning "DEBUG_LOG is enabled"

#include <internal_debug.h++>

constexpr bool DEBUG = true;

#define LOG log::stream( "log", __FILE__, __LINE__, nutsloop::Level::INFO )
#define LOG_WARN log::stream( "log", __FILE__, __LINE__, nutsloop::Level::WARN )
#define LOG_ERROR log::stream( "log", __FILE__, __LINE__, nutsloop::Level::ERROR )

#else

constexpr bool DEBUG = false;

// Mock macros to provide `<<` compatibility
#include <sstream>

#define LOG std::ostream(nullptr) // No-op stream
#define LOG_WARN std::ostream(nullptr) // No-op stream
#define LOG_ERROR std::ostream(nullptr) // No-op stream

#endif

#include "types.h++"

#include <atomic>
#include <fstream>
#include <memory>

namespace nutsloop::nlog {

using namespace nlog::types;

class internal_debug {

public:
  internal_debug() = default;
  ~internal_debug() = default;

  void file_is_active();
  std::ofstream& stream( const char* file, int line_number = 0, Level c = INFO ) const;
  bool stream_is_open() const;

private:
  std::atomic<bool> tmp_file_already_set_{ false };
  std::atomic<std::string*> tmp_file_path_{};
  std::unique_ptr<std::ofstream> tmp_file_stream_{ nullptr };
  [[nodiscard]] bool tmp_file_reassign_or_cleanup_();
  [[nodiscard]] std::string generate_new_session_header_() const;
  void tmp_file_create_();
};

}
