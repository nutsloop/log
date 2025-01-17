#include "log.h++"

#include <sstream>

namespace nutsloop {

std::string log::shortened_path_( const std::filesystem::path& dir ) {
  std::vector<std::string> components;

  for ( const auto& part : dir ) {
    if ( std::string component = part.string(); !component.empty() && component != "/" ) {
      components.push_back( component );
    }
  }

  std::ostringstream shortened_dir;
  for ( size_t i = 0; i < components.size(); ++i ) {
    if ( i < components.size() - 1 ) {
      shortened_dir << components[ i ][ 0 ] << "/";
    }
    else {
      shortened_dir << components[ i ];
    }
  }

  return shortened_dir.str();
}

}
