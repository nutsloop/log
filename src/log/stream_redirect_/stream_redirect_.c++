#include "log.h++"

#include <iostream>

namespace nutsloop {

log::stream_redirect_::stream_redirect_( std::ostream& stream_out, std::ostream& stream_err ) :
    original_cout_( stream_out.rdbuf() ), original_cerr_( stream_err.rdbuf() ) {

  this->cout_file_.open( nutsloop::nutsloop_logs_directory / "nutsloop_cout.log", std::ios::out | std::ios::app );
  this->cerr_file_.open( nutsloop::nutsloop_logs_directory / "nutsloop_cerr.log", std::ios::out | std::ios::app );

  if ( !this->cout_file_.is_open() ) {
    if ( DEBUG ) {
#if DEBUG_LOG
      { // MARK (LOG) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        debug_stream_( __FILE__, __LINE__, ERROR )
          << "log::stream_redirect_::stream_redirect_() failed to open cout<<log file '"
          << nutsloop::nutsloop_logs_directory / "nutsloop_cout.log" << "'" << std::endl;
      }
#endif
    }
    return;
  }

  if ( !this->cerr_file_.is_open() ) {
    if ( DEBUG ) {
#if DEBUG_LOG
      { // MARK (LOG) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        debug_stream_( __FILE__, __LINE__, ERROR )
          << "log::stream_redirect_::stream_redirect_() failed to open cerr<<log file '"
          << nutsloop::nutsloop_logs_directory / "nutsloop_cerr.log" << "'" << std::endl;
      }
#endif
    }
    return;
  }

  this->cout_file_ << std::unitbuf;
  this->cerr_file_ << std::unitbuf;

  stream_out.rdbuf( this->cout_file_.rdbuf() );
  stream_err.rdbuf( this->cerr_file_.rdbuf() );
}

log::stream_redirect_::~stream_redirect_() {
  this->cout_file_.close();
  this->cerr_file_.close();
  std::cout.rdbuf( original_cout_ );
  std::cerr.rdbuf( original_cerr_ );
}

} // namespace nutsloop
