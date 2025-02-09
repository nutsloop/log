#include "internal_debug.h++"

#include "shortened_path.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

void internal_debug::activate_() {

  // if the file is not set yet, set it up.
  // print the session header.
  if ( !tmp_file_already_set_.load() && tmp_file_stream_ == nullptr ) {
    tmp_file_create_();
    tmp_file_stream_ = std::make_unique<std::ofstream>( *tmp_file_path_.load(), std::ios::out | std::ios::app );
    *tmp_file_stream_
      << generate_new_session_header_()
      << "[INFO] [" << shortened_path( __FILE__ ) << ':' << __LINE__ << "] "
      << "internal_debug::file_is_active() called  ⇣" << '\n'
      << "  tmp_file_already_set_ was -> [ false ] now => [ true ]" << '\n'
      << "  tmp_file_stream_ -> [ false ] => [ true ]" << std::endl;

    return;
  }

  // if the file is already set, but the stream is not open, open it.
  // print the session header.
  if ( tmp_file_already_set_.load() && tmp_file_stream_ == nullptr ) {
    tmp_file_stream_ = std::make_unique<std::ofstream>( *tmp_file_path_.load(), std::ios::out | std::ios::app );
    *tmp_file_stream_
      << generate_new_session_header_()
      << "[INFO] [" << shortened_path( __FILE__ ) << ':' << __LINE__ << "] "
      << "internal_debug::file_is_active() called  ⇣" << '\n'
      << "  tmp_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
      << "  tmp_file_stream_ -> [ false ] => [ true ]" << std::endl;

    return;
  }

  // if the file is already set, and the stream is open, do nothing.
  // do NOT print the session header.
  if ( tmp_file_already_set_.load() && tmp_file_stream_ != nullptr && tmp_file_stream_->is_open() ) {
    *tmp_file_stream_
      << "[INFO] [" << shortened_path( __FILE__ ) << ':' << __LINE__ << "] "
      << "internal_debug::file_is_active() called  ⇣" << '\n'
      << "  tmp_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
      << "  tmp_file_stream_->is_open() -> [ true ]" << std::endl;
  }

}

#endif

}
