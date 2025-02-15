#include "log/internal_debug.h++"

#include "util/shortened_path.h++"
#include "util/uintptr.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

void internal_debug::activate_() {

  // if the file is not set yet, set it up.
  // print the session header.
  if (!this->tmp_file_already_set_.load() &&
      this->tmp_file_stream_ == nullptr) {

    // create the file and set `true` tmp_file_path atomic variable.
    // do some reassignment or cleaning if debug files are found
    tmp_file_create_();

    // initialize the smart pointer and open file stream in `append` mode.
    this->tmp_file_stream_ = std::make_unique<std::ofstream>(
        *this->tmp_file_path_.load(), std::ios::out | std::ios::app);

    // stream to file the session header.
    stream() << '\n' << generate_new_session_header_() << '\n' << '\n';

    // stream the call to activate_ method.
    stream(__FILE__, __LINE__, INFO)
        << "internal_debug::activate_() called  ⇣" << '\n'
        << "  internal_debug::tmp_file_already_set_ was -> [ false ] now => [ "
           "true ]"
        << '\n'
        << std::format("  internal_debug::tmp_file_stream_ was -> [ nullptr ] "
                       "now => [(address "
                       "-> 0x{:x})]",
                       uintptr(this->tmp_file_stream_))
        << std::endl;
  }
}

#endif

} // namespace nutsloop::nlog
