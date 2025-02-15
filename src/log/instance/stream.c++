#include "log/instance.h++"

namespace nutsloop::nlog {

std::ostream& instance::ostream() const {
  return log_->stream;
}

}
