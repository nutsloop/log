#include "log.h++"

namespace nutsloop {

void log::deactivate() {

  const bool previous_activate_status = activated_.exchange( false );
  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::unique_lock lock( mtx_ );
      internal_debug_->file_is_active();
      internal_debug_->stream( __FILE__, __LINE__, INFO ) << "log::deactivate() called ⇣" << '\n'
        << "  activated_ ("
        << " was -> [ " << std::boolalpha << previous_activate_status << " ]" // previous
        << " => now[ " << std::boolalpha << activated_ << " ] )" << std::endl; // actual
    }
#endif
  }
}

}
