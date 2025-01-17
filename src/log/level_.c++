#include "log.h++"

namespace nutsloop {

const char* log::level_( const Level level ) {

  switch ( level ) {
  case INFO:
    return "INFO";
  case WARN:
    return "WARN";
  case ERROR:
    return "ERROR";
  case NONE:
    return "";
  }

  return "UNKNOWN";
}

}
