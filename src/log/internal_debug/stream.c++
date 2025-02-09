#include "internal_debug.h++"

#include "../../include/util/level.h++"
#include "../../include/util/shortened_path.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

std::ofstream& internal_debug::stream( const char* file, const int line_number, const Level c ) const {

  return dynamic_cast<std::ofstream&>( *tmp_file_stream_ << "[" << level( c ) << "] "
                                                               << "[" << shortened_path( file ) << ':'
                                                               << line_number << "] " );
}

#endif

}
