#include "log.h++"

namespace nutsloop {

bool log::full_running( const std::string& ident){
  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_file_is_active_();
      debug_stream_( __FILE__, __LINE__, INFO )
        << std::format( "log::stream(ident[{}]) called ⇣", ident) << std::endl;
    }
#endif
  }

  if ( !is_activated_() ) return false;

  if (log_registry_->contains( ident ) ) {
    const log_t* log_ident = &log_registry_->at( ident );
    if ( log_ident->running && log_ident->stream.is_open() ) {
      return true;
    }
    if ( log_ident->running && !log_ident->stream.is_open() ) {
      return false;
    }
    if ( !log_ident->running && !log_ident->stream.is_open() ) {
      return false;
    }
  }

  return false;
}

}
