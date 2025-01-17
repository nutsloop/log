#include "log.h++"

namespace nutsloop {

bool log::is_set_called_() {

  if ( DEBUG ) {
#if DEBUG_LOG
    {
      // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_stream_( __FILE__, __LINE__, INFO ) << "log::is_set_called_() called ⇣" << '\n'
        << "  log has been set: [ " << std::boolalpha << set_has_been_called_ << " ] " << std::endl; // actual
    }
#endif

  }

  return set_has_been_called_.load();
}

}
