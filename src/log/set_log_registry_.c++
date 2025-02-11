#include "log.h++"

#include <uintptr.h++>

namespace nutsloop {

void log::set_log_registry_() {

  if (log_registry_ == nullptr) {
    log_registry_ = std::make_unique<log_registry_t>();

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock{mtx_};
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log::set_log_registry() called  ⇣" << '\n'
          << "  log_registry_ is nullptr, initialising..." << '\n'
          << std::format("  pointer @ -> 0x{:x}", uintptr(log_registry_))
          << '\n';
    }
#endif
    return;
  }

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock{mtx_};
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::set_log_registry() called  ⇣" << '\n'
        << std::format("  log_registry_ pointer @ -> 0x{:x}",
                       uintptr(log_registry_))
        << '\n';
  }
#endif
}

} // namespace nutsloop
