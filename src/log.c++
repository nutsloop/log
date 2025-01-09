#include "log.h++"

#include <cassert>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace nutsloop {

// HINT: log internal debug file.
constexpr bool DEBUG = true;

std::atomic<bool> log::tmp_debug_file_already_set_{ false };
std::atomic<std::string*> log::tmp_debug_file_path_{};
std::unique_ptr<std::ofstream> log::debug_tmp_file_stream_{ nullptr };

std::shared_mutex log::mtx_;
std::atomic<bool> log::set_has_been_called_{ false };
std::atomic<bool> log::activated_{ false };

std::unique_ptr<log_registry_t> log::log_registry_{ nullptr };
std::unique_ptr<log_t> log::log_{ nullptr };

std::unique_ptr<log::stream_redirect_> log::stream_redirect_pointer_ = nullptr;
log::null_logger_ log::null_log_;

void log::activate() {

    const bool previous_activate_status = activated_.exchange( true );
    if ( DEBUG ) {
      {
        // MARK (LOG) MUTEX LOCK
        std::shared_lock lock{ mtx_ };
        debug_file_is_active_();
        debug_stream_( __FILE__, __LINE__, 'I' ) << "log::activate() called ⇣" << '\n'
          << "  activated_ ( "
          << " was -> [ " << std::boolalpha << previous_activate_status << " ]" // previous
          << " => now[ " << std::boolalpha << activated_ << " ] )" << std::endl; // actual
      }
    }


  if ( log_registry_ == nullptr ) {
    log_registry_ = std::make_unique<log_registry_t>();

    if ( DEBUG ) {
      {
        std::shared_lock lock{ mtx_ };
        const auto log_registry_address = reinterpret_cast<uintptr_t>(&log_registry_);
        debug_stream_( __FILE__, __LINE__, 'I' ) << "log_registry_ is nullptr, creating..." << '\n'
          << std::format("pointer with address -> 0x{:x}", log_registry_address) << std::endl;
      }
    }
  }
}

void log::deactivate() {

    const bool previous_activate_status = activated_.exchange( false );
    if ( DEBUG ) {
      { // MARK (LOG) MUTEX LOCK
        std::unique_lock lock( mtx_ );
        debug_file_is_active_();
        debug_stream_( __FILE__, __LINE__, 'I' ) << "log::deactivate() called ⇣" << '\n'
          << "  activated_ ("
          << " was -> [ " << std::boolalpha << previous_activate_status << " ]" // previous
          << " => now[ " << std::boolalpha << activated_ << " ] )" << std::endl; // actual
      }
    }

}

