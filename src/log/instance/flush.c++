#include "instance.h++"
#include "log.h++"

namespace nutsloop::nlog {

void instance::flush() const {
  log::flush( ident() );
}

}
