#include "log.h++"

namespace nutsloop {

void log::start( const std::string& ident ) {

  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_file_is_active_();
      debug_stream_( __FILE__, __LINE__, INFO )
        << std::format( "log::start([{}]) called ⇣", ident ) << '\n';
    }
#endif
  }

  if ( ! log_registry_->contains( ident ) ) {
    if ( DEBUG ) {
#if DEBUG_LOG
      { // MARK (LOG) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        debug_stream_( __FILE__, __LINE__, ERROR )
          << std::format( "log::start([{}]) called ⇣", ident ) << '\n'
          << std::format( "  log identified with `{}` not found.", ident ) << '\n';
      }
#endif
    }

    return;
  }

  log_t* log_ident = &log_registry_->at( ident );
  const bool previous_running_status = log_ident->running;
  log_ident->running = true;
  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_stream_( __FILE__, __LINE__, INFO )
        << std::format( "log::start([{}]) called ⇣", ident ) << '\n'
        << "  setting log_t.running ( "
        << " was -> [ " << std::boolalpha << previous_running_status << " ]" // previous
        << " => now[ " << std::boolalpha << log_ident->running << " ] )" << std::endl; // actual
    }
#endif
  }
}

}
