#include "log.h++"

namespace nutsloop {

#if DEBUG_LOG

std::ofstream& log::debug_stream_( const char* file, const int line_number, const Level level ) {

  return dynamic_cast<std::ofstream&>( *debug_tmp_file_stream_ << "[" << level_( level ) << "] "
                                                               << "[" << shortened_path_( file ) << ':'
                                                               << line_number << "] " );
}

#endif

}
