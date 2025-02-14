#pragma once

#include <array>
#include <format>
#include <string>
#include <string_view>

namespace nutsloop::nansi {
class ansi {
public:
  // MARK (ansi) constructors
  ansi() = default;
  explicit ansi(const std::string_view text) : text_(text) {}
  template <typename... Args> explicit ansi(std::format_string<Args...> fmt, Args &&...args) {
    text_ = std::vformat(fmt.get(), std::make_format_args(args...));
  }
  ~ansi() = default;

  // MARK (ansi) red
  [[nodiscard]] ansi red() const { return ansi("\033[31m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_red() const { return ansi("\033[91m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_red() const { return ansi("\033[41m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_red() const { return ansi("\033[101m" + text_ + "\033[0m"); }
  // MARK (ansi) green
  [[nodiscard]] ansi green() const { return ansi("\033[32m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_green() const { return ansi("\033[92m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_green() const { return ansi("\033[42m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_green() const {
    return ansi("\033[102m" + text_ + "\033[0m");
  }
  // MARK (ansi) yellow
  [[nodiscard]] ansi yellow() const { return ansi("\033[33m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_yellow() const { return ansi("\033[93m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_yellow() const { return ansi("\033[43m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_yellow() const {
    return ansi("\033[103m" + text_ + "\033[0m");
  }
  // MARK (ansi) blue
  [[nodiscard]] ansi blue() const { return ansi("\033[34m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_blue() const { return ansi("\033[94m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_blue() const { return ansi("\033[44m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_blue() const {
    return ansi("\033[104m" + text_ + "\033[0m");
  }
  // MARK (ansi) magenta
  [[nodiscard]] ansi magenta() const { return ansi("\033[35m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_magenta() const { return ansi("\033[95m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_magenta() const { return ansi("\033[45m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_magenta() const {
    return ansi("\033[105m" + text_ + "\033[0m");
  }
  // MARK (ansi) cyan
  [[nodiscard]] ansi cyan() const { return ansi("\033[36m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_cyan() const { return ansi("\033[96m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_cyan() const { return ansi("\033[46m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_cyan() const {
    return ansi("\033[106m" + text_ + "\033[0m");
  }
  // MARK (ansi) gray|white
  [[nodiscard]] ansi white() const { return ansi("\033[37m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_white() const { return ansi("\033[97m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi background_white() const { return ansi("\033[47m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi bright_background_white() const {
    return ansi("\033[107m" + text_ + "\033[0m");
  }
  // MARK (ansi) rgb
  [[nodiscard]] ansi rgb(const std::array<int, 3> &rgb) const {
    return ansi("\033[38;2;" + std::to_string(rgb[0]) + ";" + std::to_string(rgb[1]) + ";" +
                std::to_string(rgb[2]) + "m" + text_ + "\033[0m");
  }
  [[nodiscard]] ansi background_rgb(const std::array<int, 3> &rgb) const {
    return ansi("\033[48;2;" + std::to_string(rgb[0]) + ";" + std::to_string(rgb[1]) + ";" +
                std::to_string(rgb[2]) + "m" + text_ + "\033[0m");
  }
  // MARK (ansi) 256 color code
  [[nodiscard]] ansi color(const int color_code) const {
    return ansi("\033[38;5;" + std::to_string(color_code) + "m" + text_ + "\033[0m");
  }
  [[nodiscard]] ansi background_color256(const int color_code) const {
    return ansi("\033[48;5;" + std::to_string(color_code) + "m" + text_ + "\033[0m");
  }

  [[nodiscard]] ansi bold() const { return ansi("\033[1m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi underline() const { return ansi("\033[4m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi blink() const { return ansi("\033[5m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi reverse() const { return ansi("\033[7m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi hidden() const { return ansi("\033[8m" + text_ + "\033[0m"); }
  [[nodiscard]] ansi reset() const { return ansi("\033[0m" + text_ + "\033[0m"); }

  // MARK (ansi) operators
  friend std::ostream &operator<<(std::ostream &os, const ansi &c_str){
    os << c_str.text_;
    return os;
  }

  explicit operator std::string() const { return text_; }

  template <typename... Args>
  [[nodiscard]] ansi operator()(std::format_string<Args...> fmt, Args &&...args) {
    return ansi(std::vformat(fmt.get(), std::make_format_args(args...)));
  }

  // MARK (ansi) conversion methods
  [[nodiscard]] std::string to_string() const { return text_; }

private:
  std::string text_;
};

inline ansi operator""_(const char *str, std::size_t) { return ansi(std::string_view(str)); }

} // namespace nutsloop::nansi
