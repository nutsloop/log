#include "log.h++"

namespace nutsloop {

void log::flush(const std::string &ident) {

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << std::format("log::flush([{}]) called ⇣", ident) << '\n';
  }
#endif

  if (!log_registry_->contains(ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, ERROR)
          << std::format("log::flush([{}]) called ⇣", ident) << '\n'
          << std::format("  log identified with `{}` not found.", ident) << '\n';
    }
#endif

    return;
  }

  { // MARK (LOG) MUTEX LOCK
    log_t *log_ident = &log_registry_->at(ident);
    std::unique_lock lock(mtx_);
    if (log_ident->stream.is_open()) {
      log_ident->stream.close();
    }
    log_ident->stream.open(log_ident->settings.absolute_path(), std::ios::out | std::ios::trunc);
  }
}

} // namespace nutsloop
