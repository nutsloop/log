#include "log.h++"

#include <iostream>

namespace nutsloop {

void log::mkdir_default_(const std::filesystem::path &path) {
  try {
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directory(path);
    } else {

#if DEBUG_LOG == true
      { // MARK (LOG) MUTEX LOCK
        std::shared_lock lock(mtx_);
        internal_debug_->stream(__FILE__, __LINE__, WARN)
            << "default directory '" << path << "' already exists."
            << std::endl;
      }
#endif
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Error creating directory '" << path << "': " << e.what()
              << "\n";
  }
}

} // namespace nutsloop
