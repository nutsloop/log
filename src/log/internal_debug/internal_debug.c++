#include "internal_debug.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

internal_debug::~internal_debug() {
  if (tmp_file_stream_ != nullptr) {
    *tmp_file_stream_ << "*----------------* closing internal_debug stream "
                         "*----------------*";
    tmp_file_stream_->close();
    tmp_file_stream_->open(*tmp_file_path_.load(), std::ofstream::trunc);
    tmp_file_stream_->close();
  }
}

#endif

} // namespace nutsloop::nlog
