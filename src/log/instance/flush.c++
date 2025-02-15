#include "log.h++"

namespace nutsloop::nlog {

void instance::flush() const { log::flush(ident()); }

} // namespace nutsloop::nlog
