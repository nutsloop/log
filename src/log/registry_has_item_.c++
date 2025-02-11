#include "log.h++"

namespace nutsloop {

bool log::registry_has_item_(std::string ident) {

  if (log_registry_->contains(ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      log_t *log_ident = &log_registry_->at(ident);

      internal_debug_->stream(__FILE__, __LINE__, WARN)
          << "log::registry_has_item_() called ⇣" << '\n'
          << std::format("  log_registry_ has -> [ {} ]",
                         log_ident->settings.get_ident())
          << '\n'
          << std::format("  log with ident_ -> [ {} ]",
                         log_ident->settings.get_ident())
          << '\n'
          << std::format("    is_active -> [ {} ]",
                         log_ident->settings.get_active() ? "true" : "false")
          << '\n'
          << std::format("    is_running -> [ {} ]",
                         log_ident->running ? "true" : "false")
          << '\n'
          << std::format("  use: `log::start( '{}' )` | `log::stop( '{}' )`",
                         ident, ident)
          << '\n'
          << std::format(
                 "  to change the running state for log with ident_ -> `{}`.",
                 ident)
          << std::endl;
    }
#endif

    return true;
  }
#if DEBUG_LOG == true
  {
    // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::registry_has_item_() called ⇣" << '\n'
        << std::format("  log_registry_ doesn't have -> [ {} ]", ident) << '\n'
        << " proceeding..." << std::endl;
  }
#endif
  return false;
}

} // namespace nutsloop
