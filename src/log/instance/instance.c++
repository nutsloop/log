#include <utility>

#include "instance.h++"

namespace nutsloop::nlog {

instance::instance( log_settings_t settings ) : settings_{std::move( settings )} {

  this->name_ = settings_.ident;
}

std::string instance::name() const {
  return name_;
}

}