void log::set( const log_settings_t& settings ) {

  if ( log_registry_ != nullptr && log_registry_->contains( settings.ident ) ) {

    if ( DEBUG ) {
      {
        std::shared_lock<std::shared_mutex> lock( mtx_ );
        log_t* log_ident = &log_registry_->at( settings.ident );
        if ( debug_tmp_file_stream_->is_open() ) {

          debug_stream_( __FILE__, __LINE__, 'W' )
            << "log::set() called ⇣" << '\n'
            << std::format( "  log_registry_ has -> [ {} ]", log_ident->settings.ident ) << '\n'
            << std::format( "  log with ident -> [ {} ]", log_ident->settings.ident ) << '\n'
            << std::format( "    is_active -> [ {} ]", log_ident->settings.active ? "true" : "false" ) << '\n'
            << std::format( "    is_running -> [ {} ]", log_ident->running ? "true" : "false" ) << '\n'
            << std::format( "  use: `log::start( '{}' )` | `log::stop( '{}' )`", settings.ident, settings.ident ) << '\n'
            << std::format( "  to change the running state for log with ident -> `{}`.", settings.ident ) << std::endl;
        }
      }
    }
    return;
  }

  // HINT: maybe find a purpose for this? :D
  if ( is_set_called_() ) {}

  const bool previous_set_status = set_has_been_called_.exchange( true );

  if ( DEBUG ) {
    { // MARK (LOG) MUTEX LOCK
      std::shared_lock<std::shared_mutex> lock( mtx_ );
      debug_file_is_active_();

      debug_stream_( __FILE__, __LINE__, 'I' ) << "log::set() called ⇣" << '\n'
        << "  set_has_been_called_ ("
        << " was -> [ " << std::boolalpha << previous_set_status << " ]" // previous
        << " => now[ "<< std::boolalpha << set_has_been_called_ << " ] )" << std::endl; // actual
    }
  }

  if ( ! is_activated_() ) {


    if ( DEBUG ) {
      {
        std::shared_lock<std::shared_mutex> lock( mtx_ );
        debug_stream_( __FILE__, __LINE__, 'W' ) << "log::set called ⇣" << '\n'
          << "  log system is not active. " << '\n'
          << "  use `log::activate()` to activate the log system." << std::endl;
      }
    }
    return;
  }

  uintptr_t log_address;
  if ( log_ == nullptr ) {

    log_ = std::make_unique<log_t>();
    if ( DEBUG ) {
      {
        std::shared_lock lock( mtx_ );
        log_address = reinterpret_cast<uintptr_t>(&log_);
        debug_stream_( __FILE__, __LINE__, 'I' ) << "log_ is nullptr, construct..." << '\n'
          << std::format("pointer with address -> 0x{:x}", log_address) << std::endl;
      }
    }
  }

  log_->settings = settings;
  log_->stream = std::ofstream();
  log_->running = settings.active;

  log_registry_->insert_or_assign( settings.ident, std::move( *log_ ) );

  log_.reset();
  if ( DEBUG ) {
    {
      std::shared_lock lock( mtx_ );
      debug_stream_( __FILE__, __LINE__, 'I' ) << "log_ is being destroyed..." << '\n'
        << std::format("pointer with address -> 0x{:x}", log_address) << std::endl;
    }
  }

  log_t* log_ident = &log_registry_->at( settings.ident );

  if ( ! log_ident->settings.directory ) {
    if ( DEBUG ) {
      {
        std::shared_lock lock( mtx_ );
        debug_stream_( __FILE__, __LINE__, 'I' ) << "log custom directory is not set. " << '\n'
          << "  using default directory: " << nutsloop_logs_directory << '\n'
          << "  use `log::set( ident, settings )` to set a custom directory." << std::endl;
      }
    }

    // make the default directories
    mkdir_default_( nutsloop_dir );
    mkdir_default_( nutsloop_logs_directory );
  }

  bool renamed = false;
  const std::filesystem::path log_file_path = nutsloop_logs_directory / log_ident->settings.filename;
  if ( std::filesystem::exists( log_file_path ) && std::filesystem::file_size( log_file_path ) > LOG_MAX_SIZE ) {
    const std::string backup_path = nutsloop_logs_directory / ( log_ident->settings.filename + ".backup" );
    if ( log_ident->stream.is_open() ) {
      log_ident->stream.close();
    }
    std::filesystem::rename( log_file_path, backup_path ); // Archive existing log
    renamed = true;
  }

  log_ident->stream.open( log_file_path, std::ios::out | ( renamed ? std::ios::trunc : std::ios::app ) );
  std::optional<std::string> log_file_stream_error = error_on_log_file_( log_ident );
  if ( log_file_stream_error ) {
    throw std::runtime_error( *log_file_stream_error );
  }

  // Setup stream redirection only once
  if ( !stream_redirect_pointer_ ) {
    stream_redirect_pointer_ = std::make_unique<stream_redirect_>( std::cout, std::cerr, log_ident->stream.rdbuf() );
  }

  log_ident->stream << std::unitbuf;

  // Add the default session header if the custom one has not been set.
  if ( !log_ident->settings.session_header ) {
    log_ident->stream << generate_new_session_header_( log_ident->settings.ident,
                                                         log_file_path ); // Add session header
  }

}

