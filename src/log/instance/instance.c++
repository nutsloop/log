#include <utility>

#include "instance.h++"

namespace nutsloop::nlog {

instance::instance( log_t* log ) : log_{ log } {
}

std::string instance::ident() const {
  return log_->settings.ident;
}

std::ostream& instance::ostream() const {

  return log_->stream;
}


}
