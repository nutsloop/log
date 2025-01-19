#pragma once
#include <types.h++>

namespace nutsloop::nlog {

using namespace nutsloop::nlog::types;

class instance{
public:
  explicit instance( log_settings_t  settings );
  [[nodiscard]] std::string name() const;
private:
  log_settings_t settings_;
  std::string name_;
};

}
