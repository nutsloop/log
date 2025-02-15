#include "log.h++"

namespace nutsloop {

std::filesystem::path log::get_absolute_path(const std::string &ident) {
  if (!log_registry_->contains(ident)) {

#if DEBUG_LOG == true
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock lock(mtx_);
      internal_debug_->stream(__FILE__, __LINE__, ERROR)
          << ansi("log::get_absolute_path([{}]) called ⇣", ident).red().bold() << '\n'
          << ansi("    log identified with `{}` not found.", ident).red() << '\n'
          << "    throw std::invalid_argument" << '\n';
    }
#endif

    throw std::invalid_argument("log identified with `" + ident + "` not found.");
  }

  return log_registry_->at(ident).settings.absolute_path();
}

}
