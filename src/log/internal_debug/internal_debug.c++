#include "internal_debug.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

internal_debug::internal_debug() { this->activate_(); }

internal_debug::~internal_debug() {
  if (tmp_file_stream_ != nullptr) {
    stream() << '\n'
             << "*----------------*"
             << "closing internal_debug stream "
             << "*----------------*" << '\n';
    tmp_file_stream_->close();
    tmp_file_stream_->open(*tmp_file_path_.load(), std::ofstream::trunc);
    tmp_file_stream_->close();
  }
}

#endif

} // namespace nutsloop::nlog
