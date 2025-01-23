#include "log.h++"

#include <datetime.h++>

namespace nutsloop {

std::string log::generate_new_session_header_( const std::string& ident, const std::string& filename ) {
  return "\n"  "◀︎[" + ident + "]---------" + datetime() + "---------[" + filename + "]▶︎" + "\n" + "\n";
}

}
