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

**_command to build it as a shared library._**

```bash

# simply clone it form GitHub and cd into the root directory of the repository.
git clone https://github.com/nutsloop/log.git && cd log
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build/release

```

___

## Initialization and Usage

The whole class is a container of static methods, and it is not necessary to instantiate the class.
In fact, the constructors are ` = delete` in the library itself.

It has default directories, can be changed if specified, located in the user home directory.

`/home/user/.nutsloop/logs`

**custom directory**

> ⚠ this feature is not implemented yet.

___

It has a default header, can be changed if specified, so formatted:
`◀︎[identifier]---------17-01-2025_11:16:35---------[/home/user/.nutsloop/logs/filename.log]▶︎`

**custom header**

> ⚠ this feature is not implemented yet.

___

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

Accepts only one parameter, that is a reference to the struct `log_settings_t`.
It sets one unique log, using the provided `log_settings_t`.

___

``` c++
#include "log.h++"

// let's assume that the below code is wrapped in the main function.

// let's give the log basic settings.
const nutsloop::log_settings_t llog_settings{
  .ident = "log", // a unique identifier
  .filename = "log.log", // a filename and only, no relative or absolute path needed.
  .active = true // this set this log active, but not the logging system itself.
};

nutsloop::log::setup( llog_settings );
```

_example continues below_

___

### Writing Logs

| **Name**        | **Details**                                                                                       |
|-----------------|---------------------------------------------------------------------------------------------------|
| **Signature**   | `static std::ostream& stream(const char* ident, const char* file, int line, Level level = INFO);` |
| **Description** | This method returns a reference to an output stream for logging purposes.                         |
| **Parameters**  |                                                                                                   |
| `ident`         | A `const char*` that represents the identifier of the message source.                             |
| `file`          | A `const char*` that specifies the file path where the method is called.                          |
| `line`          | An `int` representing the line number in the code where this method is invoked.                   |
| `level`         | An optional `Level` indicating the log level. Default value: `INFO`.                              |
| **Returns**     | A reference to `std::ostream` used for logging.                                                   |

**Notes**

- The `Level` type is expected to be an enumerated type (e.g., `enum`)
  ```c++
  enum Level {
    INFO,
    WARN,
    ERROR,
    NONE
  };
  ```

This function facilitates structured logging by accepting relevant metadata (identifier, file, line, and log level).

___

_example continuation_

```c++
// first let's activate the log system
// without this the log won't write to file/s
nutsloop::log::activate();
// no need, to call `activate` again cause is a global atomic bool

nutsloop::log::stream("log", __FILE__, __LINE__, nutsloop::nlog::types::Level::INFO) << '\n'
  << "main has been called" << '\n';

```
 

___

### Cleaning Up

> ⚠ documentation is not written yet.

___

### Disabling Logs Globally

> ⚠ documentation is not written yet.

___

## File Size Management

Logs exceeding **10 MB** will be archived automatically by renaming the current file to `<filename>.backup` and starting a new session in a fresh file.

> ⚠ this feature is partially implemented.  
> it split the log file only when `fresh` running the application.  
> needs to do so during runtime.

___

## Example Workflow

___

## Notes

- **Environment**: Default directories (`~/.nutsloop/logs`) are used for storing logs. Ensure the program has the necessary permissions to create these directories.
- **Debug log directory if built in Debug**: `/var/tmp`

## Contributions

Contributions are welcome! Open an issue or submit a PR to propose enhancements or report bugs.

## License

This library is licensed under the **Apache-2.0**.
