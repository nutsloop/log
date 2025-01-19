#include "log.h++"

namespace nutsloop {

#if EXPERIMENTAL_INSTANCE

std::unique_ptr<nlog::instance> log::set_instance( const log_settings_t& settings ){
  return std::make_unique<nlog::instance>( nlog::instance( settings ) );
}

#endif

}
