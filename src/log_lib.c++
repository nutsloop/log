#include "log_lib.h++"

#include <fstream>
#include <iostream>
#include <sstream>

namespace nutsloop {

bool log::logs_disabled_ = false;
std::unique_ptr<log::stream_redirect_> log::stream_redirect_pointer_ = nullptr;
std::unique_ptr<std::unordered_map<std::string, std::tuple<bool, std::ofstream>>> log::log_files_ = nullptr;
std::shared_mutex log::log_mtx_;
log::null_logger_ log::null_log_;

void log::setup( const std::string& ident, const std::string& filename, const bool enabled /*=true*/ ) {

  if ( logs_disabled_ )
    return;

  {
    std::unique_lock lock( log_mtx_ );

    // Initialize the log files map if needed
    if ( !log_files_ ) {
      log_files_ = std::make_unique<std::unordered_map<std::string, std::tuple<bool, std::ofstream>>>();
    }

    log_files_->emplace( ident, std::make_tuple( enabled, std::ofstream() ) );
    auto& [ enabled_file, log_file ] = log_files_->at( ident );

    if ( !enabled_file ) {
      return;
    }

    // Lambda for directory creation
    const auto create_directory_ = []( const std::filesystem::path& dir_ ) {
      try {
        if ( !std::filesystem::exists( dir_ ) ) {
          std::filesystem::create_directory( dir_ );
        }
      }
      catch ( const std::filesystem::filesystem_error& e ) {
        std::cerr << "Error creating directory '" << dir_ << "': " << e.what() << "\n";
      }
    };

    // Create the necessary directories
    create_directory_( nutsloop_dir );
    create_directory_( nutsloop_logs_directory );

    const std::filesystem::path log_file_path_ = nutsloop_logs_directory / filename;
    if ( constexpr size_t max_size = 10 * 1024 * 1024;
         std::filesystem::exists( log_file_path_ ) && std::filesystem::file_size( log_file_path_ ) > max_size ) {
      const std::string backup_path = nutsloop_logs_directory / ( filename + ".backup" );
      log_file.close();
      std::filesystem::rename( log_file_path_, backup_path ); // Archive existing log
      log_file.open( log_file_path_, std::ios::out | std::ios::trunc ); // Start fresh
    }
    else {
      // Open the log file in `append` mode
      log_file.open( log_file_path_, std::ios::out | std::ios::app );
    }

    if ( !log_file.is_open() ) {
      std::cerr << "Failed to open log file: " << filename << "\n";
      log_files_->erase( ident );
      return;
    }

    // Setup stream redirection only once
    if ( !stream_redirect_pointer_ ) {
      stream_redirect_pointer_ = std::make_unique<stream_redirect_>( std::cout, std::cerr, log_file.rdbuf() );
    }

    log_file << std::unitbuf; // Ensure immediate flushing
    log_file << generate_new_session_header_( filename ); // Add session header
  }
}

void log::cleanup() {

  if ( logs_disabled_ )
    return;

  {
    std::unique_lock lock( log_mtx_ );

    std::cerr << "log::cleanup() called\n";

    if ( log_files_ ) {
      for ( auto& [ ident, log_file_opt ] : *log_files_ ) {
        std::cerr << "Cleaning log: " << ident << "\n";
        if ( auto& [ enabled_file, log_file ] = log_file_opt; enabled_file && log_file.is_open() ) {
          log_file.close();
        }
      }
      log_files_.reset();
    }
    else {
      std::cerr << "log_files_ is null\n";
    }

    stream_redirect_pointer_.reset();
    // std::cerr << "log::cleanup() finished\n";
  }
}

std::ostream& log::stream( const char* ident, const char* file, const int line, const char level /*= 'I'*/ ) {

  if ( logs_disabled_ )
    return null_log_;

  {
    std::shared_lock lock( log_mtx_ );

    // Check if the identifier exists in log_files_
    if ( !log_files_ || !log_files_->contains( ident ) ) {
      std::cerr << "Warning: Invalid log identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    auto& [ enabled, stream ] = log_files_->at( ident );

    if ( !enabled ) {
      std::cerr << "Warning: Logging is disabled for identifier '" << ident << "'.\n";
      return null_log_;
    }

    if ( !stream.is_open() ) {
      std::cerr << "Warning: Log file stream is not open for identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    // Lambda for determining log level
    const auto get_log_level_ = [ &level ]() -> const char* {
      switch ( level ) {
      case 'I':
        return "INFO";
      case 'W':
        return "WARN";
      case 'E':
        return "ERROR";
      default:
        return "UNKNOWN";
      }
    };

    // Write log prefix with level, file, and line
    stream << "[" << get_log_level_() << "] "
           << "[" << shortened_path_( file ) << ":" << line << "] ";
    return stream;
  }
}

std::string log::shortened_path_( const std::filesystem::path& dir ) {
  std::vector<std::string> components;

  for ( const auto& part : dir ) {
    if ( std::string component = part.string(); !component.empty() && component != "/" ) {
      components.push_back( component );
    }
  }

  std::ostringstream shortened_dir;
  for ( size_t i = 0; i < components.size(); ++i ) {
    if ( i < components.size() - 1 ) {
      shortened_dir << components[ i ][ 0 ] << "/";
    }
    else {
      shortened_dir << components[ i ];
    }
  }

  return shortened_dir.str();
}

std::string log::generate_new_session_header_( const std::string& filename ) {

  // Lambda to generate time string
  const auto get_time_string_ = []() -> std::string {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t( now );
    const std::tm* local_time = std::localtime( &now_time );

    if ( !local_time ) {
      return "unknown-time"; // Fallback if time retrieval fails
    }

    char buffer[ 32 ];
    std::strftime( buffer, sizeof( buffer ), "%Y-%m-%d %H:%M:%S", local_time );
    return { buffer };
  };

  // Format the header
  return "\n-[" + filename + "]----------------" + get_time_string_() + "------------------[" + filename + "]-\n";
}

log::stream_redirect_::~stream_redirect_() {
  std::cout.rdbuf( original_cout_ );
  std::cerr.rdbuf( original_cerr_ );
}

} // namespace nutsloop
