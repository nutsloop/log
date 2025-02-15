#include "log.h++"

namespace nutsloop {

void log::close(const std::string &ident) {

  if (!log_registry_->contains(ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, ERROR)
          << '\n'
          << ansi("  log::close([{}]) called ⇣", ident).red().bold() << '\n'
          << ansi("    log identified with `{}` not found.", ident).red() << '\n'
          << "    throw std::invalid_argument" << '\n';
    }
#endif

    throw std::invalid_argument("log identified with `" + ident + "` not found.");
  }

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << '\n'
        << ansi("  log::close([{}]) called ⇣", ident).green() << '\n';
  }
#endif

  log_t *log_ident = &log_registry_->at(ident);
  // first, we set the log_ident->running_ to false
  if (log_ident->is_running()) {
    stop(ident);
  }
  // then, we close the log_ident->stream
  if (log_ident->stream.is_open()) {
    log_ident->stream.close();

    // TODO: do something with this thang.
    if (log_ident->stream.fail()) {}
  }
}

} // namespace nutsloop
