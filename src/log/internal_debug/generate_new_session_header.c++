#include "internal_debug.h++"

#include "../../include/util/datetime.h++"
#include "../../include/util/shortened_path.h++"

#include "ansi.h++"

using namespace nutsloop::nansi;
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
