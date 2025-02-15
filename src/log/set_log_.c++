#include "log.h++"

#include "util/uintptr.h++"

namespace nutsloop{

log_t* log::set_log_(log_settings_t* settings) {

  if (log_ == nullptr) {

    log_ = std::make_unique<log_t>(*settings);

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log_ is nullptr, construct..." << '\n'
          << std::format("pointer @ -> 0x{:x}", uintptr( log_ ))
          << std::endl;
    }
#endif
  }

  log_registry_->insert_or_assign(settings->get_ident(), std::move(*log_));

  log_.reset();

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log_ is being destroyed..." << '\n'
        << std::format("pointer @ -> 0x{:x}", uintptr( log_ ))
        << std::endl;
  }
#endif

  return &log_registry_->at(settings->get_ident());

}

}
