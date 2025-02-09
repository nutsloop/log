#include "log.h++"

#include <iostream>

namespace nutsloop {

void log::set(const log_settings_t &settings) {

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    // activating internal debug logging system.
    if (internal_debug_ == nullptr) {
      internal_debug_ = std::make_unique<nlog::internal_debug>();
      internal_debug_->file_is_active();
    }
  }
#endif

  if (log_registry_ == nullptr) {
    log_registry_ = std::make_unique<log_registry_t>();

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
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

  if (log_registry_ != nullptr && log_registry_->contains(settings.ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock<std::shared_mutex> lock(mtx_);
      log_t *log_ident = &log_registry_->at(settings.ident);
      if (internal_debug_->stream_is_open()) {

        internal_debug_->stream(__FILE__, __LINE__, WARN)
            << "log::set() called ⇣" << '\n'
            << std::format("  log_registry_ has -> [ {} ]",
                           log_ident->settings.ident)
            << '\n'
            << std::format("  log with ident -> [ {} ]",
                           log_ident->settings.ident)
            << '\n'
            << std::format("    is_active -> [ {} ]",
                           log_ident->settings.active ? "true" : "false")
            << '\n'
            << std::format("    is_running -> [ {} ]",
                           log_ident->running ? "true" : "false")
            << '\n'
            << std::format("  use: `log::start( '{}' )` | `log::stop( '{}' )`",
                           settings.ident, settings.ident)
            << '\n'
            << std::format(
                   "  to change the running state for log with ident -> `{}`.",
                   settings.ident)
            << std::endl;
      }
    }
#endif

    return;
  }

  // HINT: maybe find a purpose for this? :D
  if (is_set_called_()) {
  }

  const bool previous_set_status = set_has_been_called_.exchange(true);

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock<std::shared_mutex> lock(mtx_);
    internal_debug_->file_is_active();
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log::set() called ⇣" << '\n'
        << "  set_has_been_called_ ("
        << " was -> [ " << std::boolalpha << previous_set_status
        << " ]" // previous
        << " => now[ " << std::boolalpha << set_has_been_called_ << " ] )"
        << std::endl; // actual
  }
#endif

  uintptr_t log_address;
  if (log_ == nullptr) {

    log_ = std::make_unique<log_t>();

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      log_address = reinterpret_cast<uintptr_t>(&log_);
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log_ is nullptr, construct..." << '\n'
          << std::format("pointer with address -> 0x{:x}", log_address)
          << std::endl;
    }
#endif
  }

  log_->settings = settings;
  log_->stream = std::ofstream();
  log_->running = settings.active;

  log_registry_->insert_or_assign(settings.ident, std::move(*log_));

  log_.reset();

#if DEBUG_LOG == true
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    internal_debug_->stream(__FILE__, __LINE__, INFO)
        << "log_ is being destroyed..." << '\n'
        << std::format("pointer with address -> 0x{:x}", log_address)
        << std::endl;
  }
#endif

  log_t *log_ident = &log_registry_->at(settings.ident);

  if (!log_ident->settings.directory) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log custom directory is not set. " << '\n'
          << "  using default directory: " << nutsloop_logs_directory << '\n'
          << "  use `log::set( ident, settings )` to set a custom directory."
          << std::endl;
    }
#endif

    // make the default directories
    mkdir_default_(nutsloop_dir);
    mkdir_default_(nutsloop_logs_directory);
  }
  // TODO: handle custom log directory
  else {
  }

  bool renamed = false;
  const std::filesystem::path log_file_path =
      nutsloop_logs_directory / log_ident->settings.filename;
  if (std::filesystem::exists(log_file_path) &&
      std::filesystem::file_size(log_file_path) > LOG_MAX_SIZE) {
    const std::string backup_path =
        nutsloop_logs_directory / (log_ident->settings.filename + ".backup");
    if (log_ident->stream.is_open()) {
      log_ident->stream.close();
    }
    std::filesystem::rename(log_file_path, backup_path); // Archive existing log
    renamed = true;
  }

  log_ident->stream.open(log_file_path,
                         std::ios::out |
                             (renamed ? std::ios::trunc : std::ios::app));
  std::optional<std::string> log_file_stream_error =
      error_on_log_file_(log_ident);
  if (log_file_stream_error) {
    throw std::runtime_error(*log_file_stream_error);
  }

  // HINT: maybe find a purpose for this? :D
  // if ( is_stream_redirect_active_() ) {}

  log_ident->stream << std::unitbuf;

  // Add the default session header if the custom one has not been set.
  if (!log_ident->settings.session_header) {
    log_ident->stream << generate_new_session_header_(
        log_ident->settings.ident,
        log_file_path); // Add session header
  }
  // TODO: handle custom log header
  else {
  }
}

} // namespace nutsloop
