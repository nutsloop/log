#include "log.h++"

namespace nutsloop::nlog {

bool instance::is_open() const { return log::is_open( ident() ); }

} // namespace nutsloop::nlog
