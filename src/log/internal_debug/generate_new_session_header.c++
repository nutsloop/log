#include "internal_debug.h++"

#include "../../include/util/datetime.h++"
#include "../../include/util/shortened_path.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

std::string internal_debug::generate_new_session_header_() const {

  return std::format("◀︎[log lib internal_debug]---------{}---------[{}]▶︎",
                     datetime(), shortened_path(*this->tmp_file_path_.load()));
}

#endif

} // namespace nutsloop::nlog
