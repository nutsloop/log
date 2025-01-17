#include "log.h++"

namespace nutsloop {

std::ostream& log::stream( const char* ident, const char* file, const int line, const Level level /*= INFO*/ ) {

  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_file_is_active_();
      const std::string log_stream_signature = std::format("log::stream( ident[{}], file[{}], line[{}], level[{}] ) called ⇣", ident, file, line, level_( level ) );
      debug_stream_( __FILE__, __LINE__, INFO ) << log_stream_signature << std::endl;
    }
#endif
  }

  const std::optional<log_t*> null_stream = null_stream_( ident );
  log_t* log_ident = nullptr;
  if ( null_stream ) log_ident = null_stream.value();
  else return null_log_;

  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock( mtx_ );

    const std::string level_stream = std::format("[{}] ", level_( level ) );
    const std::string shortened_path_stream = strlen( file ) == 0 ? "" : std::format("{}", shortened_path_( file ) );
    const std::string line_stream = line == 0 ? "" : std::format("{}", line );
    const std::string location_stream = std::format( "[{}:{}]", shortened_path_stream, line_stream );
    // Write log prefix with level, file, and line
    log_ident->stream << level_stream << location_stream;
    return log_ident->stream;
  }
}

std::ostream& log::stream( const char* ident ) {
  if ( DEBUG ) {
#if DEBUG_LOG
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      debug_file_is_active_();
      const std::string log_stream_signature = std::format("log::stream( ident[{}] ) called ⇣", ident );
      debug_stream_( __FILE__, __LINE__, INFO ) << "log::stream() called ⇣" << std::endl;
    }
#endif
  }

  if ( const std::optional<log_t*> null_stream = null_stream_( ident ) ) return null_stream.value()->stream;

  return null_log_;

}

}
