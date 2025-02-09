#pragma once

#include <chrono>
#include <iomanip>
#include <string>

namespace nutsloop {

inline std::string datetime() {

  std::string time_now;

  try {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Use `std::put_time` for formatting
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%d-%m-%Y_%H:%M:%S");
    time_now = oss.str();
  } catch (const std::exception &) {
    time_now = "unknown-time"; // Fallback in case of formatting issues
  }

  return time_now;
}
} // namespace nutsloop
