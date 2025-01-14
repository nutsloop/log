#include "log.h++"

#include <atomic>
#include <csignal>
#include <iostream>
#include <random>
#include <thread>

// Global atomic flag to signal when to stop the thread
std::atomic<bool> running(true);

// Signal handler to catch CTRL-C
void signal_handler(int signal) {
    if (signal == SIGINT) {
        running = false;
    }
}

// this main function is only used to test the log library
// it is not meant to be compiled or run directly
int main() {

  #define LOG nutsloop::log::stream( "log", __FILE__, __LINE__, 'I' )
  const nutsloop::log_settings_t llog_settings{
    .ident = "log",
    .filename = "log.log",
    .active = true
  };

  log::activate();
  log::activate_stream_redirect();
  log::set( llog_settings );

  std::cout << "std::cout >> Hello World!" << '\n';
  std::cerr << "std::cerr >> Hello World!" << '\n';

  LOG << "Hello World!" << '\n';

  // Register the signal handler for CTRL-C
  std::signal(SIGINT, signal_handler);

  // Create and start a thread
  std::thread random_stream_thread([]() {
      std::random_device rd;  // Seed for random number generator
      std::mt19937 gen(rd()); // Standard mersenne_twister_engine
      std::uniform_int_distribution<> dis(1, 100); // Uniform distribution [1, 100]

      while (running) {
          int random_number = dis(gen);
          LOG << "Random Number: " << random_number << '\n';
          std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 1 second
      }
  });



  log::deactivate();

  LOG << "Goodbye World!" << '\n';

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) ); // Sleep for 1 second ( to allow the log to flush

  log::activate();

  LOG << "Hello World! again" << '\n';

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) ); // Sleep for 1 second ( to allow the log to flush

  log::deactivate();

  std::this_thread::sleep_for( std::chrono::seconds( 10 ) ); // Sleep for 1 second ( to allow the log to flush
  log::activate();

  // Wait for the thread to finish
  random_stream_thread.join();

  return 0;
}
