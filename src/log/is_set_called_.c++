#include "log.h++"

namespace nutsloop {

bool log::is_set_called_() {

#if DEBUG_LOG == true
  {
    // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    if (internal_debug_ == nullptr) {
      internal_debug_ = std::make_unique<nlog::internal_debug>();
    }

    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::is_set_called_() called ⇣" << '\n'
        << "  log has been set: [ " << std::boolalpha << set_has_been_called_
        << " ] " << std::endl; // actual
  }
#endif

  return log_registry_ != nullptr && set_has_been_called_.load();
}

} // namespace nutsloop
