#include "internal_debug.h++"

#include "../../include/util/level.h++"
#include "../../include/util/shortened_path.h++"

#include <iostream>

namespace nutsloop::nlog {

#if DEBUG_LOG == true

std::ofstream& internal_debug::stream( const char* file, const int line_number, const Level c ) const {

  if (this->stream_is_open_()) {
    return dynamic_cast<std::ofstream&>( *tmp_file_stream_ << "[" << level( c ) << "] "
                                                             << "[" << shortened_path( file ) << ':'
                                                             << line_number << "] " );
  }

  throw std::runtime_error( "internal_debug::stream() : stream is not open" );

}

#endif

}
