#include "log.h++"

namespace nutsloop {

bool log::is_stream_redirect_active_() {

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::is_stream_redirect_active() called ⇣" << '\n'
        << "  stream_redirect_active_: [ " << std::boolalpha
        << stream_redirect_active_ << " ] " << std::endl; // actual
  }
#endif

  return stream_redirect_active_.load();
}

} // namespace nutsloop
