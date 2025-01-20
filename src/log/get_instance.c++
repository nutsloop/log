#include "log.h++"

namespace nutsloop {

#if EXPERIMENTAL_INSTANCE

std::unique_ptr<nlog::instance> log::get_instance( const std::string& ident ){

  // TODO: debug info
  if ( !log_registry_->contains( ident ) ) {
    throw std::runtime_error( "log::get_instance(): log instance not found" );
  }

  log_t* log_ident = &log_registry_->at( ident );
  return std::make_unique<nlog::instance>( nlog::instance( log_ident ) );
}

#endif

}
