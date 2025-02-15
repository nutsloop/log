#include "log.h++"

#include "util/datetime.h++"
#include "util/shortened_path.h++"

#include <ansi.h++>

namespace nutsloop {

std::string log::generate_new_session_header_(const std::string &ident,
                                              const std::string &filename) {
  return ansi("◀︎[{}]---------{}---------[{}]▶︎", ident, datetime(), shortened_path(filename))
      .bold()
      .underline()
      .to_string();
}

} // namespace nutsloop
