#include "log.h++"

namespace nutsloop {

void log::set_internal_debug_() {
  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock(mtx_);
    if (internal_debug_ == nullptr) {
      internal_debug_ = std::make_unique<nlog::internal_debug>();
    }
  }
}

}
