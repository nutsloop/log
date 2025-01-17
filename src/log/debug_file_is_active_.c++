#include "log.h++"

namespace nutsloop {

#if DEBUG_LOG

void log::debug_file_is_active_() {

  if ( tmp_debug_file_already_set_.load() == false && debug_tmp_file_stream_ == nullptr ) {
    debug_tmp_file_create_();
    debug_tmp_file_stream_ = std::make_unique<std::ofstream>( *tmp_debug_file_path_.load(), std::ios::out | std::ios::app );
    *debug_tmp_file_stream_
      << generate_new_session_header_( "debug_log", *tmp_debug_file_path_.load() )
      << "[INFO] [" << shortened_path_( __FILE__ ) << ':' << __LINE__ << "] "
      << "log::debug_file_is_active_() called  ⇣" << '\n'
      << "  tmp_debug_file_already_set_ was -> [ false ] now => [ true ]" << '\n'
      << "  debug_tmp_file_stream_ -> [ false ] => [ true ]" << std::endl;

    return;
  }

  if ( tmp_debug_file_already_set_.load() == true && debug_tmp_file_stream_ == nullptr ) {
    debug_tmp_file_stream_ = std::make_unique<std::ofstream>( *tmp_debug_file_path_.load(), std::ios::out | std::ios::app );
    *debug_tmp_file_stream_
      << generate_new_session_header_( "debug_log", *tmp_debug_file_path_.load() )
      << "[INFO] [" << shortened_path_( __FILE__ ) << ':' << __LINE__ << "] "
      << "log::debug_file_is_active_() called  ⇣" << '\n'
      << "  tmp_debug_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
      << "  debug_tmp_file_stream_ -> [ false ] => [ true ]" << std::endl;

    return;
  }

  if ( tmp_debug_file_already_set_.load() == true && debug_tmp_file_stream_ != nullptr && debug_tmp_file_stream_->is_open() ) {
    *debug_tmp_file_stream_
      << "[INFO] [" << shortened_path_( __FILE__ ) << ':' << __LINE__ << "] "
      << "log::debug_file_is_active_() called  ⇣" << '\n'
      << "  tmp_debug_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
      << "  debug_tmp_file_stream_->is_open() -> [ true ]" << std::endl;
  }

}

#endif

}
