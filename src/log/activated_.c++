#include "log.h++"

namespace nutsloop {

bool log::is_activated_() {

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);

    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::is_activated_() called ⇣" << '\n'
        << "  log system is active: [ " << std::boolalpha << activated_.load()
        << " ] " << std::endl; // actual
  }
#endif

  return activated_.load();
}

} // namespace nutsloop
