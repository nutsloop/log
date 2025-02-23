#include "log/internal_debug.h++"

#include "util/level.h++"
#include "util/shortened_path.h++"

#include <iostream>

namespace nutsloop::nlog {

#if DEBUG_LOG == true

std::ofstream &internal_debug::stream(const char *file, const int line_number,
                                      const Level c) const {

  if (this->stream_is_open_()) {
    return dynamic_cast<std::ofstream &>(*tmp_file_stream_
                                         << "[" << level(c) << "] "
                                         << "[" << shortened_path(file) << ':'
                                         << line_number << "] ");
  }

  throw std::runtime_error("internal_debug::stream() : stream is not open");
}

std::ofstream &internal_debug::stream() const {

  if (this->stream_is_open_()) {
    return *tmp_file_stream_;
  }

  throw std::runtime_error("internal_debug::stream() : stream is not open");
}

#endif

} // namespace nutsloop::nlog
