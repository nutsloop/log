#pragma once
#include <types.h++>

namespace nutsloop::nlog {

using namespace nutsloop::nlog::types;

class instance{
public:
  explicit instance( log_t* log );
  [[nodiscard]] std::string ident() const;
  [[nodiscard]] std::ostream& ostream() const;
private:
  log_t* log_;
};

}
