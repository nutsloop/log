#include "log.h++"

namespace nutsloop {

bool log::is_set_called_() {

  if ( DEBUG ) {
#if DEBUG_LOG
    {
      // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      if ( internal_debug_ == nullptr ) {
        internal_debug_ = std::make_unique<nlog::internal_debug>();
      }
      internal_debug_->file_is_active();
      internal_debug_->stream( __FILE__, __LINE__, INFO ) << "log::is_set_called_() called ⇣" << '\n'
        << "  log has been set: [ " << std::boolalpha << set_has_been_called_ << " ] " << std::endl; // actual
    }
#endif

  }

  return set_has_been_called_.load();
}

}
