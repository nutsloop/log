#pragma once

namespace nutsloop {

enum Level { INFO, WARN, ERROR, NONE };

inline const char *level(const Level level) {

  switch (level) {
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
} // namespace nutsloop
