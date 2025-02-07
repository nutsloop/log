#include "internal_debug.h++"

#include <datetime.h++>

namespace nutsloop::nlog {

#if DEBUG_LOG

std::string internal_debug::generate_new_session_header_() const {

  return "\n"  "◀︎[log lib internal_debug]---------" + datetime() + "---------[" + *this->tmp_file_path_.load() + "]▶︎" + "\n" + "\n";
}

#endif

}