std::ostream& log::stream( const char* ident, const char* file, const int line, const char level /*= 'I'*/ ) {

  if ( !is_activated_() ) return null_log_;

  { // MARK (LOG) MUTEX LOCK
    std::shared_lock lock( mtx_ );

    // Check if the log_registry is nullptr
    if (log_registry_ == nullptr) {
      std::cerr << "[ERROR] log_registry_ is nullptr\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    // Check if the identifier exists in log_registry_
    if ( !log_registry_->contains( ident ) ) {
      std::cerr << "[ERROR] Invalid log identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    log_t* log_ident = &log_registry_->at( ident );

    // Check if the log is active
    if ( !log_ident->settings.active ) {
      std::cerr << "[ERROR] Logging is disabled for identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    // Check if the log is running
    if ( !log_ident->running ) {
      std::cerr << "[ERROR] Log is not running for identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }

    // Check if the log file stream is open
    if ( !log_ident->stream.is_open() ) {
      std::cerr << "[ERROR] Log file stream is not open for identifier '" << ident << "'.\n";
      // IDEA: Consider throwing an exception or handling error specifically
      return null_log_;
    }


    // Write log prefix with level, file, and line
    log_ident->stream << "[" << level_( level ) << "] "
           << "[" << shortened_path_( file ) << ":" << line << "] ";
    return log_ident->stream;
  }
}

// MARK (LOG) PRIVATE METHODS

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

bool log::debug_tmp_file_reassign_or_cleanup_() {

  const std::string directory_path = "/var/tmp";
  std::vector<std::filesystem::path> debug_files_found;

  for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path)) {
    if (entry.is_regular_file()) {
      if ( const auto& file_name = entry.path().filename().string();
           file_name.find("nutsloop_log") != std::string::npos) {
        debug_files_found.push_back(entry.path());
      }
    }
  }

  if (debug_files_found.empty()) {
    std::cout << "no debug file found.\n";

    return false;
  }

  std::cout << "found (" << debug_files_found.size() << ") log class debug files." << '\n';
  for (const auto& file_path : debug_files_found) {
    std::cout << "  " << file_path << "\n";
  }

  if ( debug_files_found.size() > 1 ) {
    std::cout << "multiple log class debug files found." << '\n'
              << "Do you want to delete all of them? (y/n): ";
    char response;
    std::cin >> response;

    if ( response == 'y' || response == 'Y' ) {
      for ( const auto& file_path : debug_files_found ) {
        try {
          std::filesystem::remove( file_path );
          std::cout << "Deleted: " << file_path << "\n";
        } catch ( const std::filesystem::filesystem_error& e ) {
          std::cerr << "Error deleting file: " << e.what() << "\n";
        }
      }
      return false;
    }
  }

  for (const auto& file_path : debug_files_found) {
    std::cout << std::format("Do you want to reassign the stream to {}? (y/n): ", file_path.string());

    char response;
    std::cin >> response;

    if ( response == 'y' || response == 'Y' ) {
      tmp_debug_file_path_.store( new std::string( file_path.string() ) ); // Atomic update
      tmp_debug_file_already_set_.store( true );
      std::cout << "Reassigned: " << file_path << "\n";

      return true;
    }

    std::cout << "Do you want to delete this file? (y/n): ";
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
      try {
        std::filesystem::remove(file_path);
        std::cout << "Deleted: " << file_path << "\n";
      } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting file: " << e.what() << "\n";
      }
    }
  }

  return false;
}

void log::debug_tmp_file_create_() {

  // first a bit of cleanup or reassign
  if ( debug_tmp_file_reassign_or_cleanup_() )
    return;

  std::string tmp_path = std::format( "/var/tmp/{}_nutsloop_logXXXXXX", datetime_() );
  const int fd = mkstemp( tmp_path.data() ); // Secure and unique
  if ( fd == -1 ) {
    perror( "mkstemp failed" );
    exit( EXIT_FAILURE );
  }
  //#include <unistd.h>
  if ( ::close( fd ) == -1){
    perror( "close failed" );
    exit( EXIT_FAILURE );
  }

  // Store the temp file path safely
  tmp_debug_file_path_.store( new std::string( tmp_path ) ); // Atomic update
  tmp_debug_file_already_set_.store( true );
  std::cerr << "temporary debug file created: " << *tmp_debug_file_path_.load() << '\n';
}

