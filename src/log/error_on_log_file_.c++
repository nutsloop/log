#include "log.h++"

namespace nutsloop {

std::optional<std::string> log::error_on_log_file_( const log_t* log_ident ) {

  if (log_ident->stream.fail()) {
    const std::string error_ident =  "Error opening log file: " + log_ident->settings.filename + "\n";
    if (log_ident->stream.rdstate() & std::ios::failbit) {
      return error_ident + "Logical error on i/o operation\n";
    }
    if (log_ident->stream.rdstate() & std::ios::badbit) {
      return error_ident + "Read/writing error on i/o operation\n";
    }
    if (log_ident->stream.rdstate() & std::ios::eofbit) {
      return error_ident + "End-of-file reached prematurely\n";
    }
  }
  return std::nullopt;
}

}
