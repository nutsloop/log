#include "log.h++"

namespace nutsloop {

void log::set_called_() {

  [[maybe_unused]]const bool previous_set_status = set_has_been_called_.exchange(true);
#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::set_called_() called ⇣" << '\n'
        << "  set_has_been_called_ ("
        << " was -> [ " << std::boolalpha << previous_set_status
        << " ]" // previous
        << " => now[ " << std::boolalpha << set_has_been_called_ << " ] )"
        << std::endl; // actual
  }
#endif
}

}