std::ofstream& log::debug_stream_( const char* file, const int line_number, const char level ) {

  return dynamic_cast<std::ofstream&>( *debug_tmp_file_stream_ << "[" << level_( level ) << "] "
                                                               << "[" << shortened_path_( file ) << ':'
                                                               << line_number << "] " );
}

bool log::is_activated_() {

  if ( DEBUG ) {
    debug_stream_( __FILE__, __LINE__, 'I' ) << "log::is_activated_() called ⇣" << '\n'
      << "  log system is active: [ " << std::boolalpha << activated_.load() << " ] " << std::endl; // actual
  }

  return activated_.load();
}

bool log::is_set_called_() {

  if ( DEBUG ) {
    debug_stream_( __FILE__, __LINE__, 'I' ) << "log::is_set_called_() called ⇣" << '\n'
      << "  log has been set: [ " << std::boolalpha << set_has_been_called_ << " ] " << std::endl; // actual
  }

  return set_has_been_called_.load();
}

std::optional<std::string> log::error_on_log_file_( const log_t* log_ident ) {

  if (log_ident->stream.fail()) {
    const std::string error_ident =  "Error opening log file: " + log_ident->settings.filename + "\n";
    if (log_ident->stream.rdstate() & std::ios::failbit) {
      return error_ident + "Logical error on i/o operation\n";
    }
    if (log_ident->stream.rdstate() & std::ios::badbit) {
      return error_ident + "Read/writing error on i/o operation\n";
    }
    if (log_ident->stream.rdstate() & std::ios::eofbit) {
      return error_ident + "End-of-file reached prematurely\n";
    }
  }
  return std::nullopt;
}

void log::mkdir_default_( const std::filesystem::path& path){
  try {
    if ( !std::filesystem::exists( path ) ) {
      std::filesystem::create_directory( path );
    }else {
      if ( DEBUG ) {

        {
          std::shared_lock lock( mtx_ );
          debug_stream_( __FILE__, __LINE__, 'W' )
            << "default directory '" << path << "' already exists." << std::endl;
        }
      }
    }
  }
  catch ( const std::filesystem::filesystem_error& e ) {
    std::cerr << "Error creating directory '" << path << "': " << e.what() << "\n";
  }
}

const char* log::level_( const char level ) {

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

std::string log::generate_new_session_header_( const std::string& ident, const std::string& filename ) {
  return "\n"  "◀︎[" + ident + "]---------" + datetime_() + "---------[" + filename + "]▶︎" + "\n" + "\n";
}

std::string log::datetime_() {

  std::string time_now;

  try {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Use `std::put_time` for formatting
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%d-%m-%Y_%H:%M:%S");
    time_now = oss.str();
  } catch (const std::exception&) {
    time_now = "unknown-time";  // Fallback in case of formatting issues
  }

  if (DEBUG) {

    if ( debug_tmp_file_stream_ != nullptr && debug_tmp_file_stream_->is_open() ) {
      *debug_tmp_file_stream_ << '\n' << "----------------------------------------------------------------------" << '\n';
      debug_stream_( __FILE__, __LINE__, 'I' )
        << "datetime_() called ⇣" << '\n'
        << "now => [ " << time_now << " ]" << '\n'
        << "this is happening before new session header." << '\n'
        << "----------------------------------------------------------------------" << std::endl;
    }
  }

  return time_now;
}

log::stream_redirect_::stream_redirect_(std::ostream& stream_out, std::ostream& stream_err, std::streambuf* log_file_buffer) :
      original_cout_(stream_out.rdbuf()),
      original_cerr_(stream_err.rdbuf()) {

  stream_out.rdbuf(log_file_buffer);
  stream_err.rdbuf(log_file_buffer);
}

log::stream_redirect_::~stream_redirect_() {
  std::cout.rdbuf( original_cout_ );
  std::cerr.rdbuf( original_cerr_ );
}

}
