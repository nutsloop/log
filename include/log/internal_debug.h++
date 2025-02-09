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

  // MARK: (internal_debug) activation and session checks methods.

  /**
   * @brief Activates internal debug logging by setting up the necessary
   *        file resources and writing the session header.
   *
   * This method is a part of the debug logging system and is responsible for
   * ensuring that a file is created or associated to store log information. If
   * the temporary debug file has not been set up previously, this method will:
   * - Generate and set the debug file path.
   * - Create and initialize the file stream in `append` mode.
   * - Write the session header to the newly opened debug file.
   * It ensures that logs can be continuously written during the program's
   * session.
   *
   * The method is designed to write informative and detailed log information,
   * including debug paths and session state information, to help developers
   * trace internal debug states.
   *
   * This method is only compiled and executed when DEBUG_LOG is set to true.
   *
   * @note :
   * - This functionality is part of the nutsloop::nlog namespace and
   *    operates as a private debug handler for internal logging.
   * - This method is called only one time when instantiating the internal_debug
   * class.
   */
  void activate_();
  bool stream_is_open_() const;

  // MARK: (internal_debug) file management methods.
  void tmp_file_create_();
  [[nodiscard]] bool tmp_file_reassign_or_cleanup_();

  // MARK: (internal_debug) utility methods.
  [[nodiscard]] std::string generate_new_session_header_() const;
};

} // namespace nutsloop::nlog

#endif
