#include "log/internal_debug.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

bool internal_debug::stream_is_open_() const {

  return this->tmp_file_already_set_.load() &&
         this->tmp_file_stream_ != nullptr && this->tmp_file_stream_->is_open();
}

#endif

} // namespace nutsloop::nlog
