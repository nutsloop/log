#include "log/internal_debug.h++"

#include "util/datetime.h++"
#include "util/shortened_path.h++"

#include "ansi.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

std::string internal_debug::generate_new_session_header_() const {

  return ansi("◀︎[log lib internal_debug]---------{}---------[{}]▶︎", datetime(),
              shortened_path(*this->tmp_file_path_.load()))
      .bold()
      .underline()
      .to_string();
}

#endif

} // namespace nutsloop::nlog
