#include "log.h++"

#include <iostream>


namespace nutsloop {

void log::set(log_settings_t &settings) {

  // GOOD internal debug logging system.

  // Initialise an internal debug logging system globally accessible.
  // This comes first so that if the internal debug logging system is
  // initialised.
#if DEBUG_LOG == true
  set_internal_debug_();
#endif

  // GOOD: internal debug logging system.

  // this method has been called so it is possible to call all the public
  // methods this method can be called again to set other logs.
  set_called_();

  // ONGOING: setting up the registry

  set_log_registry_();

  if (registry_has_item_(settings.get_ident())) {
    return;
  }

  // ONGOING: setting up the registry

  // ONGOING: setting up the log

  log_t *log_ident = set_log_(&settings);

  if (!log_ident->settings.get_directory()) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, INFO)
          << "log custom directory is not set. " << '\n'
          << "  using default directory: " << nutsloop_logs_directory << std::endl;
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
      nutsloop_logs_directory / log_ident->settings.get_filename();
  if (std::filesystem::exists(log_file_path) &&
      std::filesystem::file_size(log_file_path) > LOG_MAX_SIZE) {
    const std::string backup_path =
        nutsloop_logs_directory / (log_ident->settings.get_filename() + ".backup");
    if (log_ident->stream.is_open()) {
      log_ident->stream.close();
    }
    std::filesystem::rename(log_file_path, backup_path); // Archive existing log
    renamed = true;
  }

  log_ident->stream.open(log_file_path,
                         std::ios::out | (renamed ? std::ios::trunc : std::ios::app));
  std::optional<std::string> log_file_stream_error = error_on_log_file_(log_ident);
  if (log_file_stream_error) {
    throw std::runtime_error(*log_file_stream_error);
  }

  // HINT: maybe find a purpose for this? :D
  // if ( is_stream_redirect_active_() ) {}

  log_ident->stream << std::unitbuf;

  // Add the default session header if the custom one has not been set.
  if (!log_ident->settings.get_session_header()) {
    log_ident->stream << '\n'
                      << generate_new_session_header_(log_ident->settings.get_ident(),
                                                      log_file_path) // Add session header
                      << '\n'
                      << '\n';
  }
  // TODO: handle custom log header
  else {
  }

  // ONGOING: setting up the log
}

} // namespace nutsloop
