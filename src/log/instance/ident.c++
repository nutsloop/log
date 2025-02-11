#include "instance.h++"

namespace nutsloop::nlog {

std::string instance::ident() const {
  return log_->settings.get_ident();
}

}
