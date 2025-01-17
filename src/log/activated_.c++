#include "log.h++"

namespace nutsloop {

bool log::is_activated_() {

  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );

      debug_stream_( __FILE__, __LINE__, INFO ) << "log::is_activated_() called ⇣" << '\n'
        << "  log system is active: [ " << std::boolalpha << activated_.load() << " ] " << std::endl; // actual
    }
#endif
  }

  return activated_.load();
}

}
