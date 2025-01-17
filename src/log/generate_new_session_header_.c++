#include "log.h++"

#include <sstream>

namespace nutsloop {

std::string log::generate_new_session_header_( const std::string& ident, const std::string& filename ) {
  return "\n"  "◀︎[" + ident + "]---------" + datetime_() + "---------[" + filename + "]▶︎" + "\n" + "\n";
}

std::string log::datetime_() {

  std::string time_now;

  try {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Use `std::put_time` for formatting
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%d-%m-%Y_%H:%M:%S");
    time_now = oss.str();
  } catch (const std::exception&) {
    time_now = "unknown-time";  // Fallback in case of formatting issues
  }

  if (DEBUG) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      if ( debug_tmp_file_stream_ != nullptr && debug_tmp_file_stream_->is_open() ) {
        *debug_tmp_file_stream_ << '\n' << "----------------------------------------------------------------------" << '\n';
        debug_stream_( __FILE__, __LINE__, INFO )
          << "datetime_() called ⇣" << '\n'
          << "now => [ " << time_now << " ]" << '\n'
          << "this is happening before new session header." << '\n'
          << "----------------------------------------------------------------------" << std::endl;
      }
    }
#endif
  }

  return time_now;
}

}
