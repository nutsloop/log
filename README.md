# nutsloop::log
###### a thread-safe logging system.

`nutsloop::log` is a thread-safe C++ logging library designed for efficient and flexible logging to file systems.
It supports dynamic log configuration, multithreaded environments, and provides mechanisms to group and manage logs
with unique identifiers.

___

## Features

- **Thread-Safe Logging**: Logging operations are safe to use across multiple threads.
- **Log File Management**: Logs can be grouped and written to different files, identified by unique identifiers (`ident`).
- **Stream Redirection**: Redirects `std::cout` and `std::cerr` to the predefined log files.
- **Session Tracking**: Provides session headers in logs to separate logs across different runs.
- **Resource Cleanup**: Automatically closes all log files and restores original stream states ( `std::cout` and `std::cerr` if them where redirect).
- **Logging Levels**: Supports log levels (`INFO`, `WARN`, `ERROR`, `NONE`) with user-defined flexibility.
- **File Size Management**: Handles large log files by archiving them when they exceed the threshold size of 10MB.

___

## Installation

This serves as a fundamental installation procedure.

**Note on Debug and Release Builds:**

_Release-type build of the library._
- It disables the library’s internal debug logs.
- It is indeed faster and space-efficient.

**_Debug-type build of the library._**

- It enables the library’s internal debug logs.
- The debug file location is under the directory `/var/tmp`.
- The filename is generated with a unique name, such as  
  `/var/tmp/15-01-2025_07:02:34_nutsloop_lognyipfv`.
- At the beginning of your program, you will be prompted to decide what to do with older logs.
  1. You can delete all of them or select one to be reassigned.
  2. Choosing to delete all of them will create a new log file.
  3. You can selectively delete one file at a time and then select one to be reassigned or anyway create a new file.
  4. Alternatively, you may choose to retain the existing file/s and create a new one. 

> ⚠ library debug logs can occupy a lot of disk space.

```bash

# simply clone it form GitHub and cd into the root directory of the repository.
git clone https://github.com/nutsloop/log.git && cd log
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release

```

Once the library has been created, link it to your program,  
and add the root directory of the repository to the include paths of your program.

> ⚠ By default, it builds a static library.

___

## Initialization and Usage

The whole class is a container of static methods, and it is not necessary to an instance of the class.
It has default directories, can be changed if specified, located in the user home directory.  
`/home/user/.nutsloop/logs`

It has a default header, can be changed if specified, so formatted:  
`◀︎[identifier]---------17-01-2025_11:16:35---------[/home/user/.nutsloop/logs/filename.log]▶︎`

### Setting up a log.

Use the `set` method to initialize a logger for a specific module or purpose.

#### **log_settings_t**
###### the log settings struct.

```c++
struct log_settings_t {
  std::string ident;
  std::string filename;
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> session_header;
  bool active;
};
```

| Field Name       | Type                                   | Description                                                         |
|------------------|----------------------------------------|---------------------------------------------------------------------|
| `ident`          | `std::string`                          | Identifier or name for the log.                                     |
| `filename`       | `std::string`                          | The name of the log file.                                           |
| `directory`      | `std::optional<std::filesystem::path>` | Optional directory path where the log file will be stored.          |
| `session_header` | `std::optional<std::string>`           | Optional session-specific header to prepend or include in the log.  |
| `active`         | `bool`                                 | Indicates whether logging is active (`true`) or inactive (`false`). |

___

#### **nutsloop::log::set(`const log_settings_t& settings`)**
###### the log set method.

This method can be called without an instance of the class.  
Accepts only one parameter, that is a reference to the struct `log_settings_t`.
It sets one unique log, using the provided `log_settings_t`.

___

``` c++
#include "log.h++"

// let's give the log basic settings.
const nutsloop::log_settings_t llog_settings{
  .ident = "log", // a unique identifier
  .filename = "log.log", // a filename and only, no relative or absolute path needed.
  .active = true // this set this log active, but not the logging system itself.
};

nutsloop::log::setup( llog_settings );
```

- `example_logger`: Unique identifier for the logger.
- `example.log`: File name where logs will be written.

**Optional Parameters**:
- `enabled`: Pass `false` to disable logging for this identifier.

___

### Writing Logs

Logs are written using the `stream()` method. You can specify the log level (`INFO`, `WARN`, `ERROR`) and context information (source file and line number).

``` c++
nutsloop::log::stream("example_logger", __FILE__, __LINE__, 'I') << "This is an info message.\n";
nutsloop::log::stream("example_logger", __FILE__, __LINE__, 'E') << "This is an error message.\n";
```

- The log message includes metadata such as log level, source file, and line number for easy debugging.
- If the provided identifier is invalid or logging is disabled, logs will be silently ignored.

### Cleaning Up

Before program termination, ensure to call `cleanup` to release resources, close opened log files, and restore stream states.

``` c++
nutsloop::log::cleanup();
```

### Disabling Logs Globally

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
