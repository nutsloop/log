#include "log.h++"

#include <atomic>
#include <csignal>
#include <iostream>
#include <random>
#include <thread>

// Global atomic flag to signal when to stop the thread
std::atomic<bool> running( true );

// Signal handler to catch CTRL-C
void signal_handler( const int signal ) {
    if ( signal == SIGINT ) {
        running = false;
    }
}

// this main function is only used to test the log library
// it is not meant to be compiled or run directly
int main() {

  log::activate();

  const nutsloop::log_settings_t llog_settings(
    "log",
    "log.log",
    true,
    std::nullopt,
    std::nullopt
  );
  log::set( llog_settings );

  const auto llog_instance = log::get_instance( llog_settings.ident );
  log::stream( "log", "", *"", nutsloop::Level::WARN ) << '\n'
    << llog_instance->ident() << '\n';
  llog_instance->ostream() << "llog_ostream >> Hello World!" << '\n';

  log::activate_stream_redirect();
  std::cout << "std::cout >> Hello World!" << '\n';
  std::cerr << "std::cerr >> Hello World!" << '\n';

  LOG << "Hello World!" << '\n';

  log::stream( "log" ) << "Hello World!" << '\n';
  log::stream( "log", "", *"", nutsloop::Level::NONE ) << "Hello World!" << '\n';

  // Register the signal handler for CTRL-C
  std::signal( SIGINT, signal_handler );

  // Create and start a thread
  std::thread random_stream_thread( []() {
    std::random_device rd; // Seed for random number generator
    std::mt19937 gen( rd() ); // Standard mersenne_twister_engine
    std::uniform_int_distribution<> dis( 1, 100 ); // Uniform distribution [1, 100]

    while ( running ) {
      const int random_number = dis( gen );
      LOG << "Random Number: " << random_number << '\n';
      std::this_thread::sleep_for( std::chrono::seconds( 2 ) ); // Sleep for 2 seconds
    }
  } );

  log::deactivate();

  LOG << "Goodbye World!" << '\n';

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) ); // Sleep for 1 second

  log::activate();

  LOG << "Hello World! again" << '\n';

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) ); // Sleep for 1 second

  log::deactivate();

  std::this_thread::sleep_for( std::chrono::seconds( 10 ) ); // Sleep for 10 seconds
  log::activate();

  std::thread start_and_stop_stream_log( []() {

    const nutsloop::log_settings_t start_stop_log(
      "start_stop",
      "start_stop.log",
      true,
      std::nullopt,
      std::nullopt
    );
    log::set( start_stop_log );
    log::stop( "start_stop" );
    LOG << "start_stop set to running false" << '\n';

    while ( running ) {

      LOG << "trying to log to NOT running START_STOP log" << '\n';
      log::stream( "start_stop" ) << "Hello World! NOT RUNNING" << '\n';
      LOG << "start_stop set to running true" << '\n';
      log::start( "start_stop" );
      LOG << "trying to log to running START_STOP log" << '\n';
      log::stream( "start_stop" ) << "Hello World! RUNNING" << '\n';
      LOG << "start_stop set to running false" << '\n';
      log::stop( "start_stop" );

      std::this_thread::sleep_for( std::chrono::seconds( 3 ) ); // Sleep for 3 seconds
    }
  });

  // Wait for the threads to finish
  start_and_stop_stream_log.join();
  random_stream_thread.join();

  // Flush the log file created by the thread start_and_stop_stream_log
  log::flush( "start_stop" );
  llog_instance->flush();

  return 0;
}
