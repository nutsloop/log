#include "log.h++"

namespace nutsloop::nlog {

void instance::close() const { log::close(ident()); }

} // namespace nutsloop::nlog
