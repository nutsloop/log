#include "log.h++"

#include <iostream>

namespace nutsloop {

void log::activate() {

  if (!is_set_called_()) {
    std::cerr << "log::activate() called before log::set()!" << std::endl;
    exit(1);
  }
  [[maybe_unused]]const bool previous_activate_status = activated_.exchange(true);

#if DEBUG_LOG == true
  {
    // MARK (LOG) MUTEX LOCK
    std::shared_lock lock{mtx_};
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::activate() called ⇣" << '\n'
        << "  activated_ ( "
        << " was -> [ " << std::boolalpha << previous_activate_status
        << " ]" // previous
        << " => now[ " << std::boolalpha << activated_ << " ] )"
        << std::endl; // actual
  }
#endif

  if (log_registry_ == nullptr) {
    log_registry_ = std::make_unique<log_registry_t>();

#if DEBUG_LOG == true
    {
      std::shared_lock lock{mtx_};
      const auto log_registry_address =
          reinterpret_cast<uintptr_t>(&log_registry_);
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log_registry_ is nullptr, creating..." << '\n'
          << std::format("pointer with address -> 0x{:x}", log_registry_address)
          << std::endl;
    }
#endif
  }
}

} // namespace nutsloop
