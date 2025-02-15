#include "log.h++"

namespace nutsloop {

void log::start(const std::string &ident) {

  if (!log_registry_->contains(ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, ERROR)
          << std::format("log::start([{}]) called ⇣", ident) << '\n'
          << std::format("  log identified with `{}` not found.", ident) << '\n';
    }
#endif

    return;
  }

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << '\n'
        << ansi("  log::start([{}]) called ⇣", ident).green().bold();
  }
#endif

  log_t *log_ident = &log_registry_->at(ident);
  const bool previous_running_status = log_ident->is_running();
  log_ident->set_running(true);

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream() << '\n'
                              << "    setting log_t.running_ ( "
                              << " was -> [ " << std::boolalpha << previous_running_status
                              << " ]" // previous
                              << " => now[ " << std::boolalpha << log_ident->is_running() << " ] )";
  }
#endif

  const bool previous_active_status = log_ident->settings.get_active();
  log_ident->settings.set_active(true);

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream() << '\n'
                              << "    setting log_t.settings.active_ ( "
                              << " was -> [ " << std::boolalpha << previous_active_status << " ]"
                              << " => now[ " << std::boolalpha << log_ident->settings.get_active()
                              << " ] )" << std::endl; // actual
  }
#endif
}

} // namespace nutsloop
