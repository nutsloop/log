#include "log.h++"

// this main function is only used to test the log library
// it is not meant to be compiled or run directly
int main() {

  log::setup( "main", "main.log", true );

  log::stream( "main", __FILE__, __LINE__, 'I' ) << "Hello, world!\n";
  log::stream( "main", __FILE__, __LINE__, 'W' ) << "Hello, world!\n";
  log::stream( "main", __FILE__, __LINE__, 'E' ) << "Hello, world!\n";
  return 0;
}
