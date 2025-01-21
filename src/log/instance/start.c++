#include "instance.h++"
#include "log.h++"

namespace nutsloop::nlog {

void instance::start() const {
  log::start( ident() );
}

}
