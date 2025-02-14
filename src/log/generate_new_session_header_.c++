#include "log.h++"

#include "datetime.h++"
#include "shortened_path.h++"

#include <ansi.h++>

using namespace nutsloop::nansi;
namespace nutsloop {

std::string log::generate_new_session_header_(const std::string &ident,
                                              const std::string &filename) {
  return ansi("◀︎[{}]---------{}---------[{}]▶︎", ident, datetime(), shortened_path(filename))
      .bold()
      .underline()
      .to_string();
}

} // namespace nutsloop
