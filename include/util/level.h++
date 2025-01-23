#pragma once

#include "types.h++"

using namespace nutsloop::nlog::types;

inline const char* level( const Level level ) {

  switch ( level ) {
  case INFO:
    return "INFO";
  case WARN:
    return "WARN";
  case ERROR:
    return "ERROR";
  case  NONE:
    return "";
  }

  return "UNKNOWN";
}
