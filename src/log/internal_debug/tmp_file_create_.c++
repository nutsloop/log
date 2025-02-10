#include "internal_debug.h++"

#include "../../include/util/datetime.h++"

#include <iostream>
#include <unistd.h>

namespace nutsloop::nlog {

#if DEBUG_LOG == true

void internal_debug::tmp_file_create_() {

  // first a bit of cleanup or reassign
  if (tmp_file_reassign_or_cleanup_()) {
    // if we reassigned, we're done.
    return;
  }

  std::string tmp_path =
      std::format("/var/tmp/{}_nutsloop_logXXXXXX", datetime());
  const int fd = mkstemp(tmp_path.data()); // Secure and unique
  if (fd == -1) {
    perror("mkstemp failed");
    exit(EXIT_FAILURE);
  }
  // #include <unistd.h>
  if (::close(fd) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }

  // Store the temp file path safely
  tmp_file_path_.store(new std::string(tmp_path)); // Atomic update
  tmp_file_already_set_.store(true);
  std::cerr << "temporary debug file created: " << *tmp_file_path_.load()
            << '\n';
}

#endif

} // namespace nutsloop::nlog
