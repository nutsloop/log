#include "internal_debug.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

bool internal_debug::stream_is_open() const {

  return this->tmp_file_stream_ != nullptr && this->tmp_file_stream_->is_open();
}

#endif

}
