#include "log.h++"

#include <iostream>

namespace nutsloop {

std::optional<log_t*> log::null_stream_( const std::string& ident ){

  if ( !is_activated_() ) return std::nullopt;

  // Check if the log_registry is nullptr
  if (log_registry_ == nullptr) {
    std::cerr << "[ERROR] log_registry_ is nullptr\n";
    // IDEA: Consider throwing an exception or handling error specifically
    return std::nullopt;
  }

  // Check if the identifier exists in log_registry_
  if ( !log_registry_->contains( ident ) ) {
    std::cerr << "[ERROR] Invalid log identifier '" << ident << "'.\n";
    // IDEA: Consider throwing an exception or handling error specifically
    return std::nullopt;
  }

  log_t* log_ident = &log_registry_->at( ident );

  // Check if the log is active
  if ( !log_ident->settings.active ) {
    std::cerr << "[ERROR] Logging is disabled for identifier '" << ident << "'.\n";
    // IDEA: Consider throwing an exception or handling error specifically
    return std::nullopt;
  }

  // Check if the log is running
  if ( !log_ident->running ) {
    std::cerr << "[ERROR] Log is not running for identifier '" << ident << "'.\n";
    // IDEA: Consider throwing an exception or handling error specifically
    return std::nullopt;
  }

  // Check if the log file stream is open
  if ( !log_ident->stream.is_open() ) {
    std::cerr << "[ERROR] Log file stream is not open for identifier '" << ident << "'.\n";
    // IDEA: Consider throwing an exception or handling error specifically
    return std::nullopt;
  }

  return log_ident;
}

}
