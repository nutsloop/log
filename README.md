# nutsloop::log

`nutsloop::log` is a thread-safe C++ logging library designed for efficient and flexible logging to file systems.
It supports dynamic log configuration, multithreaded environments, and provides mechanisms to group and manage logs
with unique identifiers.
This library is particularly useful for creating isolated log files per module, session tracking,
and redirecting `std::cout` and `std::cerr` output to log files.

___

## Features

- **Thread-Safe Logging**: Ensures logging operations are safe to use across multiple threads.
- **Log File Management**: Logs can be grouped and written to different files, identified by unique identifiers (`ident`).
- **Stream Redirection**: Redirects `std::cout` and `std::cerr` to the configured log files.
- **Session Tracking**: Provides session headers in logs to separate logs across different runs.
- **Resource Cleanup**: Automatically closes all log files and restores original stream states during cleanup.
- **Logging Levels**: Supports log levels (`INFO`, `WARN`, `ERROR`) with user-defined flexibility.
- **File Size Management**: Handles large log files by archiving them when they exceed a threshold size.

___

## Installation

will be provided.

___

## Initialization and Usage

### 1. Logger Setup

Use the `setup` function to initialize a logger for a specific module or purpose.
This sets up the log file, writes session headers, and redirects `std::cout` and `std::cerr` if required.

``` c++
#include "log.h++"

nutsloop::log::setup("example_logger", "example.log");
```

- `example_logger`: Unique identifier for the logger.
- `example.log`: File name where logs will be written.

**Optional Parameters**:
- `enabled`: Pass `false` to disable logging for this identifier.

___

### 2. Writing Logs

Logs are written using the `stream()` method. You can specify the log level (`INFO`, `WARN`, `ERROR`) and context information (source file and line number).

``` c++
nutsloop::log::stream("example_logger", __FILE__, __LINE__, 'I') << "This is an info message.\n";
nutsloop::log::stream("example_logger", __FILE__, __LINE__, 'E') << "This is an error message.\n";
```

- The log message includes metadata such as log level, source file, and line number for easy debugging.
- If the provided identifier is invalid or logging is disabled, logs will be silently ignored.

### 3. Cleaning Up

Before program termination, ensure to call `cleanup` to release resources, close opened log files, and restore stream states.

``` c++
nutsloop::log::cleanup();
```

### 4. Disabling Logs Globally

You can disable all logging globally by using the `set_logs` function:

``` c++
nutsloop::log::set_logs(true); // Disables logging globally
```

Set it back to `false` to enable logging again.

___

## File Size Management

Logs exceeding **10 MB** will be archived automatically by renaming the current file to `<filename>.backup` and starting a new session in a fresh file.

___

## Example Workflow

``` c++
#include "log.h++"

int main() {
    // Initialize logger
    nutsloop::log::setup("app", "app.log");

    // Start logging
    for (int i = 0; i < 10; ++i) {
        nutsloop::log::stream("app", __FILE__, __LINE__, 'I') << "Iteration " << i << "\n";
    }

    // Clean up resources
    nutsloop::log::cleanup();

    return 0;
}
```

## Notes

1. **Environment**: Default directories (`~/.nutsloop/logs`) are used for storing logs. Ensure the program has the necessary permissions to create these directories.
2. **Customization**: Users can extend and modify the library to fit specific needs (e.g., adding custom log levels).
3. **Fallbacks**: If a log file cannot open, warnings are printed to `std::cerr`.

## Contributions

Contributions are welcome! Open an issue or submit a PR to propose enhancements or report bugs.

## License

This library is licensed under the **Apache-2.0**.
