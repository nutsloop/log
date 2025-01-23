#include <iostream>

#include "log.h++"

namespace nutsloop {

void log::activate_stream_redirect() {

  [[maybe_unused]] const bool previous_stream_redirect_active = stream_redirect_active_.exchange( true );
  // Setup stream redirection only once
  if ( !stream_redirect_pointer_ ) {
    stream_redirect_pointer_ = std::make_unique<stream_redirect_>( std::cout, std::cerr );
  }
  if ( DEBUG ) {
#if DEBUG_LOG
    {
      // MARK (LOG) MUTEX LOCK
      std::shared_lock lock{ mtx_ };
      internal_debug_->file_is_active();
      internal_debug_->stream( __FILE__, __LINE__, INFO ) << "log::activate_stream_redirect() called ⇣" << '\n'
        << "  activated_ ( "
        << " was -> [ " << std::boolalpha << previous_stream_redirect_active << " ]" // previous
        << " => now[ " << std::boolalpha << stream_redirect_active_ << " ] )" << std::endl; // actual
    }
#endif
  }
}

}
