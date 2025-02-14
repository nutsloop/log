#pragma once

#include "ansi.h++"
using namespace nutsloop::nansi;

namespace nutsloop {

enum Level { INFO, WARN, ERROR, NONE };

inline std::string level(const Level level) {

  switch (level) {
  case INFO:
    return "INFO"_.green().to_string();
  case WARN:
    return "WARN"_.yellow().to_string();
  case ERROR:
    return "ERROR"_.red().to_string();
  case NONE:
    return "";
  }

  return "UNKNOWN";
}
} // namespace nutsloop
