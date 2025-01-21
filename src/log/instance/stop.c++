#include "instance.h++"
#include "log.h++"

namespace nutsloop::nlog {

void instance::stop() const {
  log::stop( ident() );
}

}
