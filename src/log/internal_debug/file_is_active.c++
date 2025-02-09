#include "internal_debug.h++"

#include "../../include/util/shortened_path.h++"

namespace nutsloop::nlog {

#if DEBUG_LOG == true

  void internal_debug::file_is_active() {

    if ( tmp_file_already_set_.load() == false && tmp_file_stream_ == nullptr ) {
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

    if ( tmp_file_already_set_.load() == true && tmp_file_stream_ == nullptr ) {
      tmp_file_stream_ = std::make_unique<std::ofstream>( *tmp_file_path_.load(), std::ios::out | std::ios::app );
      *tmp_file_stream_
        << generate_new_session_header_()
        << "[INFO] [" << shortened_path( __FILE__ ) << ':' << __LINE__ << "] "
        << "internal_debug::file_is_active() called  ⇣" << '\n'
        << "  tmp_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
        << "  tmp_file_stream_ -> [ false ] => [ true ]" << std::endl;

      return;
    }

    if ( tmp_file_already_set_.load() == true && tmp_file_stream_ != nullptr && tmp_file_stream_->is_open() ) {
      *tmp_file_stream_
        << "[INFO] [" << shortened_path( __FILE__ ) << ':' << __LINE__ << "] "
        << "internal_debug::file_is_active() called  ⇣" << '\n'
        << "  tmp_file_already_set_ was -> [ true ] now => [ true ]" << '\n'
        << "  tmp_file_stream_->is_open() -> [ true ]" << std::endl;
    }

  }

#endif

}
